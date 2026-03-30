# I — Interface Segregation Principle (ISP)
### *"Clients should not be forced to depend on interfaces they do not use."* — Robert C. Martin

---

## Table of Contents
1. [The Pain — The Fat Interface Problem](#the-pain--the-fat-interface-problem)
2. [What ISP Actually Means](#what-isp-actually-means)
3. [The "Fat Interface" Anti-Pattern](#the-fat-interface-anti-pattern)
4. [Example 1 — The Printer Machine Interface](#example-1--the-printer-machine-interface)
5. [Example 2 — Animal Kingdom Interfaces](#example-2--animal-kingdom-interfaces)
6. [Example 3 — Worker Interface in a Factory](#example-3--worker-interface-in-a-factory)
7. [Example 4 — REST API Client Interface](#example-4--rest-api-client-interface)
8. [Real World Scenario — Media Player System](#real-world-scenario--media-player-system)
9. [ISP and the Relationship to LSP](#isp-and-the-relationship-to-lsp)
10. [How to Design Good Interfaces](#how-to-design-good-interfaces)
11. [ISP in Dynamic Languages (Python, JS)](#isp-in-dynamic-languages-python-js)
12. [Common Misunderstandings](#common-misunderstandings)
13. [Summary](#summary)

---

## The Pain — The Fat Interface Problem

Imagine you start a new job. On day one, your manager hands you a job contract. The contract says your responsibilities are:

- Writing backend code
- Designing the UI
- Managing the database servers
- Writing marketing copy
- Doing customer support calls
- Managing vendor relationships
- Cleaning the office

You're a backend developer. You only agreed to write backend code. But now you're legally bound to a contract that includes dozens of things you don't do, have no training for, and never agreed to.

That's exactly what a **fat interface** does to your classes.

In code, when a class is forced to implement an interface with 15 methods but only actually uses 3, you get:

```python
class SimpleLogger(ICompleteLoggingSystem):
    def log_info(self, msg): print(msg)       # ✅ Uses this
    def log_error(self, msg): print(msg)      # ✅ Uses this

    # Forced to "implement" all of these — but does nothing:
    def log_to_database(self, msg): pass      # ❌ Can't do this
    def send_alert_email(self, msg): pass     # ❌ Doesn't do this
    def rotate_logs(self): pass               # ❌ Doesn't do this
    def archive_to_s3(self): pass             # ❌ Doesn't do this
    def parse_log_format(self, fmt): pass     # ❌ Doesn't do this
    def generate_report(self): pass           # ❌ Doesn't do this
    # ... 6 more empty methods
```

These empty implementations are **lies**. They tell the world "I can do this" but they do nothing. Any code that depends on `archive_to_s3()` being real will silently fail. This is a maintenance trap and a bug factory.

---

## What ISP Actually Means

> **"Clients should not be forced to depend on interfaces they do not use."**

A "client" is any class that uses (implements or calls) an interface.

The principle says: don't force a class to know about methods it has no business knowing about. Interfaces should be **small** and **focused** — tailored to specific needs rather than trying to describe every possible thing a category of object could do.

The antidote is **interface segregation**: break large, fat interfaces into smaller, focused ones. Classes then implement only the interfaces that match their actual capabilities.

Think of it like this: instead of one giant Swiss Army knife contract, you have separate contracts — one for cutting, one for opening bottles, one for filing nails. An object only signs the contracts for the tools it actually has.

### Why does it matter?

1. **Empty implementations are bugs waiting to happen.** Code that calls `archive_to_s3()` and gets nothing back silently fails. No error. No log. Just... nothing.

2. **Coupling increases.** When you implement a fat interface, your class is now coupled to every method in it. A change to `rotate_logs()` might force you to recompile/redeploy your `SimpleLogger` even though it doesn't use `rotate_logs()`.

3. **Testing becomes harder.** To test one class, you need to mock an entire 15-method interface, even if only 2 methods are actually used.

4. **Discoverability suffers.** When reading a class that implements a 20-method interface, you see 18 empty stubs. Signal-to-noise ratio drops to zero.

---

## The "Fat Interface" Anti-Pattern

A fat interface is one that:
- Has too many methods
- Forces implementors to stub out methods they can't support
- Tries to be "the interface for all possible X objects"
- Changes for many different reasons (violates SRP at the interface level)

Fat interfaces often grow gradually. The interface starts small and clean, then someone adds one more method... then another... then another. Six months later it's a monster that no one wants to implement.

**Warning signs of a fat interface:**
- More than 5–7 methods (rough guideline)
- Methods that are only used by 1–2 of the implementing classes
- Methods that require throwing `NotImplementedException` or returning `null`/`None` in some implementations
- Implementing classes that are wildly different from each other

---

## Example 1 — The Printer Machine Interface

This is Robert Martin's original example — and it's perfect.

You're building software for office machines. Some are basic printers. Some are full all-in-one machines that print, scan, fax, and staple.

### ❌ BAD — One fat interface for all machines

```java
interface IMultiFunctionMachine {
    void print(Document document);
    void scan(Document document);
    void fax(Document document);
    void staple(Document document);
    void photocopy(Document document);
}

// Modern all-in-one machine — implements all of these naturally
class ModernOfficeDevice implements IMultiFunctionMachine {
    public void print(Document doc) { /* laser print */ }
    public void scan(Document doc) { /* scan to PDF */ }
    public void fax(Document doc) { /* send via fax line */ }
    public void staple(Document doc) { /* auto-staple */ }
    public void photocopy(Document doc) { /* photocopy */ }
}

// Old basic printer — ONLY prints. Forced to implement everything else.
class OldBasicPrinter implements IMultiFunctionMachine {
    public void print(Document doc) {
        // Actually prints — this works
        inkjet.print(doc);
    }

    public void scan(Document doc) {
        throw new UnsupportedOperationException("No scanner!");  // 💥 LSP violation
    }

    public void fax(Document doc) {
        throw new UnsupportedOperationException("No fax!");  // 💥
    }

    public void staple(Document doc) {
        throw new UnsupportedOperationException("No stapler!");  // 💥
    }

    public void photocopy(Document doc) {
        throw new UnsupportedOperationException("No photocopier!");  // 💥
    }
}
```

The `OldBasicPrinter` is forced to "implement" 4 operations it fundamentally cannot do. Anyone who receives an `IMultiFunctionMachine` and calls `scan()` on the `OldBasicPrinter` will get a runtime exception.

### ✅ GOOD — Segregated interfaces

```java
// Fine-grained, focused interfaces
interface IPrinter {
    void print(Document document);
}

interface IScanner {
    void scan(Document document);
}

interface IFax {
    void fax(Document document);
}

interface IStapler {
    void staple(Document document);
}

interface IPhotocopier {
    void photocopy(Document document);
}

// Old basic printer — only implements what it can actually do
class OldBasicPrinter implements IPrinter {
    public void print(Document doc) {
        inkjet.print(doc);  // Does exactly one thing, does it well
    }
}

// Modern all-in-one — implements everything it supports
class ModernOfficeDevice implements IPrinter, IScanner, IFax, IStapler, IPhotocopier {
    public void print(Document doc) { /* laser print */ }
    public void scan(Document doc) { /* scan to PDF */ }
    public void fax(Document doc) { /* send via fax */ }
    public void staple(Document doc) { /* auto-staple */ }
    public void photocopy(Document doc) { /* photocopy */ }
}

// Medium machine — print and scan, but not fax
class ScannerPrinter implements IPrinter, IScanner {
    public void print(Document doc) { /* print */ }
    public void scan(Document doc) { /* scan */ }
}

// Code that ONLY needs printing — works with ANY printer
void printReport(IPrinter printer, Document report) {
    printer.print(report);  // Safe — all IPrinter implementors can actually print
}

// Code that needs scanning — only accepts scanners
void digitizeDocument(IScanner scanner, Document doc) {
    scanner.scan(doc);  // Safe — no risk of UnsupportedOperationException
}
```

Now the type system enforces the contracts. You literally cannot pass an `OldBasicPrinter` to `digitizeDocument()` — it doesn't implement `IScanner`. The compiler catches the error before it hits production.

---

## Example 2 — Animal Kingdom Interfaces

### ❌ BAD — One giant Animal interface

```python
from abc import ABC, abstractmethod

class Animal(ABC):
    @abstractmethod
    def walk(self): pass

    @abstractmethod
    def fly(self): pass

    @abstractmethod
    def swim(self): pass

    @abstractmethod
    def make_sound(self): pass

    @abstractmethod
    def climb(self): pass

# Duck — walks, flies, swims — but can't climb. Forced to implement it anyway.
class Duck(Animal):
    def walk(self): print("Duck waddles")
    def fly(self): print("Duck flies")
    def swim(self): print("Duck swims")
    def make_sound(self): print("Quack")
    def climb(self): pass  # 🤔 Ducks don't climb... empty stub

# Snake — no legs, no wings, can swim. Forced to implement walk, fly, climb.
class Snake(Animal):
    def walk(self): pass          # Snakes don't walk
    def fly(self): pass           # Snakes don't fly
    def swim(self): print("Snake slithers through water")
    def make_sound(self): print("Hiss")
    def climb(self): print("Snake climbs trees")  # ✅ Actually does this!
```

### ✅ GOOD — Segregated capability interfaces

```python
from abc import ABC, abstractmethod

# Each capability is its own focused interface
class Walkable(ABC):
    @abstractmethod
    def walk(self): pass

class Flyable(ABC):
    @abstractmethod
    def fly(self): pass

class Swimmable(ABC):
    @abstractmethod
    def swim(self): pass

class Climbable(ABC):
    @abstractmethod
    def climb(self): pass

class VocalAnimal(ABC):
    @abstractmethod
    def make_sound(self): pass

# Duck implements only what ducks can do
class Duck(Walkable, Flyable, Swimmable, VocalAnimal):
    def walk(self): print("Duck waddles")
    def fly(self): print("Duck flies south for winter")
    def swim(self): print("Duck glides on water")
    def make_sound(self): print("Quack!")

# Snake implements only snake capabilities
class Snake(Swimmable, Climbable, VocalAnimal):
    def swim(self): print("Snake moves through water")
    def climb(self): print("Snake ascends a tree")
    def make_sound(self): print("Hisss")

# Eagle — flies, walks (lands), makes sound — but doesn't swim
class Eagle(Walkable, Flyable, VocalAnimal):
    def walk(self): print("Eagle hops awkwardly")
    def fly(self): print("Eagle soars at 10,000 feet")
    def make_sound(self): print("Screech!")

# Functions only request the capabilities they need
def all_fly(flyers: list[Flyable]):
    for f in flyers:
        f.fly()  # 100% safe — only Flyable things in this list

def chorus(animals: list[VocalAnimal]):
    for a in animals:
        a.make_sound()  # All of these can make sounds

all_fly([Duck(), Eagle()])         # ✅ No snakes accidentally included
chorus([Duck(), Snake(), Eagle()]) # ✅ All make sounds — safe
```

---

## Example 3 — Worker Interface in a Factory

This is another classic example from Martin's writings.

### ❌ BAD

```typescript
interface IWorker {
  work(): void;
  eat(): void;
  sleep(): void;
  attendMeeting(): void;
  fillTimesheet(): void;
}

class HumanWorker implements IWorker {
  work() { console.log("Human is working"); }
  eat() { console.log("Human is eating lunch"); }
  sleep() { console.log("Human goes home to sleep"); }
  attendMeeting() { console.log("Human attends standup"); }
  fillTimesheet() { console.log("Human fills timesheet"); }
}

// A robot works — but doesn't eat, sleep, attend meetings, or fill timesheets
class RobotWorker implements IWorker {
  work() { console.log("Robot is working 24/7"); }
  eat() { /* Robots don't eat */ }    // ❌ Meaningless stub
  sleep() { /* Robots don't sleep */ } // ❌ Meaningless stub
  attendMeeting() { /* ... */ }        // ❌ Robots don't attend meetings
  fillTimesheet() { /* ... */ }        // ❌ Robots don't have timesheets
}
```

### ✅ GOOD

```typescript
// Split by actual responsibility
interface IWorkable {
  work(): void;
}

interface IFeedable {
  eat(): void;
}

interface ISleepable {
  sleep(): void;
}

interface IManageable {
  attendMeeting(): void;
  fillTimesheet(): void;
}

// Human fulfills all human worker responsibilities
class HumanWorker implements IWorkable, IFeedable, ISleepable, IManageable {
  work() { console.log("Human is working"); }
  eat() { console.log("Human is eating lunch"); }
  sleep() { console.log("Human goes home to sleep"); }
  attendMeeting() { console.log("Human attends standup"); }
  fillTimesheet() { console.log("Human fills timesheet"); }
}

// Robot only works — that's it
class RobotWorker implements IWorkable {
  work() { console.log("Robot is working around the clock"); }
}

// Internship — works and can eat (in the break room), but no meetings yet
class Intern implements IWorkable, IFeedable {
  work() { console.log("Intern is working"); }
  eat() { console.log("Intern eats lunch with team"); }
}

// The factory manager only cares about work output
function runFactory(workers: IWorkable[]) {
  workers.forEach(w => w.work());  // Humans, robots, interns — all work
}

// The HR system manages human workers
function runHrProcess(employees: IManageable[]) {
  employees.forEach(e => {
    e.attendMeeting();
    e.fillTimesheet();
  });
}

runFactory([new HumanWorker(), new RobotWorker(), new Intern()]); // ✅
runHrProcess([new HumanWorker()]);  // ✅ No robots or interns passed here
```

---

## Example 4 — REST API Client Interface

Very common in modern web development.

### ❌ BAD

```typescript
interface IApiClient {
  get(url: string): Promise<any>;
  post(url: string, body: any): Promise<any>;
  put(url: string, body: any): Promise<any>;
  patch(url: string, body: any): Promise<any>;
  delete(url: string): Promise<any>;
  setAuthToken(token: string): void;
  setBaseUrl(url: string): void;
  uploadFile(url: string, file: File): Promise<any>;
  downloadFile(url: string): Promise<Blob>;
  setRetryPolicy(retries: number): void;
  setCachePolicy(ttl: number): void;
}

// A read-only API integration — only ever reads data
class PublicDataClient implements IApiClient {
  get(url: string) { return fetch(url); }

  // Forced to implement all of these — but they make no sense
  post() { throw new Error("Read-only client!"); }
  put() { throw new Error("Read-only client!"); }
  patch() { throw new Error("Read-only client!"); }
  delete() { throw new Error("Read-only client!"); }
  setAuthToken() { /* No auth needed */ }
  setBaseUrl() { /* Fixed URL */ }
  uploadFile() { throw new Error("Read-only client!"); }
  downloadFile(url: string) { return fetch(url).then(r => r.blob()); }
  setRetryPolicy() { /* Not configurable */ }
  setCachePolicy() { /* Not configurable */ }
}
```

### ✅ GOOD

```typescript
// Core HTTP verbs
interface IHttpReader {
  get(url: string): Promise<any>;
}

interface IHttpWriter {
  post(url: string, body: any): Promise<any>;
  put(url: string, body: any): Promise<any>;
  patch(url: string, body: any): Promise<any>;
  delete(url: string): Promise<any>;
}

// Authentication
interface IAuthenticatable {
  setAuthToken(token: string): void;
}

// File operations
interface IFileTransferable {
  uploadFile(url: string, file: File): Promise<any>;
  downloadFile(url: string): Promise<Blob>;
}

// Configuration
interface IConfigurable {
  setBaseUrl(url: string): void;
  setRetryPolicy(retries: number): void;
  setCachePolicy(ttl: number): void;
}

// Read-only public API — only reads, no auth
class PublicDataClient implements IHttpReader {
  get(url: string) { return fetch(url); }
}

// Authenticated read/write API
class AuthenticatedApiClient
  implements IHttpReader, IHttpWriter, IAuthenticatable, IConfigurable {
  get(url: string) { /* ... */ }
  post(url: string, body: any) { /* ... */ }
  put(url: string, body: any) { /* ... */ }
  patch(url: string, body: any) { /* ... */ }
  delete(url: string) { /* ... */ }
  setAuthToken(token: string) { /* store token */ }
  setBaseUrl(url: string) { /* set base */ }
  setRetryPolicy(retries: number) { /* configure */ }
  setCachePolicy(ttl: number) { /* configure */ }
}

// Services only ask for what they need
class UserProfileService {
  constructor(private reader: IHttpReader) {}  // Read-only access

  async getProfile(userId: string) {
    return this.reader.get(`/users/${userId}`);
  }
}

class UserAdminService {
  constructor(private client: IHttpWriter & IAuthenticatable) {}

  async deleteUser(userId: string) {
    return this.client.delete(`/users/${userId}`);
  }
}
```

---

## Real World Scenario — Media Player System

You're building a media player that handles audio files, video files, and streaming content. Not all players support all features.

```python
from abc import ABC, abstractmethod

# Segregated media capability interfaces
class AudioPlayable(ABC):
    @abstractmethod
    def play_audio(self, track): pass

    @abstractmethod
    def pause_audio(self): pass

    @abstractmethod
    def adjust_volume(self, level: int): pass

class VideoPlayable(ABC):
    @abstractmethod
    def play_video(self, video): pass

    @abstractmethod
    def adjust_brightness(self, level: int): pass

    @abstractmethod
    def toggle_fullscreen(self): pass

class Streamable(ABC):
    @abstractmethod
    def connect_stream(self, url: str): pass

    @abstractmethod
    def buffer_stream(self, seconds: int): pass

class Recordable(ABC):
    @abstractmethod
    def start_recording(self): pass

    @abstractmethod
    def stop_recording(self): pass

    @abstractmethod
    def save_recording(self, filename: str): pass

# Simple MP3 player — audio only
class Mp3Player(AudioPlayable):
    def play_audio(self, track): print(f"Playing {track}")
    def pause_audio(self): print("Paused")
    def adjust_volume(self, level): print(f"Volume: {level}")

# DVD player — audio + video, no streaming
class DvdPlayer(AudioPlayable, VideoPlayable):
    def play_audio(self, track): print(f"Playing audio: {track}")
    def pause_audio(self): print("Paused")
    def adjust_volume(self, level): print(f"Volume: {level}")
    def play_video(self, video): print(f"Playing video: {video}")
    def adjust_brightness(self, level): print(f"Brightness: {level}")
    def toggle_fullscreen(self): print("Fullscreen toggled")

# Smart TV — everything
class SmartTv(AudioPlayable, VideoPlayable, Streamable, Recordable):
    def play_audio(self, track): print(f"Playing: {track}")
    def pause_audio(self): print("Paused")
    def adjust_volume(self, level): print(f"Volume: {level}")
    def play_video(self, video): print(f"Playing: {video}")
    def adjust_brightness(self, level): print(f"Brightness: {level}")
    def toggle_fullscreen(self): print("Fullscreen")
    def connect_stream(self, url): print(f"Streaming: {url}")
    def buffer_stream(self, secs): print(f"Buffering {secs}s")
    def start_recording(self): print("Recording started")
    def stop_recording(self): print("Recording stopped")
    def save_recording(self, f): print(f"Saved: {f}")

# Service functions only accept what they need
def play_music_playlist(player: AudioPlayable, tracks: list):
    for track in tracks:
        player.play_audio(track)

def stream_live_event(device: Streamable, url: str):
    device.connect_stream(url)
    device.buffer_stream(10)

# Works perfectly — no empty stubs, no runtime errors
play_music_playlist(Mp3Player(), ["Song A", "Song B"])   # ✅
play_music_playlist(SmartTv(), ["Song A", "Song B"])     # ✅
stream_live_event(SmartTv(), "https://live.example.com") # ✅
# stream_live_event(Mp3Player(), "...")  # ✅ Won't compile — type error caught early
```

---

## ISP and the Relationship to LSP

ISP and LSP are deeply connected. Many LSP violations are actually the result of ISP violations.

When you have a fat interface that forces classes to implement things they can't do, you get:
- Classes that throw `NotImplementedException` → LSP violation
- Classes that return `null` or `None` where values are expected → LSP violation
- Classes with empty methods that silently do nothing → subtle LSP violation

**Fixing ISP violations often simultaneously fixes LSP violations.**

If the `Bird` interface had only `eat()` and `make_sound()`, the `Ostrich` would never need to implement `fly()` — the LSP problem disappears because ISP was applied correctly.

The relationship:
```
Fat Interface → Forced Empty Implementations → Silent Failures
                                             → Exception Throws
                                             → LSP Violations
```

```
Segregated Interfaces → Each class implements only what it supports
                      → No empty stubs
                      → No LSP violations
                      → Type system catches misuse at compile time
```

---

## How to Design Good Interfaces

### Rule 1: Design from the client's perspective
Don't ask "what can this thing do?" Ask "what does the thing using it need?"

### Rule 2: One interface per role or capability
`IPrintable`, `IScannable`, `IFaxable` — each one is a distinct capability.

### Rule 3: The "3–5 methods" rule of thumb
If your interface has more than 5–7 methods, ask yourself if it can be split. This isn't a hard rule, but it's a useful red flag.

### Rule 4: If any implementation throws "not supported" — split the interface
This is the strongest signal: if ANY implementing class can't genuinely implement a method, the interface needs to be split.

### Rule 5: Interfaces can be composed
Small interfaces can be combined:
```typescript
// Small, focused interfaces
interface IReadable { read(): string; }
interface IWritable { write(data: string): void; }
interface ICloseable { close(): void; }

// Composition for those that need it
interface IReadWritable extends IReadable, IWritable {}
interface IStream extends IReadable, IWritable, ICloseable {}
```

---

## ISP in Dynamic Languages (Python, JS)

In dynamically typed languages (Python, JavaScript), there are no compile-time interface checks. But ISP still matters:

**In Python**, use Abstract Base Classes (ABCs) to define interface contracts, and use **duck typing** carefully. Even without ABCs, split your conceptual contracts:

```python
# Instead of one giant dependency
class DataService:
    def read(self, id): ...
    def write(self, data): ...
    def delete(self, id): ...
    def export_pdf(self): ...
    def send_email(self): ...

# Use dependency injection with focused protocols
from typing import Protocol

class Reader(Protocol):
    def read(self, id: str): ...

class Writer(Protocol):
    def write(self, data: dict): ...

# Functions take only what they need
def display_data(reader: Reader, record_id: str):
    return reader.read(record_id)

def create_record(writer: Writer, data: dict):
    writer.write(data)
```

Python's `typing.Protocol` (Python 3.8+) enables **structural typing** — if an object has the right methods, it satisfies the protocol, even without explicit `implements`.

---

## Common Misunderstandings

### ❌ Myth 1: "ISP means every interface should have exactly one method"
**Wrong.** That's extreme over-segregation. A good interface groups **cohesive, related** methods. `AudioPlayable` with `play_audio()`, `pause_audio()`, `adjust_volume()` is perfectly fine — those three methods always go together.

### ❌ Myth 2: "ISP only applies to formal interfaces in typed languages"
**Wrong.** In Python and JavaScript, the same principle applies to the implicit contracts your code creates. Passing an object to a function creates an implicit interface — ISP says: only require what you'll actually use.

### ❌ Myth 3: "More interfaces = better design"
**Wrong.** The goal is the **right** interfaces, not the **most** interfaces. 50 single-method interfaces for a simple system is over-engineering. Aim for natural capability groupings.

### ❌ Myth 4: "ISP is just about interfaces — it doesn't affect classes"
**Wrong.** ISP affects class design too. A class that takes a large dependency object and only uses 2 of its 20 methods has an ISP violation — even if no formal interface is involved. The class should depend on a smaller, focused abstraction.

---

## Summary

| Concept | Description |
|---|---|
| **The Principle** | Don't force classes to depend on methods they don't use |
| **The Problem It Solves** | Fat interfaces that create empty stubs, lies, and runtime crashes |
| **The Mechanism** | Split large interfaces into small, focused, role-specific ones |
| **The Benefit** | No meaningless implementations, better type safety, less coupling |
| **The Anti-Pattern** | The "fat interface" — one interface trying to describe everything |
| **The Warning Sign** | Any implementing class throws "not supported" or leaves methods empty |
| **Related Principle** | LSP — ISP violations often cause LSP violations |

> **Practical rule of thumb**: When writing a function or constructor, list exactly which methods of the dependency you actually call. If that list is significantly shorter than the full interface, you have an ISP opportunity. Create a smaller interface that captures only what you use.