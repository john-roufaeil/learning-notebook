# Node.js Advanced Topics — Interview Study Guide

> **Assumption**: You know basic Node.js (require/import, basic Express routes, npm). This guide covers the advanced topics that separate junior from senior Node developers in interviews.

---

# PART 1: NODE.JS ARCHITECTURE — HOW IT ACTUALLY WORKS

This is the single most important section. Most Node interviews start here.

---

## 1.1 The Event Loop in Node.js

### What is it?

Node.js is single-threaded but handles thousands of concurrent connections through an event-driven, non-blocking I/O model. The **Event Loop** is the mechanism that makes this possible.

### Why does it exist?

Traditional servers (Apache) create a new thread per connection — 1000 connections = 1000 threads, each consuming ~2MB of RAM. Node.js uses one thread and an event loop, handling I/O asynchronously. This makes it extremely memory-efficient for I/O-heavy workloads.

### Node.js Event Loop Phases (in order)

```
┌──────────────────────────────────────┐
│           timers                      │  setTimeout, setInterval callbacks
│           pending callbacks           │  I/O callbacks deferred to next loop
│           idle, prepare              │  internal use
│           poll                        │  retrieve new I/O events ← MOST TIME HERE
│           check                       │  setImmediate callbacks
│           close callbacks            │  close event callbacks
└──────────────────────────────────────┘
   Between each phase: process.nextTick() and Promise microtasks run
```

```javascript
const fs = require("fs");

console.log("1 - Start");

setTimeout(() => console.log("2 - setTimeout"), 0);

setImmediate(() => console.log("3 - setImmediate"));

process.nextTick(() => console.log("4 - nextTick"));

Promise.resolve().then(() => console.log("5 - Promise"));

fs.readFile(__filename, () => {
    // Inside an I/O callback (poll phase):
    setTimeout(() => console.log("6 - setTimeout inside I/O"), 0);
    setImmediate(() => console.log("7 - setImmediate inside I/O"));
    // Inside I/O: setImmediate ALWAYS runs before setTimeout
});

console.log("8 - End");

// Output:
// 1 - Start
// 8 - End
// 4 - nextTick       (runs before Promise microtasks)
// 5 - Promise        (microtask)
// 2 - setTimeout     (timers phase)
// 3 - setImmediate   (check phase — order vs setTimeout is non-deterministic outside I/O)
// 7 - setImmediate inside I/O  (setImmediate ALWAYS wins inside I/O callbacks)
// 6 - setTimeout inside I/O
```

### libuv — the engine under the hood

```
Node.js
├── V8 engine          (executes JS)
├── libuv              (event loop, thread pool, OS async)
│   ├── Thread pool    (4 threads by default — for fs, crypto, DNS)
│   └── Async I/O      (network via OS: epoll/kqueue/IOCP)
└── Node.js bindings   (connect JS to C++ APIs)
```

**Important**: Network I/O (TCP, HTTP) is truly async via the OS. File system, crypto, DNS, and zlib use **libuv's thread pool** (4 threads by default, configurable with `UV_THREADPOOL_SIZE`).

### Interview talking points
- Node is single-threaded but NOT single-threaded for I/O — libuv's thread pool handles blocking operations.
- `process.nextTick()` runs before any other microtasks — use sparingly, can starve the event loop.
- `setImmediate` runs in the check phase (after I/O). Inside an I/O callback, `setImmediate` always runs before `setTimeout`.
- Blocking the event loop (heavy computation in the main thread) = all other requests are blocked.

---

## 1.2 Streams

### What is it?

Streams are Node.js's way of handling data piece by piece instead of loading everything into memory. A stream is an EventEmitter that reads or writes data in chunks.

### Why does it exist?

Without streams, `fs.readFile("10gb-file.txt")` would load 10GB into RAM before you could process a single byte. Streams process the file chunk by chunk with constant, small memory usage.

### Four types of streams

```javascript
const { Readable, Writable, Transform, Duplex } = require("stream");

// 1. Readable — source of data
const readable = new Readable({
    read(size) {
        // Push data into the stream
        this.push("chunk of data ");
        this.push("more data ");
        this.push(null);  // Signals end of stream
    }
});

// 2. Writable — destination for data
const writable = new Writable({
    write(chunk, encoding, callback) {
        console.log("Received:", chunk.toString());
        callback();  // Signal that you're done processing this chunk
    }
});

// 3. Transform — read + write + transform (Duplex with transformation)
const { Transform } = require("stream");

const upperCaseTransform = new Transform({
    transform(chunk, encoding, callback) {
        this.push(chunk.toString().toUpperCase());
        callback();
    }
});

// 4. Pipe — connect streams together (cleanest API)
const fs = require("fs");
const zlib = require("zlib");

// Compress a large file — uses CONSTANT memory regardless of file size
fs.createReadStream("large-file.txt")
    .pipe(upperCaseTransform)
    .pipe(zlib.createGzip())
    .pipe(fs.createWriteStream("large-file.txt.gz"))
    .on("finish", () => console.log("Done compressing"));
```

### Real-world example: streaming HTTP response

```javascript
const http = require("http");
const fs = require("fs");

// BAD — loads entire file into memory before sending
app.get("/bad", (req, res) => {
    fs.readFile("./large-video.mp4", (err, data) => {
        res.end(data);  // 2GB file = 2GB in memory before a single byte is sent!
    });
});

// GOOD — streams directly to client
app.get("/good", (req, res) => {
    const stream = fs.createReadStream("./large-video.mp4");
    stream.pipe(res);  // Constant memory, starts sending immediately
});

// BETTER — with error handling and range support
app.get("/better", (req, res) => {
    const filePath = "./large-video.mp4";
    const stat = fs.statSync(filePath);
    const fileSize = stat.size;
    const range = req.headers.range;

    if (range) {
        const [start, end] = range.replace(/bytes=/, "").split("-").map(Number);
        const chunkSize = end - start + 1;

        res.writeHead(206, {
            "Content-Range": `bytes ${start}-${end}/${fileSize}`,
            "Content-Length": chunkSize,
            "Content-Type": "video/mp4"
        });

        fs.createReadStream(filePath, { start, end }).pipe(res);
    } else {
        res.writeHead(200, { "Content-Length": fileSize, "Content-Type": "video/mp4" });
        fs.createReadStream(filePath).pipe(res);
    }
});
```

### Stream pipeline (preferred over pipe)

```javascript
const { pipeline } = require("stream/promises");
const zlib = require("zlib");
const fs = require("fs");

// pipeline is better than .pipe() — handles errors and cleanup automatically
async function compress(input, output) {
    await pipeline(
        fs.createReadStream(input),
        zlib.createGzip(),
        fs.createWriteStream(output)
    );
    console.log("Compression complete");
}

compress("large.csv", "large.csv.gz").catch(console.error);
```

### Interview talking points
- Streams are event-driven: `data`, `end`, `error`, `finish` are the key events.
- Back-pressure: if the writable can't keep up with the readable, `.pipe()` pauses the readable automatically.
- `stream/promises` has the `pipeline` function which handles errors better than manual `.pipe()`.
- Transform streams are the most versatile — use them for compression, encryption, parsing, etc.
- Node.js HTTP `req` and `res` are streams.

---

## 1.3 Worker Threads

### What is it?

Worker threads run JavaScript in parallel threads within the same Node.js process. Unlike `child_process`, workers share memory via `SharedArrayBuffer` and communicate via message passing.

### Why does it exist?

Node's single thread is a bottleneck for CPU-intensive tasks (image processing, ML inference, heavy computation). Worker threads let you use multiple CPU cores without spawning separate processes.

```javascript
// main.js
const { Worker, isMainThread, parentPort, workerData } = require("worker_threads");

if (isMainThread) {
    // Main thread
    function runWorker(data) {
        return new Promise((resolve, reject) => {
            const worker = new Worker(__filename, { workerData: data });
            worker.on("message", resolve);
            worker.on("error", reject);
            worker.on("exit", code => {
                if (code !== 0) reject(new Error(`Worker exited with code ${code}`));
            });
        });
    }

    async function main() {
        console.time("parallel");
        const results = await Promise.all([
            runWorker({ n: 40 }),
            runWorker({ n: 41 }),
            runWorker({ n: 42 }),
            runWorker({ n: 43 }),
        ]);
        console.timeEnd("parallel");
        console.log(results);
    }

    main();

} else {
    // Worker thread
    function fibonacci(n) {
        if (n <= 1) return n;
        return fibonacci(n - 1) + fibonacci(n - 2);
    }

    const result = fibonacci(workerData.n);
    parentPort.postMessage(result);
}
```

### Worker thread pool

```javascript
const { Worker } = require("worker_threads");
const os = require("os");

class WorkerPool {
    constructor(workerFile, poolSize = os.cpus().length) {
        this.workerFile = workerFile;
        this.pool = [];
        this.queue = [];
        this.activeWorkers = 0;

        // Pre-create workers
        for (let i = 0; i < poolSize; i++) {
            this.createWorker();
        }
    }

    createWorker() {
        const worker = new Worker(this.workerFile);
        worker.on("message", (result) => {
            // Worker finished — return to pool
            this.activeWorkers--;
            worker._resolve(result);
            worker._resolve = null;
            this.pool.push(worker);
            this.processQueue();
        });
        this.pool.push(worker);
    }

    runTask(data) {
        return new Promise((resolve, reject) => {
            const task = { data, resolve, reject };
            if (this.pool.length > 0) {
                this.executeTask(this.pool.pop(), task);
            } else {
                this.queue.push(task);
            }
        });
    }

    executeTask(worker, task) {
        this.activeWorkers++;
        worker._resolve = task.resolve;
        worker.postMessage(task.data);
    }

    processQueue() {
        if (this.queue.length > 0 && this.pool.length > 0) {
            this.executeTask(this.pool.pop(), this.queue.shift());
        }
    }
}
```

### Interview talking points
- Worker threads share the same process memory (unlike child_process) — can share `SharedArrayBuffer`.
- Use `Atomics` for synchronized access to shared memory between workers.
- Worker threads are heavy — for short tasks, the overhead outweighs the benefit. Consider a pool.
- Unlike `cluster`, worker threads share ports/memory. Unlike `child_process`, no separate V8 instances.

---

# PART 2: EXPRESS.JS AND API DESIGN

---

## 2.1 Middleware Architecture

### What is it?

Express middleware are functions in a pipeline that process requests and responses. Each middleware receives `(req, res, next)` and either responds or passes control to the next middleware with `next()`.

```javascript
const express = require("express");
const app = express();

// Middleware is executed in the order it's registered

// 1. Global middleware — runs for ALL routes
app.use((req, res, next) => {
    console.log(`${new Date().toISOString()} ${req.method} ${req.path}`);
    next();  // Pass to next middleware
});

// 2. Error-handling middleware — 4 parameters, MUST come last
app.use((err, req, res, next) => {
    console.error(err.stack);
    res.status(err.statusCode || 500).json({
        error: err.message,
        code: err.code || "INTERNAL_ERROR"
    });
});

// 3. Route-specific middleware
const authenticate = (req, res, next) => {
    const token = req.headers.authorization?.split(" ")[1];
    if (!token) return res.status(401).json({ error: "No token" });
    // Verify token...
    req.user = { id: 1, role: "admin" };  // Attach to request
    next();
};

const authorize = (roles) => (req, res, next) => {
    if (!roles.includes(req.user.role)) {
        return res.status(403).json({ error: "Forbidden" });
    }
    next();
};

app.get("/admin", authenticate, authorize(["admin"]), (req, res) => {
    res.json({ message: "Admin area" });
});
```

### Building production middleware

```javascript
// Rate limiter middleware
const rateLimiter = (options = {}) => {
    const { windowMs = 60_000, max = 100 } = options;
    const requests = new Map();

    return (req, res, next) => {
        const ip = req.ip;
        const now = Date.now();
        const windowStart = now - windowMs;

        // Clean old entries
        const ipData = requests.get(ip) || { count: 0, timestamps: [] };
        ipData.timestamps = ipData.timestamps.filter(t => t > windowStart);
        ipData.count = ipData.timestamps.length;

        if (ipData.count >= max) {
            return res.status(429).json({
                error: "Too many requests",
                retryAfter: Math.ceil(windowMs / 1000)
            });
        }

        ipData.timestamps.push(now);
        ipData.count++;
        requests.set(ip, ipData);

        res.setHeader("X-RateLimit-Limit", max);
        res.setHeader("X-RateLimit-Remaining", max - ipData.count);
        next();
    };
};

app.use("/api", rateLimiter({ windowMs: 60_000, max: 100 }));
```

---

## 2.2 Error Handling Patterns

```javascript
// Custom error classes
class AppError extends Error {
    constructor(message, statusCode = 500, code = "INTERNAL_ERROR") {
        super(message);
        this.statusCode = statusCode;
        this.code = code;
        this.isOperational = true;  // Operational vs programming errors
        Error.captureStackTrace(this, this.constructor);
    }
}

class ValidationError extends AppError {
    constructor(errors) {
        super("Validation failed", 400, "VALIDATION_ERROR");
        this.errors = errors;
    }
}

class NotFoundError extends AppError {
    constructor(resource) {
        super(`${resource} not found`, 404, "NOT_FOUND");
    }
}

// Async wrapper — eliminates try/catch in every route
const asyncHandler = fn => (req, res, next) => {
    Promise.resolve(fn(req, res, next)).catch(next);
};

// Routes — clean, no try/catch needed
app.get("/users/:id", asyncHandler(async (req, res) => {
    const user = await User.findById(req.params.id);
    if (!user) throw new NotFoundError("User");
    res.json(user);
}));

// Global error handler
app.use((err, req, res, next) => {
    // Log the error
    console.error(err);

    // Operational errors — safe to send to client
    if (err.isOperational) {
        return res.status(err.statusCode).json({
            error: err.message,
            code: err.code,
            ...(err.errors && { details: err.errors })
        });
    }

    // Programming errors — don't leak details
    res.status(500).json({ error: "Internal server error" });

    // In production, you might want to restart the process for unrecoverable errors
});

// Catch unhandled rejections and exceptions
process.on("unhandledRejection", (reason, promise) => {
    console.error("Unhandled Rejection:", reason);
    // Graceful shutdown...
});

process.on("uncaughtException", (error) => {
    console.error("Uncaught Exception:", error);
    process.exit(1);  // Must exit — app is in unknown state
});
```

---

## 2.3 RESTful API Design Best Practices

```javascript
const express = require("express");
const router = express.Router();

// Resource-based URLs (nouns, not verbs)
// GET    /users           — list all users
// POST   /users           — create a user
// GET    /users/:id       — get one user
// PUT    /users/:id       — replace a user
// PATCH  /users/:id       — update a user partially
// DELETE /users/:id       — delete a user

router.get("/", asyncHandler(async (req, res) => {
    // Pagination
    const page = Math.max(1, parseInt(req.query.page) || 1);
    const limit = Math.min(100, parseInt(req.query.limit) || 20);
    const skip = (page - 1) * limit;

    // Filtering
    const filter = {};
    if (req.query.role) filter.role = req.query.role;
    if (req.query.active) filter.active = req.query.active === "true";

    // Sorting
    const sort = {};
    if (req.query.sort) {
        const [field, order] = req.query.sort.split(":");
        sort[field] = order === "desc" ? -1 : 1;
    }

    const [users, total] = await Promise.all([
        User.find(filter).sort(sort).skip(skip).limit(limit).lean(),
        User.countDocuments(filter)
    ]);

    res.json({
        data: users,
        meta: {
            total,
            page,
            limit,
            totalPages: Math.ceil(total / limit),
            hasNextPage: page * limit < total,
            hasPrevPage: page > 1
        }
    });
}));
```

---

# PART 3: DATABASE PATTERNS

---

## 3.1 Connection Pooling

### What is it?

A connection pool maintains a set of pre-established database connections that can be reused. Creating a new DB connection is expensive (TCP handshake, auth) — pooling amortizes that cost.

```javascript
// PostgreSQL with pg-pool
const { Pool } = require("pg");

const pool = new Pool({
    host: process.env.DB_HOST,
    port: 5432,
    database: process.env.DB_NAME,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    max: 20,           // Max connections in pool
    idleTimeoutMillis: 30_000,   // Close idle connections after 30s
    connectionTimeoutMillis: 2_000,  // Wait max 2s for a connection
});

// Always use pool.query() for simple queries
async function getUser(id) {
    const { rows } = await pool.query(
        "SELECT * FROM users WHERE id = $1",
        [id]
    );
    return rows[0];
}

// Use pool.connect() for transactions (need same connection)
async function transferFunds(fromId, toId, amount) {
    const client = await pool.connect();
    try {
        await client.query("BEGIN");
        await client.query(
            "UPDATE accounts SET balance = balance - $1 WHERE id = $2",
            [amount, fromId]
        );
        await client.query(
            "UPDATE accounts SET balance = balance + $1 WHERE id = $2",
            [amount, toId]
        );
        await client.query("COMMIT");
    } catch (err) {
        await client.query("ROLLBACK");
        throw err;
    } finally {
        client.release();  // ALWAYS release back to pool
    }
}
```

---

## 3.2 Redis for Caching and Sessions

```javascript
const redis = require("ioredis");

const client = new redis({
    host: process.env.REDIS_HOST,
    port: 6379,
    retryStrategy: times => Math.min(times * 50, 2000)  // Retry with backoff
});

// Cache-aside pattern
async function getUser(id) {
    const cacheKey = `user:${id}`;

    // 1. Check cache
    const cached = await client.get(cacheKey);
    if (cached) return JSON.parse(cached);

    // 2. Cache miss — fetch from DB
    const user = await User.findById(id).lean();
    if (!user) throw new NotFoundError("User");

    // 3. Store in cache with TTL
    await client.setex(cacheKey, 3600, JSON.stringify(user));  // Expire in 1 hour

    return user;
}

// Cache invalidation
async function updateUser(id, updates) {
    const user = await User.findByIdAndUpdate(id, updates, { new: true });
    await client.del(`user:${id}`);  // Invalidate cache
    return user;
}

// Redis for rate limiting (atomic, works across multiple server instances)
async function rateLimitCheck(ip) {
    const key = `rate:${ip}:${Math.floor(Date.now() / 60_000)}`;  // Per minute
    const count = await client.incr(key);

    if (count === 1) {
        await client.expire(key, 60);  // Set TTL on first request
    }

    return count <= 100;  // Allow up to 100 requests per minute
}

// Pub/Sub for real-time events
const publisher = new redis();
const subscriber = new redis();

subscriber.subscribe("user:events");
subscriber.on("message", (channel, message) => {
    const event = JSON.parse(message);
    console.log(`Event on ${channel}:`, event);
});

// In another service:
publisher.publish("user:events", JSON.stringify({
    type: "USER_UPDATED",
    userId: 123,
    timestamp: Date.now()
}));
```

---

# PART 4: SECURITY

---

## 4.1 Authentication with JWT

```javascript
const jwt = require("jsonwebtoken");
const bcrypt = require("bcrypt");

// Password hashing — NEVER store plaintext
async function hashPassword(password) {
    return bcrypt.hash(password, 12);  // 12 rounds is a good balance
}

async function verifyPassword(password, hash) {
    return bcrypt.compare(password, hash);
}

// JWT — access token (short-lived) + refresh token (long-lived)
function generateTokens(userId, role) {
    const accessToken = jwt.sign(
        { userId, role },
        process.env.JWT_SECRET,
        { expiresIn: "15m" }  // Short-lived — if stolen, expires quickly
    );

    const refreshToken = jwt.sign(
        { userId },
        process.env.JWT_REFRESH_SECRET,
        { expiresIn: "7d" }
    );

    return { accessToken, refreshToken };
}

function verifyAccessToken(token) {
    return jwt.verify(token, process.env.JWT_SECRET);
}

// Auth middleware
const authenticate = (req, res, next) => {
    const token = req.headers.authorization?.split(" ")[1];
    if (!token) return res.status(401).json({ error: "No token provided" });

    try {
        const payload = verifyAccessToken(token);
        req.user = payload;
        next();
    } catch (err) {
        if (err.name === "TokenExpiredError") {
            return res.status(401).json({ error: "Token expired", code: "TOKEN_EXPIRED" });
        }
        return res.status(401).json({ error: "Invalid token" });
    }
};

// Refresh token rotation
app.post("/auth/refresh", asyncHandler(async (req, res) => {
    const { refreshToken } = req.body;
    if (!refreshToken) throw new AppError("No refresh token", 401);

    const payload = jwt.verify(refreshToken, process.env.JWT_REFRESH_SECRET);

    // Check if refresh token is in the whitelist (revocation support)
    const isValid = await TokenStore.isValid(refreshToken);
    if (!isValid) throw new AppError("Refresh token revoked", 401);

    // Rotate — invalidate old token, issue new ones
    await TokenStore.revoke(refreshToken);
    const tokens = generateTokens(payload.userId, payload.role);
    await TokenStore.store(tokens.refreshToken, payload.userId);

    res.json(tokens);
}));
```

---

## 4.2 Security Best Practices

```javascript
const helmet = require("helmet");
const rateLimit = require("express-rate-limit");
const mongoSanitize = require("express-mongo-sanitize");
const xss = require("xss-clean");

// Helmet — sets security HTTP headers
app.use(helmet());
// Sets: X-Content-Type-Options, X-Frame-Options, HSTS, CSP, etc.

// Rate limiting
app.use("/api/auth", rateLimit({
    windowMs: 15 * 60 * 1000,  // 15 minutes
    max: 10,                    // Max 10 attempts per window
    message: "Too many auth attempts"
}));

// Sanitize against NoSQL injection
// Without: { "$gt": "" } could bypass password check
app.use(mongoSanitize());  // Removes $ and . from req.body, req.query

// XSS protection — clean HTML input
app.use(xss());

// CORS — restrict which origins can call your API
app.use(cors({
    origin: process.env.ALLOWED_ORIGINS?.split(","),
    methods: ["GET", "POST", "PUT", "PATCH", "DELETE"],
    allowedHeaders: ["Content-Type", "Authorization"],
    credentials: true
}));

// SQL injection prevention — use parameterized queries
// WRONG:
const badQuery = `SELECT * FROM users WHERE email = '${email}'`;  // Injection!

// RIGHT:
const goodQuery = await db.query("SELECT * FROM users WHERE email = $1", [email]);

// HTTPS redirect
if (process.env.NODE_ENV === "production") {
    app.use((req, res, next) => {
        if (req.header("x-forwarded-proto") !== "https") {
            return res.redirect(301, `https://${req.host}${req.url}`);
        }
        next();
    });
}
```

---

# PART 5: PERFORMANCE AND SCALABILITY

---

## 5.1 Clustering — Using All CPU Cores

### What is it?

Node.js is single-threaded. A server with 8 cores runs Node on ONE core by default. Clustering spawns multiple worker processes that all listen on the same port, using all CPU cores.

```javascript
const cluster = require("cluster");
const os = require("os");
const http = require("http");

if (cluster.isPrimary) {
    const numCPUs = os.cpus().length;
    console.log(`Primary ${process.pid} starting ${numCPUs} workers`);

    // Fork workers
    for (let i = 0; i < numCPUs; i++) {
        cluster.fork();
    }

    // Restart crashed workers
    cluster.on("exit", (worker, code, signal) => {
        console.log(`Worker ${worker.process.pid} died. Restarting...`);
        cluster.fork();
    });

    // Zero-downtime restart for all workers
    function gracefulRestart() {
        const workers = Object.values(cluster.workers);
        let i = 0;

        function restartNext() {
            if (i >= workers.length) return;
            const worker = workers[i++];
            worker.send("shutdown");
            worker.on("exit", () => {
                cluster.fork();
                setTimeout(restartNext, 1000);  // Wait for new worker to start
            });
        }
        restartNext();
    }

} else {
    // Worker process
    const server = http.createServer((req, res) => {
        res.writeHead(200);
        res.end(`Hello from worker ${process.pid}`);
    });

    server.listen(3000);
    console.log(`Worker ${process.pid} started`);

    // Graceful shutdown
    process.on("message", (msg) => {
        if (msg === "shutdown") {
            server.close(() => {
                process.exit(0);
            });
        }
    });
}
```

---

## 5.2 Caching Strategies

```javascript
// In-memory cache with LRU eviction
const LRU = require("lru-cache");

const cache = new LRU({
    max: 500,              // Max 500 items
    ttl: 1000 * 60 * 5,   // 5 minutes TTL
    updateAgeOnGet: true   // Reset TTL on access
});

// HTTP response caching with ETags
app.get("/api/users/:id", async (req, res) => {
    const user = await getUser(req.params.id);
    const etag = `"${createHash("md5").update(JSON.stringify(user)).digest("hex")}"`;

    // Client already has this version
    if (req.headers["if-none-match"] === etag) {
        return res.status(304).end();
    }

    res.setHeader("ETag", etag);
    res.setHeader("Cache-Control", "private, max-age=60");
    res.json(user);
});

// Stale-while-revalidate pattern
class StaleWhileRevalidateCache {
    constructor(ttl, staleTtl) {
        this.ttl = ttl;            // Fresh for 1 minute
        this.staleTtl = staleTtl;  // Serve stale for 5 minutes while revalidating
        this.store = new Map();
    }

    async get(key, fetcher) {
        const entry = this.store.get(key);
        const now = Date.now();

        if (!entry || now > entry.staleAt) {
            // No data or too stale — must fetch
            const value = await fetcher();
            this.store.set(key, {
                value,
                freshUntil: now + this.ttl,
                staleAt: now + this.staleTtl
            });
            return value;
        }

        if (now > entry.freshUntil) {
            // Stale — serve old data but refresh in background
            fetcher().then(value => {
                this.store.set(key, {
                    value,
                    freshUntil: Date.now() + this.ttl,
                    staleAt: Date.now() + this.staleTtl
                });
            }).catch(console.error);
        }

        return entry.value;  // Serve (possibly stale) immediately
    }
}
```

---

## 5.3 Message Queues

### What is it?

Message queues (RabbitMQ, Kafka, BullMQ) decouple services. Instead of directly calling another service, you push a message to a queue and a worker picks it up asynchronously.

```javascript
const Bull = require("bull");
const redis = require("ioredis");

// Create a queue
const emailQueue = new Bull("email", {
    redis: { host: "localhost", port: 6379 },
    defaultJobOptions: {
        attempts: 3,           // Retry 3 times
        backoff: {
            type: "exponential",
            delay: 1000        // 1s, 2s, 4s
        },
        removeOnComplete: 100, // Keep last 100 completed jobs
        removeOnFail: 200      // Keep last 200 failed jobs
    }
});

// Producer — add jobs to queue
app.post("/users", asyncHandler(async (req, res) => {
    const user = await User.create(req.body);

    // Don't wait for email — add to queue and return immediately
    await emailQueue.add("welcome", {
        to: user.email,
        name: user.name,
        userId: user.id
    }, { priority: 1 });

    res.status(201).json(user);  // Response in ~5ms instead of ~500ms
}));

// Consumer — worker that processes jobs
emailQueue.process("welcome", async (job) => {
    const { to, name, userId } = job.data;

    await sendEmail({
        to,
        subject: "Welcome!",
        body: `Hi ${name}, welcome to our platform!`
    });

    return { sent: true, userId };
});

// Event listeners
emailQueue.on("completed", (job, result) => {
    console.log(`Job ${job.id} completed:`, result);
});

emailQueue.on("failed", (job, error) => {
    console.error(`Job ${job.id} failed:`, error.message);
});

// Scheduled jobs (like cron)
emailQueue.add("digest", {}, {
    repeat: { cron: "0 9 * * 1" }  // Every Monday at 9am
});
```

---

# PART 6: TESTING NODE.JS

---

## 6.1 Unit and Integration Testing

```javascript
// userService.test.js
const { describe, it, expect, beforeEach, afterEach, jest } = require("@jest/globals");
const { UserService } = require("./userService");

describe("UserService", () => {
    let userService;
    let mockUserRepository;
    let mockEmailService;

    beforeEach(() => {
        // Mock dependencies
        mockUserRepository = {
            findById: jest.fn(),
            create: jest.fn(),
            update: jest.fn(),
            delete: jest.fn()
        };
        mockEmailService = {
            sendWelcome: jest.fn().mockResolvedValue(true)
        };

        userService = new UserService(mockUserRepository, mockEmailService);
    });

    describe("createUser", () => {
        it("creates user and sends welcome email", async () => {
            const userData = { name: "Alice", email: "alice@example.com" };
            const createdUser = { id: 1, ...userData };

            mockUserRepository.create.mockResolvedValue(createdUser);

            const result = await userService.createUser(userData);

            expect(mockUserRepository.create).toHaveBeenCalledWith(userData);
            expect(mockEmailService.sendWelcome).toHaveBeenCalledWith(createdUser.email);
            expect(result).toEqual(createdUser);
        });

        it("throws ValidationError for duplicate email", async () => {
            const userData = { name: "Alice", email: "alice@example.com" };
            mockUserRepository.create.mockRejectedValue({ code: 11000 }); // MongoDB duplicate

            await expect(userService.createUser(userData))
                .rejects.toThrow("Email already exists");
        });
    });
});

// Integration test with supertest
const request = require("supertest");
const app = require("../app");

describe("POST /api/users", () => {
    it("creates a user and returns 201", async () => {
        const response = await request(app)
            .post("/api/users")
            .send({ name: "Bob", email: "bob@example.com", password: "secure123" });

        expect(response.status).toBe(201);
        expect(response.body.data.email).toBe("bob@example.com");
        expect(response.body.data.password).toBeUndefined();  // Never return passwords
    });

    it("returns 400 for invalid email", async () => {
        const response = await request(app)
            .post("/api/users")
            .send({ name: "Bob", email: "not-an-email", password: "secure123" });

        expect(response.status).toBe(400);
        expect(response.body.code).toBe("VALIDATION_ERROR");
    });
});
```

---

# PART 7: DEPLOYMENT AND PRODUCTION

---

## 7.1 Graceful Shutdown

```javascript
const http = require("http");
const app = require("./app");

const server = http.createServer(app);
let isShuttingDown = false;

server.listen(3000, () => {
    console.log("Server running on port 3000");
});

async function gracefulShutdown(signal) {
    if (isShuttingDown) return;
    isShuttingDown = true;
    console.log(`Received ${signal}. Starting graceful shutdown...`);

    // Stop accepting new connections
    server.close(async () => {
        try {
            // Close database connections
            await db.disconnect();
            // Close Redis connection
            await redisClient.quit();
            // Close message queue
            await emailQueue.close();

            console.log("All connections closed. Exiting.");
            process.exit(0);
        } catch (err) {
            console.error("Error during shutdown:", err);
            process.exit(1);
        }
    });

    // Force exit if graceful shutdown takes too long
    setTimeout(() => {
        console.error("Graceful shutdown timed out, forcing exit");
        process.exit(1);
    }, 30_000);
}

// Health check middleware — used by load balancers
app.get("/health", (req, res) => {
    if (isShuttingDown) {
        return res.status(503).json({ status: "shutting_down" });
    }
    res.json({ status: "ok", pid: process.pid, uptime: process.uptime() });
});

process.on("SIGTERM", () => gracefulShutdown("SIGTERM"));
process.on("SIGINT",  () => gracefulShutdown("SIGINT"));
```

---

## 7.2 Environment Configuration

```javascript
// config.js — centralized, validated configuration
const Joi = require("joi");

const schema = Joi.object({
    NODE_ENV: Joi.string().valid("development", "test", "production").required(),
    PORT: Joi.number().default(3000),
    DB_URI: Joi.string().uri().required(),
    JWT_SECRET: Joi.string().min(32).required(),
    REDIS_URL: Joi.string().uri().required(),
    LOG_LEVEL: Joi.string().valid("error", "warn", "info", "debug").default("info"),
}).unknown();

const { error, value: env } = schema.validate(process.env);

if (error) {
    throw new Error(`Config validation error: ${error.message}`);
}

module.exports = {
    env: env.NODE_ENV,
    port: env.PORT,
    db: { uri: env.DB_URI },
    jwt: { secret: env.JWT_SECRET },
    redis: { url: env.REDIS_URL },
    log: { level: env.LOG_LEVEL }
};
```

---

# INTERVIEW QUESTIONS AND ANSWERS

## Node.js Interview Q&A

**Q1: Explain the Node.js Event Loop and its phases.**
**A**: The event loop is what allows Node to handle asynchronous operations with a single thread. It runs through phases in order: **timers** (setTimeout/setInterval callbacks), **pending callbacks** (deferred I/O callbacks), **poll** (retrieves new I/O events — where Node spends most time), **check** (setImmediate), **close callbacks**. Between phases, all microtasks run: `process.nextTick()` first, then Promise callbacks.

---

**Q2: What is the difference between `process.nextTick()`, `setImmediate()`, and `setTimeout(fn, 0)`?**
**A**: `process.nextTick()` runs before any I/O, before Promises, at the end of the current operation — it can starve the event loop if used recursively. `Promise.then()` runs after nextTick, also a microtask. `setImmediate()` runs in the check phase, after I/O. `setTimeout(fn, 0)` runs in the timers phase. Inside an I/O callback, `setImmediate` always runs before `setTimeout(fn, 0)`. Outside I/O, their order is non-deterministic.

---

**Q3: What is back-pressure in streams and how do you handle it?**
**A**: Back-pressure occurs when the writable stream can't consume data as fast as the readable produces it. Without handling it, you buffer unbounded data in memory. Node.js stream `.pipe()` handles back-pressure automatically — it pauses the readable when the writable's buffer is full and resumes when it drains. When using `stream.write()` manually, check its return value: `false` means stop writing, listen for the `drain` event to resume.

---

**Q4: What is the difference between `cluster` and `worker_threads`?**
**A**: `cluster` creates separate Node.js processes (separate V8 instances, separate memory) that share a server port — used for scaling HTTP servers across CPU cores. `worker_threads` creates threads within the same process — they share memory via `SharedArrayBuffer` and communicate via message passing. Use `cluster` for horizontal HTTP scaling; use `worker_threads` for CPU-intensive tasks (image processing, computation) without spawning a full process.

---

**Q5: How would you prevent a slow database query from blocking other requests?**
**A**: Node is non-blocking for I/O by default — a slow DB query doesn't block the event loop because the query is delegated to libuv's thread pool (or the DB driver's async interface). The event loop continues handling other requests while waiting for the DB response. However, if you're doing heavy **CPU computation** (not I/O) based on the query result, that would block. Solutions: move computation to a worker thread, use streaming for large result sets, add appropriate indexes to the database, or add a caching layer (Redis).

---

**Q6: How do you handle unhandled Promise rejections in Node.js?**
**A**: Three approaches: (1) Always `.catch()` Promise chains. (2) Use `async/await` with `try/catch`. (3) Globally: `process.on("unhandledRejection", (reason, promise) => { ... })`. In Node.js 15+, unhandled rejections crash the process by default (correct behavior in production). Use the global handler to log the error and trigger a graceful shutdown. Never silently swallow errors.

---

**Q7: What is connection pooling and why is it important in Node.js?**
**A**: Connection pooling maintains a set of reusable database connections. Creating a new DB connection is expensive — it involves a TCP handshake, authentication, and memory allocation. In Node.js, because requests are handled concurrently, you could have thousands of simultaneous DB queries. A pool with `max: 20` means at most 20 connections — other requests queue and reuse connections as they're released. Without pooling, you'd either create thousands of connections (crashing the DB) or create a new one per request (severe latency).

---

**Q8: Explain the difference between operational errors and programmer errors.**
**A**: **Operational errors** are expected runtime errors: network failures, DB down, file not found, validation errors, rate limits. These should be handled gracefully and communicated to the client. **Programmer errors** are bugs: null pointer dereference, calling a function with wrong args, off-by-one. These should crash the process (because the app is in an unknown state) and be caught by monitoring. The pattern: `if (err.isOperational) handleGracefully(err); else crashAndRestart(err)`.

---

**Q9: What is the purpose of `helmet` middleware and what does it do?**
**A**: Helmet sets security-related HTTP headers to protect against common attacks. It sets: `X-Content-Type-Options: nosniff` (prevents MIME sniffing), `X-Frame-Options: DENY` (prevents clickjacking), `Strict-Transport-Security` (enforces HTTPS), `X-XSS-Protection`, Content Security Policy (prevents XSS), `Referrer-Policy`, and removes the `X-Powered-By: Express` header. It's a must-have security baseline for any production Express app.

---

**Q10: How would you implement rate limiting that works across multiple server instances?**
**A**: In-memory rate limiting (using a Map) only works per-process — different server instances have different counters. For multi-instance rate limiting, use a shared store: Redis. Use Redis `INCR` + `EXPIRE` commands atomically: on each request, increment a key like `rate:{ip}:{minute}`. If the count exceeds the limit, reject. Since `INCR` is atomic in Redis, multiple server instances can safely share the same counter. Libraries like `rate-limiter-flexible` implement this pattern out of the box.