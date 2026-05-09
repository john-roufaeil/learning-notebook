# PHP for JavaScript Developers

> A no-fluff syntax reference for JS devs picking up PHP.

---

## 1. The Basics: File & Execution Model

PHP is a **server-side, request-driven** language. Unlike Node.js where you spin up a server manually, PHP files are executed per-request by a web server (Apache/Nginx via PHP-FPM or the built-in dev server).

```
Browser → HTTP Request → Web Server → PHP Interpreter (runs file top-to-bottom) → HTML Response
```

- Every `.php` file runs fresh per request — **no persistent memory between requests** (unless you use sessions, cache, or a DB).
- PHP outputs directly to the HTTP response body. Whatever you `echo`, gets sent to the browser.
- PHP and HTML can be **mixed in the same file** — you open/close PHP tags anywhere.

```php
<html>
  <body>
    <h1><?php echo "Hello World"; ?></h1>
  </body>
</html>
```

---

## 2. PHP Tags

```php
<?php ... ?>        // Standard — always use this
<?= $var ?>         // Short echo tag — equivalent to <?php echo $var ?>
```

---

## 3. Variables

No `let`/`const`/`var`. All variables start with `$`. Dynamically typed.

```php
$name   = "Ali";
$age    = 30;
$price  = 9.99;
$active = true;
$data   = null;
```

**Constants** use `define()` or `const` (no `$`):

```php
define('MAX_SIZE', 100);
const VERSION = "1.0";
```

---

## 4. Strings

```php
$name = "World";

// Double quotes — interpolation works
echo "Hello $name";           // Hello World
echo "Hello {$name}s";        // Hello Worlds

// Single quotes — NO interpolation, faster for static strings
echo 'Hello $name';           // Hello $name

// Concatenation uses . (dot), not +
echo "Hello " . $name . "!";  // Hello World!

// Heredoc (like JS template literals)
$text = <<<EOT
  Hello $name,
  This is multiline.
EOT;
```

**Common string functions:**

```php
strlen($s)            // length
strtolower($s)        // toLowerCase
strtoupper($s)        // toUpperCase
str_replace('a','b',$s)
trim($s)              // strip whitespace
explode(',', $s)      // split → array  (JS: split)
implode(',', $arr)    // join array     (JS: join)
substr($s, 0, 5)      // slice
strpos($s, 'needle')  // indexOf (returns false if not found!)
str_contains($s, 'x') // PHP 8+
sprintf("%.2f", 3.14) // formatted string
```

---

## 5. `echo` vs `print`

| | `echo` | `print` |
|---|---|---|
| Returns value | No | Yes (always `1`) |
| Multiple args | Yes (`echo "a", "b"`) | No |
| Speed | Slightly faster | Slightly slower |
| Usage | Preferred everywhere | Rarely used |

```php
echo "Hello";           // ✅ Standard output
echo "Hello", " World"; // ✅ Multiple args
print "Hello";          // ✅ Works, returns 1
$x = print "Hi";        // $x === 1
```

> **Rule of thumb:** Always use `echo`. `print` exists, just forget it.

---

## 6. Arrays

PHP arrays are **ordered maps** — they combine JS Arrays and Objects.

```php
// Indexed array (like JS array)
$fruits = ["apple", "banana", "cherry"];
echo $fruits[0]; // apple

// Associative array (like JS object)
$user = [
  "name" => "Ali",
  "age"  => 30,
];
echo $user["name"]; // Ali

// Mixed
$mixed = [0 => "zero", "key" => "value", 1 => "one"];

// Nested
$matrix = [[1, 2], [3, 4]];
```

**Common array functions:**

```php
count($arr)                  // .length
array_push($arr, $val)       // .push()   — or just $arr[] = $val
array_pop($arr)              // .pop()
array_shift($arr)            // .shift()
array_unshift($arr, $val)    // .unshift()
array_merge($a, $b)          // [...a, ...b]
array_slice($arr, 1, 3)      // .slice(1, 3)
array_map(fn($x) => $x*2, $arr)     // .map()
array_filter($arr, fn($x) => $x>1)  // .filter()
array_reduce($arr, fn($c,$x) => $c+$x, 0)  // .reduce()
in_array('apple', $arr)      // .includes()
array_key_exists('name', $arr)
array_keys($arr)
array_values($arr)
sort($arr)                   // sort in-place (no return value!)
usort($arr, fn($a,$b) => $a - $b)
```

---

## 7. Control Flow

Identical logic to JS, slightly different syntax in some places.

```php
// if / elseif / else  (note: "elseif" not "else if" — both work actually)
if ($x > 10) {
    echo "big";
} elseif ($x > 5) {
    echo "medium";
} else {
    echo "small";
}

// Ternary
$label = $x > 10 ? "big" : "small";

// Null coalescing (like JS ??)
$name = $_GET['name'] ?? "Guest";

// match (PHP 8+) — like switch but strict & returns value
$status = match($code) {
    200, 201 => "OK",
    404      => "Not Found",
    500      => "Server Error",
    default  => "Unknown",
};

// for / while / foreach
for ($i = 0; $i < 10; $i++) { ... }

while ($condition) { ... }

foreach ($arr as $value) { ... }
foreach ($arr as $key => $value) { ... }  // Like Object.entries()
```

---

## 8. Functions

```php
// Declaration
function greet(string $name): string {
    return "Hello, $name";
}

// Default parameters
function greet(string $name = "World"): string { ... }

// Variadic (like JS rest params)
function sum(int ...$nums): int {
    return array_sum($nums);
}

// Arrow functions (PHP 7.4+) — captures outer scope automatically
$double = fn($x) => $x * 2;

// Anonymous functions (closures) — must use `use` to capture outer vars
$multiplier = 3;
$multiply = function($x) use ($multiplier) {
    return $x * $multiplier;
};

// Type hints (PHP 7+)
function divide(float $a, float $b): float { ... }
function maybeNull(?string $s): ?int { ... }  // nullable with ?
function anything(mixed $x): void { ... }     // PHP 8+
```

---

## 9. Classes & OOP

```php
class Animal {
    // Properties with visibility
    public string $name;
    protected int $age;
    private string $secret = "hidden";

    // Constructor
    public function __construct(string $name, int $age) {
        $this->name = $name;  // `this` is $this in PHP
        $this->age  = $age;
    }

    public function speak(): string {
        return "{$this->name} says hello";
    }

    // Static method
    public static function create(string $name): static {
        return new static($name, 0);
    }
}

// Inheritance
class Dog extends Animal {
    public function speak(): string {
        return parent::speak() . " (woof!)";
    }
}

$dog = new Dog("Rex", 3);
echo $dog->speak();         // Rex says hello (woof!)
echo $dog->name;            // Rex (dot → arrow for property access)
echo Animal::create("Cat"); // Static call
```

**Interfaces & Traits:**

```php
interface Serializable {
    public function serialize(): string;
}

trait Timestamps {
    public string $createdAt;
    public function touch(): void { $this->createdAt = date('Y-m-d'); }
}

class Post implements Serializable {
    use Timestamps;
    public function serialize(): string { return json_encode($this); }
}
```

---

## 10. Superglobals

PHP injects these **automatically** on every request — no import needed.

| Superglobal | JS Equivalent | Description |
|---|---|---|
| `$_GET` | `new URL().searchParams` | URL query params |
| `$_POST` | `req.body` (parsed) | POST form/body data |
| `$_REQUEST` | `$_GET + $_POST + $_COOKIE` | All input (avoid) |
| `$_SERVER` | `req.headers` + `process.env` | Server & request info |
| `$_FILES` | `req.files` (multer) | Uploaded files |
| `$_SESSION` | Session middleware | User session data |
| `$_COOKIE` | `document.cookie` / `req.cookies` | Cookies |
| `$_ENV` | `process.env` | Environment variables |
| `$GLOBALS` | global scope | All global variables |

```php
// Reading query params: GET /search?q=php&page=2
$query = $_GET['q'] ?? '';
$page  = (int)($_GET['page'] ?? 1);

// Reading POST body (form submission)
$username = $_POST['username'] ?? '';
$password = $_POST['password'] ?? '';

// Server info
$method   = $_SERVER['REQUEST_METHOD'];  // "GET", "POST"
$uri      = $_SERVER['REQUEST_URI'];     // "/path?query"
$ip       = $_SERVER['REMOTE_ADDR'];
$host     = $_SERVER['HTTP_HOST'];

// Sessions (like server-side cookies)
session_start();           // Must call before using $_SESSION
$_SESSION['user_id'] = 42;
$id = $_SESSION['user_id'] ?? null;
session_destroy();         // logout

// Cookies
setcookie('token', 'abc123', time() + 3600); // set
$token = $_COOKIE['token'] ?? null;           // read
```

> ⚠️ **Never trust superglobals directly.** Always validate and sanitize input.

---

## 11. Request Lifecycle (Full Flow)

```
1. Browser sends:  GET /users?id=5

2. Web server (Nginx/Apache) receives request
   └─ Matches to a PHP file or routes via index.php (front controller)

3. PHP-FPM spins up (or reuses) a worker process

4. PHP parses & executes the .php file top-to-bottom:
   ├─ $_GET, $_POST, $_SERVER etc. are populated
   ├─ Code runs (DB queries, business logic, templating)
   └─ echo/print statements write to output buffer

5. PHP sends output buffer as HTTP response body

6. Connection closes — ALL variables are destroyed
   (Nothing persists except: DB, files, cache, sessions)
```

**Front Controller Pattern** (Laravel, Symfony, etc.):

```
All requests → index.php → Router → Controller → View/Response
```

---

## 12. Type System & Casting

PHP is loosely typed by default, but supports strict types.

```php
// Loose comparison (avoid!)
0 == "foo"   // true  ← JS-like footgun
0 == false   // true
"1" == 1     // true

// Strict comparison (always use ===)
0 === "foo"  // false ✅
0 === false  // false ✅
"1" === 1    // false ✅

// Type casting
$x = (int)"42px";    // 42
$x = (float)"3.14";  // 3.14
$x = (string)100;    // "100"
$x = (bool)"";       // false
$x = (array)"hello"; // ["hello"]

// Strict mode (add at top of every file)
<?php declare(strict_types=1);
```

**Falsy values in PHP:**

```
false, null, 0, 0.0, "0", "", [], unset variables
```

---

## 13. Error Handling

```php
// try / catch / finally — same as JS
try {
    $result = riskyOperation();
} catch (InvalidArgumentException $e) {
    echo $e->getMessage();
} catch (Exception $e) {
    echo "Generic error: " . $e->getMessage();
} finally {
    cleanup();
}

// Throwing exceptions
throw new Exception("Something went wrong");
throw new InvalidArgumentException("Bad input: $value");

// Custom exception
class NotFoundException extends RuntimeException {}
throw new NotFoundException("User not found", 404);
```

---

## 14. Working with JSON

```php
// Encode (JS: JSON.stringify)
$arr  = ["name" => "Ali", "age" => 30];
$json = json_encode($arr);               // '{"name":"Ali","age":30}'
$json = json_encode($arr, JSON_PRETTY_PRINT);

// Decode (JS: JSON.parse)
$data = json_decode($json, true);        // true = associative array
$obj  = json_decode($json);              // stdClass object (default)

echo $data['name'];  // Ali
echo $obj->name;     // Ali
```

---

## 15. File I/O & Includes

```php
// Include files (like import, but imperative)
require 'config.php';       // Fatal error if not found
include 'header.php';       // Warning if not found, continues
require_once 'db.php';      // Only loads once even if called multiple times
include_once 'utils.php';

// File operations
$content = file_get_contents('data.txt');
file_put_contents('data.txt', "Hello");
$lines = file('data.txt');   // returns array of lines
```

---

## 16. Common Patterns

### Output JSON API response:
```php
<?php
header('Content-Type: application/json');
$data = ["status" => "ok", "users" => []];
echo json_encode($data);
exit;
```

### Sanitize user input:
```php
$name = htmlspecialchars($_POST['name'] ?? '', ENT_QUOTES, 'UTF-8');
$id   = filter_input(INPUT_GET, 'id', FILTER_VALIDATE_INT);
```

### PDO database query:
```php
$pdo  = new PDO("mysql:host=localhost;dbname=app", $user, $pass);
$stmt = $pdo->prepare("SELECT * FROM users WHERE id = :id");
$stmt->execute([':id' => $id]);
$user = $stmt->fetch(PDO::FETCH_ASSOC);
```

---

## 17. PHP 8+ Modern Features (Quick Reference)

```php
// Named arguments
array_slice(array: $arr, offset: 1, length: 3);

// Constructor property promotion
class User {
    public function __construct(
        public readonly string $name,
        public int $age = 0,
    ) {}
}

// Nullsafe operator (like JS ?.)
$city = $user?->getAddress()?->city;

// Enums (PHP 8.1)
enum Status: string {
    case Active   = 'active';
    case Inactive = 'inactive';
}

// Fibers (like JS async generators / coroutines)
$fiber = new Fiber(function(): void { ... });
```

---

## Quick Reference: PHP vs JS Cheatsheet

| Concept | JavaScript | PHP |
|---|---|---|
| Variable | `let x = 1` | `$x = 1` |
| String concat | `"a" + "b"` | `"a" . "b"` |
| Template string | `` `Hello ${name}` `` | `"Hello $name"` |
| Array | `[1,2,3]` | `[1, 2, 3]` |
| Object/Map | `{key: val}` | `['key' => $val]` |
| Null coalesce | `x ?? y` | `$x ?? $y` |
| Strict equal | `===` | `===` |
| Arrow fn | `x => x * 2` | `fn($x) => $x * 2` |
| Import | `import x from 'y'` | `require_once 'y.php'` |
| typeof | `typeof x` | `gettype($x)` |
| console.log | `console.log(x)` | `var_dump($x)` or `print_r($x)` |
| JSON encode | `JSON.stringify(x)` | `json_encode($x)` |
| JSON decode | `JSON.parse(s)` | `json_decode($s, true)` |
| Length | `arr.length` | `count($arr)` |
| Class instance | `new Foo()` | `new Foo()` |
| Class property | `this.name` | `$this->name` |
| Static call | `Foo.method()` | `Foo::method()` |
| Async/await | `async/await` | Not native (use ReactPHP/Swoole) |