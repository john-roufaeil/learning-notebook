# Module 05 — Frameworks In Depth: What They Add and How They Work

> **Series:** From Junior to Senior — Languages, Runtimes & Frameworks Explained  
> **Level:** Intermediate → Advanced  
> **Goal:** Understand what a framework actually IS, how each major framework works, and what they trade off.

---

## What Is a Framework, Really?

A language gives you the ability to write logic. A **framework** is a pre-built skeleton that handles the common, repetitive infrastructure of a category of application — so you only write the parts that are unique to your product.

For a web backend, this common infrastructure includes:
- Routing: mapping URLs to code that handles them
- Parsing: turning HTTP request bytes into structured data
- Middleware: running common code on every request (authentication, logging, rate limiting)
- Validation: checking that incoming data is well-formed
- Serialization: converting your objects to JSON to send back
- Database access: abstracting SQL or connecting to ORMs
- Error handling: catching exceptions and sending appropriate responses

Without a framework, you write all of this yourself — every time, for every project. Frameworks encode the industry's accumulated knowledge about how to build these things well.

**The trade-off space:**

```
Opinionated (lots of decisions made for you)    Minimal (you choose everything)
      │                                               │
   Rails                                            Flask
   Laravel                                          Gin
   Django                                           Express
   Spring Boot                                      Actix-web
   NestJS (opinionated end of Node.js)
```

Opinionated frameworks let you build faster but constrain your architecture. Minimal frameworks give freedom but require more decisions (and mistakes). Neither is "better" — the choice depends on team size, domain complexity, and how much you want the framework to guide you.

---

## How Frameworks Handle a Request: The Pipeline Concept

Every web framework, regardless of language, processes a request as a **pipeline**. The request passes through a series of steps (middleware), reaches the handler, and the response passes back through:

```
HTTP Request arrives
        │
        ▼
[Middleware 1: Parse body]
        │
        ▼
[Middleware 2: Authentication]
        │
        ▼
[Middleware 3: Rate limiting]
        │
        ▼
[Middleware 4: Logging]
        │
        ▼
[Your Route Handler: business logic]
        │
        ▼
[Response transformation]
        │
        ▼
HTTP Response sent
```

Each framework implements this pipeline differently — some use explicit middleware chains, some use decorators, some use aspect-oriented programming. But the concept is universal.

---

## Node.js / Express

### What it is
Express is the minimal, unopinionated Node.js web framework. It's essentially a thin routing layer on top of Node's HTTP server.

### How it works
Express adds a **middleware chain** to Node.js's event loop. When a request comes in:
1. Node.js's event loop receives the request
2. Express routes it to the matching middleware stack
3. Middleware functions run in order, each calling `next()` to pass to the next
4. The route handler generates the response
5. Response flows back up the chain

```javascript
const express = require('express');
const app = express();

// Middleware applied to every request
app.use(express.json());           // parses request body
app.use(authMiddleware);           // checks JWT token
app.use(requestLogger);            // logs every request

// Route-specific middleware + handler
app.get('/users/:id', 
    validateIdParam,               // validate :id is a number
    async (req, res) => {
        const user = await db.findUser(req.params.id);
        res.json(user);
    }
);
```

### What's good
- **Simplicity** — the smallest possible API surface
- **Ecosystem** — tens of thousands of npm middleware packages
- **Flexibility** — no opinions on structure, databases, or patterns
- **Familiarity** — huge developer pool

### What's missing / Watch out for
- **No structure enforced** — large Express apps become disorganized quickly
- **No built-in validation** — you add libraries manually
- **No dependency injection** — hard to test without disciplined patterns
- **Single-threaded** — CPU-bound work in handlers stalls all requests
- **Callback/async bugs** — uncaught promise rejections used to crash processes silently

---

## NestJS

### What it is
NestJS is an opinionated, TypeScript-first Node.js framework heavily inspired by Angular. It adds structure, dependency injection, decorators, and conventions on top of Express (or Fastify).

### How it works
NestJS uses **TypeScript decorators** to declare controllers, services, and modules. It has an IoC container (Inversion of Control) that handles dependency injection:

```typescript
@Injectable()
export class UserService {
    constructor(private readonly db: DatabaseService) {}
    
    async findUser(id: number): Promise<User> {
        return this.db.users.findUnique({ where: { id } });
    }
}

@Controller('users')
export class UserController {
    constructor(private readonly userService: UserService) {}
    // NestJS injects UserService automatically ↑
    
    @Get(':id')
    @UseGuards(AuthGuard)           // middleware applied via decorator
    async getUser(@Param('id') id: string): Promise<UserDto> {
        return this.userService.findUser(parseInt(id));
    }
}
```

NestJS's request lifecycle:
1. Incoming request
2. **Guards** — authentication/authorization (can you do this?)
3. **Interceptors** (before) — logging, caching, response transformation
4. **Pipes** — input validation and transformation
5. **Controller** method executes
6. **Interceptors** (after) — response manipulation
7. **Exception filters** — catch and format errors

### What's good
- **Structure** — organized by modules, controllers, services
- **TypeScript-first** — full type safety
- **Dependency injection** — testable, decoupled code
- **Ecosystem** — official modules for databases, auth, config, websockets, queues
- **Scales well** — same patterns whether you have 10 or 100 controllers
- **Familiar to Angular developers** — same architecture

### Watch out for
- **Overhead** — more boilerplate than Express; overkill for tiny APIs
- **Decorator magic** — lots of "magic" from decorators; debugging can be confusing
- **Still Node.js underneath** — CPU-bound limitations apply

---

## Next.js

### What it is
Next.js is a full-stack React framework. It's technically both a frontend and backend framework — it renders React components on the server (SSR/SSG), handles routing, and provides API routes.

### How it works
Next.js has several rendering modes that are fundamental to understand:

**Static Site Generation (SSG):** Pages are generated at build time. When a user requests the page, they receive pre-built HTML. Fastest possible page load, but content can be stale.

**Server-Side Rendering (SSR):** Pages are rendered on the server on each request. Fresh content, but slower than static — the server must run React rendering for each request.

**Client-Side Rendering (CSR):** HTML shell served, React renders in the browser, data fetched from APIs. The traditional SPA approach.

**Incremental Static Regeneration (ISR):** Static pages that automatically rebuild in the background after a time interval.

```javascript
// API Route (acts like Express route)
// /pages/api/users/[id].js
export default async function handler(req, res) {
    if (req.method === 'GET') {
        const user = await db.findUser(req.query.id);
        res.json(user);
    }
}

// Server component (React component that runs on the server)
// /app/users/[id]/page.jsx
export default async function UserPage({ params }) {
    const user = await fetch(`/api/users/${params.id}`).then(r => r.json());
    // This entire component runs on the server, sends HTML to client
    return <div>{user.name}</div>;
}
```

### What's good
- **Full-stack in one codebase** — frontend and backend unified
- **Multiple rendering strategies** — right tool for each page
- **Automatic code splitting** — only send JS needed for the current page
- **File-based routing** — directory structure = URL structure
- **Vercel deployment** — seamless deployment, edge functions
- **SEO** — SSR ensures search engines see full HTML

### Watch out for
- **Complexity** — SSR, SSG, ISR, CSR, server components, client components — managing which mode to use is non-trivial
- **Still Node.js** — API routes are Node.js; CPU limitations apply
- **Large bundle sizes** — if not careful, ships too much JavaScript
- **Not a backend framework** — for complex APIs, pair with NestJS or a separate backend

---

## Django

### What it is
Django is a "batteries-included" Python web framework. It comes with everything: ORM, authentication, admin panel, form handling, CSRF protection, and an opinion on project structure. Created for newsroom software — Django was built to ship fast.

### How it works
Django's architecture:

```
URL Configuration
       │ maps URL patterns to view functions/classes
       ▼
Views (your handlers)
       │ calls models, renders templates, returns response
       ▼
Models (Django ORM)
       │ maps Python classes to database tables
       ▼
Templates (HTML generation) or REST framework (JSON)
```

The ORM is central to Django. You define your data as Python classes:

```python
# models.py
class User(models.Model):
    email = models.EmailField(unique=True)
    name = models.CharField(max_length=200)
    created_at = models.DateTimeField(auto_now_add=True)
    is_active = models.BooleanField(default=True)

# Django generates:
# CREATE TABLE myapp_user (
#   id SERIAL PRIMARY KEY,
#   email VARCHAR(254) UNIQUE,
#   name VARCHAR(200),
#   created_at TIMESTAMP DEFAULT NOW(),
#   is_active BOOLEAN DEFAULT TRUE
# );
```

Django generates and manages database migrations automatically. When you change a model, `python manage.py makemigrations` detects the difference and `migrate` applies it.

**Django's middleware pipeline:**

```
SecurityMiddleware
    └── SessionMiddleware
        └── CommonMiddleware
            └── CsrfViewMiddleware
                └── AuthenticationMiddleware
                    └── MessageMiddleware
                        └── Your view function
```

### What's good
- **Speed to build** — admin panel generated from your models in ~5 lines of code
- **ORM** — powerful, handles complex queries, prevents SQL injection
- **Authentication system** — built-in user model, login, logout, permissions
- **Migrations** — schema change management built-in
- **Community** — enormous ecosystem, well-documented, battle-tested

### Watch out for
- **Synchronous by default** — Django's traditional WSGI model handles one request per process/thread
- **Async support is partial** — Django 3.1+ added async views, but many parts of Django (ORM, auth) are still sync
- **ORM N+1 problem** — accidentally querying the DB inside a loop is common
- **Monolithic tendency** — "batteries included" can become batteries-chaining-you

---

## FastAPI

### What it is
FastAPI is a modern Python web framework designed for building APIs. It runs on asyncio (async event loop) and uses Python type hints for automatic request validation and documentation generation.

### How it works
FastAPI uses Python type annotations as its API contract:

```python
from fastapi import FastAPI
from pydantic import BaseModel

app = FastAPI()

class UserCreate(BaseModel):
    email: str
    name: str
    age: int

@app.post("/users")
async def create_user(user: UserCreate):
    # FastAPI automatically:
    # 1. Parses the JSON request body
    # 2. Validates it against UserCreate schema (type-safe)
    # 3. Converts the data to a UserCreate object
    # 4. Returns HTTP 422 if validation fails
    
    new_user = await db.create_user(user.email, user.name, user.age)
    return new_user  # FastAPI serializes this to JSON automatically
```

FastAPI also auto-generates OpenAPI (Swagger) documentation from your type annotations. Visit `/docs` and you get an interactive API explorer.

**Architecture:** FastAPI is built on Starlette (ASGI framework) and Pydantic (data validation). Runs on Uvicorn server with uvloop (a fast asyncio event loop implementation).

### What's good
- **Performance** — fastest Python web framework by a large margin (async + Pydantic v2 uses Rust internals)
- **Automatic validation** — request/response schemas from type hints
- **Auto-documentation** — OpenAPI/Swagger generated automatically
- **Dependency injection** — clean, testable
- **Modern** — async-first, type-first

### Watch out for
- **Async discipline required** — mixing sync and async code causes problems
- **Python ceiling** — still Python; for high-throughput, Go/Java will eventually outperform
- **Less batteries** — no built-in admin, no migration system (use Alembic separately)
- **Pydantic v1 vs v2** — ecosystem is still transitioning; dependency conflicts common

---

## Flask

### What it is
Flask is Python's minimal web framework — the "microframework." It provides routing, request parsing, and response generation. Nothing else. You bring your own ORM, auth, validation, etc.

### How it works
Flask is deliberately small:

```python
from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/users/<int:user_id>', methods=['GET'])
def get_user(user_id):
    user = User.query.get_or_404(user_id)  # SQLAlchemy, added separately
    return jsonify(user.to_dict())
```

Flask is synchronous by default (WSGI). Deployed via Gunicorn (process manager) for production.

### What's good
- **Simplicity** — extremely easy to understand completely
- **Flexibility** — no opinions on database, auth, structure
- **Great for microservices** — a focused service doesn't need Django's bulk
- **Prototyping** — fastest way to get an API endpoint running

### Watch out for
- **Grows disorganized** — large Flask apps without disciplined structure become messy
- **No async by default** — Flask 2.0+ has async views but ecosystem is sync-first
- **You assemble everything** — authentication, validation, migrations all require separate libraries and integration work
- **Slower than FastAPI** — for new async Python APIs, FastAPI is now preferred

---

## Spring Boot

### What it is
Spring Boot is the dominant Java web framework. "Spring" is the underlying framework (IoC container, security, data access, etc.); "Boot" is the auto-configuration layer that makes Spring easy to start. Used at massive scale in banking, e-commerce, and enterprise software worldwide.

### How it works
Spring Boot uses **dependency injection** and **auto-configuration**. You annotate classes to tell Spring what they are:

```java
@Service
public class UserService {
    
    @Autowired  // Spring injects this dependency automatically
    private UserRepository userRepository;
    
    public User findUser(Long id) {
        return userRepository.findById(id)
            .orElseThrow(() -> new UserNotFoundException(id));
    }
}

@RestController
@RequestMapping("/api/users")
public class UserController {
    
    @Autowired
    private UserService userService;
    
    @GetMapping("/{id}")
    public ResponseEntity<UserDto> getUser(@PathVariable Long id) {
        User user = userService.findUser(id);
        return ResponseEntity.ok(UserMapper.toDto(user));
    }
}
```

Spring Boot's application startup:
1. Loads Spring ApplicationContext (IoC container)
2. Scans for `@Component`, `@Service`, `@Repository`, `@Controller` annotations
3. Wires dependencies (auto-wiring via `@Autowired`)
4. Configures auto-detected beans (database connection, web server, etc.)
5. Starts Tomcat (or Netty/Undertow) embedded server
6. Ready to accept requests

This startup process takes 5–30 seconds for large applications.

### Request lifecycle
```
HTTP request →
Tomcat/Netty receives →
DispatcherServlet →
Handler Mapping (finds @Controller method) →
Argument resolvers (parse @RequestBody, @PathVariable, etc.) →
Your @RequestMapping method executes →
Response body converted to JSON (Jackson) →
HTTP response sent
```

### What's good
- **Mature ecosystem** — Spring Security, Spring Data, Spring Cloud — decades of hardened code
- **Enterprise-grade** — handles complex authorization, transaction management, event-driven architecture
- **Auto-configuration** — add a library to classpath, Spring configures it automatically
- **Extensive support** — huge community, extensive documentation
- **Spring Data** — incredibly powerful repository pattern for database access (generates queries from method names: `findByEmailAndActiveTrue()`)

### Watch out for
- **Startup time** — the bane of Spring Boot. 5–30 seconds is normal. GraalVM Native Image addresses this.
- **Memory footprint** — JVM + Spring context = 200–500MB minimum
- **"Magic" can confuse** — auto-configuration is powerful but debugging why something auto-configured incorrectly is painful
- **Bean scope bugs** — injecting a request-scoped bean into a singleton is a classic mistake

---

## Ruby on Rails

### What it is
Rails is the framework that showed the world what "convention over configuration" could look like. Created by DHH in 2004, Rails made CRUD web apps dramatically faster to build and influenced every major web framework that followed.

### How it works
Rails is built on the MVC (Model-View-Controller) pattern and the principle that if you follow conventions, Rails does the heavy lifting:

```ruby
# Convention: a "posts" table → Post model
class Post < ApplicationRecord
  belongs_to :user
  has_many :comments, dependent: :destroy
  validates :title, presence: true, length: { maximum: 200 }
  
  scope :published, -> { where(published: true) }
end

# Convention: PostsController handles /posts routes
class PostsController < ApplicationController
  before_action :authenticate_user!   # Devise authentication
  before_action :set_post, only: [:show, :edit, :update, :destroy]
  
  def index
    @posts = Post.published.includes(:user).page(params[:page])
  end
  
  def create
    @post = current_user.posts.build(post_params)
    if @post.save
      redirect_to @post, notice: 'Post created!'
    else
      render :new
    end
  end
end
```

`rails generate scaffold Post title:string body:text published:boolean` creates the model, migration, controller, views, tests, and routes — a complete CRUD interface — in one command.

**ActiveRecord** (Rails' ORM) is one of the most influential pieces of software in web development. The pattern of data objects knowing how to query and persist themselves is now seen in Django's ORM, Eloquent (Laravel), Doctrine (Symfony), and others.

### What's good
- **Speed to ship** — for CRUD applications, nothing comes close
- **Convention** — less decision fatigue; everyone knows where everything is
- **ActiveRecord** — powerful associations, scopes, validations
- **Gems ecosystem** — Devise (auth), Pundit (authorization), Sidekiq (jobs), Paperclip (files)
- **Testing culture** — Rails community has the strongest testing tradition in web development
- **GitHub, Shopify, Basecamp built on Rails** — proven at significant scale

### Watch out for
- **"Magic"** — Rails does a lot automatically; beginners don't understand what's happening
- **Memory** — each Rails process uses significant RAM; horizontal scaling needed at moderate traffic
- **Performance ceiling** — not the fastest; requires more servers than Go/Node at equivalent load
- **Dynamic typing** — large Rails codebases can be hard to refactor safely
- **Learning curve** — the "magic" that makes it productive can make debugging opaque for beginners

---

## ASP.NET Core

### What it is
Microsoft's web framework for .NET. ASP.NET Core is cross-platform (Windows, Linux, macOS), high-performance, and modern. It's the successor to the Windows-only ASP.NET (classic). Regularly tops the TechEmpower web benchmarks for throughput.

### How it works
ASP.NET Core uses middleware and attribute-based routing:

```csharp
// Program.cs — minimal hosting model (ASP.NET Core 6+)
var builder = WebApplication.CreateBuilder(args);
builder.Services.AddControllers();
builder.Services.AddDbContext<AppDbContext>(options =>
    options.UseNpgsql(connectionString));

var app = builder.Build();

// Middleware pipeline registration
app.UseExceptionHandler("/error");
app.UseHttpsRedirection();
app.UseAuthentication();
app.UseAuthorization();
app.MapControllers();

app.Run();

// Controller
[ApiController]
[Route("api/[controller]")]
public class UsersController : ControllerBase
{
    private readonly UserService _userService;
    
    public UsersController(UserService userService) // DI constructor injection
        => _userService = userService;
    
    [HttpGet("{id}")]
    [Authorize]
    public async Task<ActionResult<UserDto>> GetUser(int id)
    {
        var user = await _userService.FindAsync(id);
        if (user == null) return NotFound();
        return Ok(user);
    }
}
```

**Kestrel** is ASP.NET Core's built-in HTTP server (replaces IIS for production in many cases). It's one of the fastest HTTP servers in the world — engineered for zero-allocation request processing.

### What's good
- **Performance** — among the fastest web frameworks in benchmarks
- **async/await** — C# async is excellent; entire pipeline can be non-blocking
- **Type safety** — C#'s type system prevents many runtime errors
- **Entity Framework Core** — ORM comparable to Hibernate; also supports raw SQL
- **SignalR** — real-time WebSocket/long-polling support built in
- **Minimal APIs** — new minimal API style is as concise as Express

### Watch out for
- **Windows/Microsoft perception** — teams default to Linux/Node/Python because of historical ASP.NET's Windows requirement; .NET Core is fully cross-platform but the perception lingers
- **Enterprise complexity** — large ASP.NET Core apps can be verbose
- **Smaller community** than Spring or Node for web APIs

---

## Gin (Go)

### What it is
Gin is a minimal, high-performance Go web framework. It's essentially a fast router and middleware chain built on Go's `net/http` standard library.

### How it works
```go
package main

import (
    "net/http"
    "github.com/gin-gonic/gin"
)

func main() {
    r := gin.New()
    
    // Middleware
    r.Use(gin.Recovery())       // recover from panics
    r.Use(AuthMiddleware())     // custom auth
    
    // Routes
    api := r.Group("/api")
    {
        api.GET("/users/:id", getUser)
        api.POST("/users", createUser)
    }
    
    r.Run(":8080")
}

func getUser(c *gin.Context) {
    id := c.Param("id")
    var user User
    if err := db.First(&user, id).Error; err != nil {
        c.JSON(http.StatusNotFound, gin.H{"error": "user not found"})
        return
    }
    c.JSON(http.StatusOK, user)
}
```

Every request gets a goroutine. Gin itself does very little — it routes the request to your handler. The Go runtime handles the concurrency.

### What's good
- **Performance** — one of the fastest web frameworks in existence
- **Low memory** — Go binary, minimal RAM overhead
- **Single binary deployment** — `go build` → deploy one file
- **Explicit** — no magic; easy to trace exactly what happens on a request
- **Goroutines handle concurrency** — no thread pool tuning needed

### Watch out for
- **Minimal** — you bring your own ORM (GORM is popular), auth, validation
- **Less ecosystem** than Spring/Rails/Django for complex enterprise needs
- **Go's verbosity** — error handling is explicit (`if err != nil`) which some find repetitive
- **Fewer "batteries"** — building a full product requires assembling more pieces

---

## Laravel

### What it is
Laravel is the dominant PHP framework. Often called "PHP Rails" — it brings the same convention-over-configuration, MVC, elegant ORM (Eloquent), and developer experience that Rails brought to Ruby, but for PHP.

### How it works
```php
// routes/api.php
Route::middleware('auth:sanctum')->group(function () {
    Route::get('/users/{user}', [UserController::class, 'show']);
    Route::post('/users', [UserController::class, 'store']);
});

// app/Http/Controllers/UserController.php
class UserController extends Controller
{
    public function show(User $user): UserResource // route model binding
    {
        $this->authorize('view', $user);           // Policies
        return new UserResource($user->load('posts'));
    }
    
    public function store(StoreUserRequest $request): UserResource // form request validation
    {
        $user = User::create($request->validated());
        return new UserResource($user);
    }
}

// Eloquent ORM:
$users = User::with('posts')           // eager load (prevents N+1)
              ->where('active', true)
              ->paginate(20);
```

Laravel's ecosystem: **Forge** (server provisioning), **Vapor** (serverless Laravel on AWS), **Nova** (admin panel), **Livewire** (reactive UIs without JavaScript framework), **Sanctum** (API authentication), **Horizon** (queue monitoring), **Telescope** (debugging).

### What's good
- **Best-in-class developer experience for PHP** — elegant syntax, comprehensive documentation
- **Eloquent** — arguably the most pleasant ORM to use day-to-day
- **Artisan CLI** — powerful code generation, migration, and utility commands
- **Queue system** — built-in job queue with multiple drivers (Redis, database, SQS)
- **The PHP ecosystem** — easy deployment, widely supported hosting
- **Livewire/Inertia.js** — modern reactive UIs without going full SPA

### Watch out for
- **PHP-FPM ceiling** — each request is a new process; stateful features (WebSockets, long connections) need Reverb/Pusher
- **Performance** — slower than Go/Java/Node for high-throughput APIs
- **"Magic"** — Laravel's magic (route model binding, facades) confuses beginners
- **PHP's reputation** — unfair in 2024, but real hiring perception challenges

---

## Framework DNA: What They Have in Common

Despite looking different, all these frameworks implement the same core concepts:

| Concept | Express | Django | Spring | Rails | Laravel | FastAPI | Gin |
|---|---|---|---|---|---|---|---|
| **Routing** | app.get() | urls.py | @GetMapping | routes.rb | Route::get() | @app.get() | r.GET() |
| **Middleware** | app.use() | MIDDLEWARE list | Filter/Interceptor | before_action | Middleware class | Depends() | r.Use() |
| **Validation** | manual/Joi | forms/serializers | Bean Validation | validates | Form Requests | Pydantic | manual/binding |
| **Auth** | Passport.js | django.contrib.auth | Spring Security | Devise | Sanctum | OAuth/JWT libs | custom |
| **ORM/DB** | Prisma/TypeORM | Django ORM | Spring Data/JPA | ActiveRecord | Eloquent | SQLAlchemy | GORM |
| **DI Container** | manual | manual | Spring IoC | N/A | Laravel IoC | FastAPI Depends | N/A |

---

## The Request Lifecycle Compared

Let's trace what happens when a request hits each framework for a GET /users/42 endpoint:

```
Express (Node.js):
  libuv receives HTTP bytes →
  HTTP parsed →
  Route matched (O(n) middleware scan) →
  Middleware chain runs →
  Handler executes (async, non-blocking) →
  Response serialized to JSON →
  libuv sends response bytes

Django (Python, sync):
  Gunicorn worker receives request →
  WSGI environ dict created →
  Middleware stack (top-down) →
  URL resolver finds view →
  View function executes (synchronous, blocking) →
  ORM queries database (synchronous, blocking) →
  Response object → JSON serialized →
  Gunicorn sends response

Spring Boot (Java):
  Tomcat thread receives request →
  HTTP parsed →
  DispatcherServlet routes →
  Handler interceptors run →
  Controller method invoked →
  Service + Repository layers execute →
  Jackson serializes response to JSON →
  Tomcat sends response

Gin (Go):
  net/http goroutine receives request →
  HTTP parsed →
  Radix tree router matches route in O(path length) →
  Middleware chain runs →
  Handler executes (goroutine, non-blocking I/O) →
  encoding/json serializes →
  Response sent

Rails (Ruby):
  Puma thread receives request →
  Rack middleware stack →
  Router matches →
  Before actions run →
  Controller action executes →
  ActiveRecord queries →
  View renders or serializer runs →
  Response sent
```

The conceptual pipeline is identical. The implementation details — thread model, performance, memory use, ease of debugging — differ enormously.

---

## Summary: Framework Personalities

| Framework | Language | Personality | Best For |
|---|---|---|---|
| **Express** | Node.js | Minimal, DIY | Flexible APIs, microservices |
| **NestJS** | TypeScript | Angular-inspired, structured | Large-scale Node APIs |
| **Next.js** | TypeScript | Full-stack React | Web apps, SSR/SSG |
| **Django** | Python | Batteries-included | Content sites, admin-heavy apps |
| **FastAPI** | Python | Modern, async, type-driven | ML APIs, async Python APIs |
| **Flask** | Python | Micro, flexible | Simple APIs, microservices |
| **Spring Boot** | Java | Enterprise, comprehensive | Large enterprise backends |
| **Rails** | Ruby | Convention magic, CRUD-fast | Rapid product development |
| **ASP.NET Core** | C# | High-performance, Microsoft | .NET ecosystem, enterprise |
| **Gin** | Go | Minimal, fast, explicit | High-performance APIs |
| **Laravel** | PHP | Elegant PHP-Rails | PHP web apps |

---

*Previous: [Module 04 — Languages In Depth](./04_languages_in_depth.md)*  
*Next: [Module 06 — Type Systems, Paradigms & Code Architecture](./06_type_systems_and_paradigms.md)*
