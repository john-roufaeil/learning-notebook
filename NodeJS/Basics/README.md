# NodeJS Day 1

- [Web 101](#web-101)
- [What is NodeJS](#what-is-nodejs)
- [NodeJS vs Browser](#nodejs-vs-browser)
- [ENV, Global, REPL, CLI Args](#env-global-repl-cli-args)
- [Modules & Require](#modules--require)
- [Core Modules](#core-modules)

## Web 101

- Client-server model; browser = client, NodeJS app = server
- HTTP is a request-response protocol
- Request has method, headers, and an optional body
- Response has status code, headers, body
- Application Server receives HTTP request, processes data/communicates with (DBMS/Filesystem/OS/Other sources) and returns fetched data.

### NodeJS main job on the web

- Receive requests
- Process data
- Send responses

### HTTP URL Anatomy

https://www.example.com:3000/path/resource?id=123#section-id

1. Scheme (https://): defines how the resource will be obtained.
2. Subdomain (www.): 
3. Domain (example): unique value within its top level domain
4. Top-level Domain (.com)
5. Port (3000): If omitted HTTP on 80, HTTPS on 443
6. Path (path/resource): specify requested resource
7. Query string (?id=123): data passed to server-side SW
8. Fragment identifier (#section-id): a specific place in HTML document.

### Future search

- HTTP request response lifecycle
- HTTP headers explained
- REST vs RPC (high level only)
- What is a port
- What is localhost
- DNS vs IP (concept only)


## What is NodeJS

Node.js is an open-source, cross-platform, back-end, JavaScript runtime environment that executes JavaScript code outside a web browser. 

- A JavaScript runtime
- Built on V8 (same engine as Chrome)
- Designed for server-side, I/O-heavy, non-blocking work
- Not a framework nor a language and not used only for APIs

### Why NodeJS

Before NodeJS:
- JS ran on browsers only
- Servers used blocking models (thread per request)

NodeJS introduced:
- Event-driven architecture
- Single-threaded + async I/O

Node = JS + V8 + C++ bindings + event loop + system APIs

### Future search

- Why NodeJS was created
- NodeJS architecture overview
- What V8 does (JIT compilation)
- Why Node is fast for I/O, not CPU

## NodeJS vs Browser

Both share:
- JS language
- V8 engine
- Event loop concept

Browser has:
- DOM
- window
- document
- Web APIs (fetch, alert, localStorage, ...)

Node has:
- fs (filesystem)
- http
- process
- os

JavaScript is the same, environment is different.

### Future search

- NodeJS vs Browser APIs
- Why there is no DOM in Node
- Isomorphic / Universal JS (concept only)

## ENV, global, REPL, CLI Args

```global```

Node’s global object (like window in browser)

Contains:
- setTimeout
- Buffer
- process

Why this matters:
Polluting global = bad design

---

```process```

Gives info about the current Node process

Common usage:
- process.env
- process.argv
- process.exit

Why this matters:
This is how Node talks to the OS
 
--- 

Environment variables (process.env)

Used for:
- secrets
- config per environment

Rule:
Code should not change between dev and prod. ENV should.

Search:
"dotenv vs process.env"

---

CLI Arguments (process.argv)

Access arguments passed when running Node

Example mental model:
node app.js --port=3000

Why this matters:
- Build scripts
- Custom CLIs

---

REPL

Interactive Node shell

Used for:
- quick experiments
- testing APIs

Search:
"Node REPL use cases"

## Modules & Require

Problem modules solve:
- Code organization
- Encapsulation
- Reusability

CommonJS (Node default):
- require()
- module.exports

Mental model:
> Each file is its own module with its own scope

Important behavior:
- Modules are cached after first load
- Code runs once per process

Why this matters:
- Singletons
- Shared state bugs

Search for: 
- CommonJS module caching
- module.exports vs exports
- ES Modules (import/export)


## Core Modules

- Built-in Node modules
- No installation needed

Must-know ones:
- fs → files
- path → file paths
- http → servers
- os → system info
- events → event-driven patterns

Why core modules matter:
> Frameworks are built on top of these. If you skip them, debugging becomes guesswork.

Search for:
- Node fs sync vs async
- Node http createServer
- Node event emitter
