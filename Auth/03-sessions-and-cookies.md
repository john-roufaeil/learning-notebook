# 03 — Sessions and Cookies

## The stateless HTTP problem

HTTP was designed to be stateless. Every request is independent. The server has no built-in memory of who you are between requests.

After you log in, the very next request arrives at the server with no context. The server sees: an HTTP request from some IP address. That's it. It doesn't know you just authenticated.

To solve this, we need to carry some kind of **proof of authentication** on every request. The two main approaches:

1. **Sessions** — server stores authentication state, client carries only an ID
2. **Tokens** — client carries the full authentication state (covered in the next file)

This file covers sessions.

---

## What is a session?

A session is a temporary record on the server that says "this ID belongs to this user."

```
Server database / memory:
{
  "sess_abc123xyz": {
    userId: 42,
    email: "alice@example.com",
    loginTime: "2024-01-15T10:30:00Z",
    expiresAt: "2024-01-15T18:30:00Z"
  }
}
```

When you log in, the server:
1. Verifies your credentials
2. Creates a session record
3. Generates a random, unpredictable **session ID**
4. Sends that session ID to your browser

On every subsequent request, your browser sends the session ID back. The server looks it up, finds your user info, and knows who you are.

```
Browser → Server:  "GET /profile, my session ID is sess_abc123xyz"
Server  → Browser: "sess_abc123xyz maps to userId 42, here's Alice's profile"
```

The session ID is just a random string — it has no meaning by itself. The meaning lives on the server.

---

## What are cookies?

A **cookie** is a small piece of data that a web browser stores and automatically sends on every request to the same domain.

The server sets a cookie like this:

```http
HTTP/1.1 200 OK
Set-Cookie: session_id=sess_abc123xyz; HttpOnly; Secure; SameSite=Strict; Max-Age=28800
```

From then on, the browser automatically includes it:

```http
GET /profile HTTP/1.1
Host: example.com
Cookie: session_id=sess_abc123xyz
```

You don't write JavaScript to send it. The browser handles it automatically on every request to `example.com`. This is both the power and the danger of cookies.

---

## Cookie attributes — these are not optional

Every security property of a cookie comes from its attributes. Get these wrong and your session system is broken.

### `HttpOnly`

```
Set-Cookie: session_id=abc; HttpOnly
```

With `HttpOnly`, the cookie **cannot be accessed by JavaScript**. `document.cookie` will not show it. This means if an attacker injects malicious JavaScript into your page (XSS), they still can't steal the session cookie.

**Always set `HttpOnly` on session cookies.** There's almost never a legitimate reason for JavaScript to need the session ID.

### `Secure`

```
Set-Cookie: session_id=abc; Secure
```

With `Secure`, the cookie is **only sent over HTTPS**. It will never be sent over plain HTTP, even if the user visits the HTTP version of your site.

**Always set `Secure` in production.** Without it, anyone on the same network (coffee shop WiFi, etc.) can sniff the session ID in plain text.

### `SameSite`

This attribute controls when the browser sends the cookie on cross-site requests.

```
Set-Cookie: session_id=abc; SameSite=Strict
Set-Cookie: session_id=abc; SameSite=Lax
Set-Cookie: session_id=abc; SameSite=None; Secure
```

**`SameSite=Strict`** — Cookie is ONLY sent when navigating within your own site. If a user clicks a link from another site to yours, the cookie is NOT sent on that first request. Very secure, but can break some flows (like clicking a link in an email).

**`SameSite=Lax`** — Cookie is sent on top-level navigations (clicking links) but NOT on cross-site sub-requests (images, iframes, forms). The modern browser default. Good balance of security and usability.

**`SameSite=None; Secure`** — Cookie is sent in all cross-site requests. Requires `Secure`. Use this only for cookies that legitimately need to work across sites (like embedding widgets or third-party auth).

`SameSite` is the primary defense against **Cross-Site Request Forgery (CSRF)** attacks. More on CSRF in `08-common-attacks.md`.

### `Max-Age` and `Expires`

```
Set-Cookie: session_id=abc; Max-Age=28800
Set-Cookie: session_id=abc; Expires=Wed, 15 Jan 2025 18:30:00 GMT
```

`Max-Age` (in seconds) is preferred over `Expires` (which requires a date and is affected by clock skew).

Without either, the cookie is a **session cookie** — it expires when the browser is closed. With Max-Age/Expires, it's a **persistent cookie** and survives browser restarts.

### `Domain`

```
Set-Cookie: session_id=abc; Domain=example.com
```

Controls which subdomains receive the cookie. If set to `example.com`, it's also sent to `api.example.com`, `www.example.com`, etc. If not set, it defaults to the exact host only.

Be careful here — sharing a cookie across subdomains means a XSS vulnerability on any subdomain can steal session cookies.

### `Path`

```
Set-Cookie: session_id=abc; Path=/
```

Controls which URL paths the cookie is sent to. `Path=/` means all paths. Rarely needs to be changed for session cookies.

---

## Session storage on the server

Where does the server store session data?

### In-memory (single server only)

```
sessions = {
  "sess_abc": { userId: 42, ... },
  "sess_xyz": { userId: 17, ... }
}
```

Simple, fast, zero dependencies. Breaks immediately in a multi-server setup (your session might live on server A, but your next request hits server B which doesn't have it). Also lost if the server restarts.

### Database

Store sessions in a SQL or NoSQL database. Works across servers. Slower (requires a DB query per request). Survives restarts. Supports easy revocation (just delete the row).

### Redis (recommended for production)

Redis is an in-memory key-value store with optional persistence. Fast like in-memory, works across servers, survives restarts if configured for it, and natively supports TTL (time-to-live) for automatic expiry.

```
redis.set("session:sess_abc123", JSON.serialize(session), EX=28800)
```

This is the standard production choice for session storage.

---

## Session lifecycle

### Creating a session

```
1. User sends credentials
2. Server verifies credentials → success
3. Server generates session ID: cryptographically random, 128+ bits
   (do NOT use sequential IDs like sess_1, sess_2 — guessable!)
4. Server stores session data in Redis/DB
5. Server sends Set-Cookie with session ID
```

### Validating a session

```
1. Request arrives with Cookie: session_id=sess_abc
2. Server reads the session ID from cookie
3. Server looks up session in storage
4. If not found → 401 (not authenticated)
5. If found but expired → 401, delete the session
6. If valid → attach user info to request context, continue
```

### Expiring a session

Sessions should expire. Two types of expiry:

**Absolute expiry** — session expires at a fixed time regardless of activity. "You must log in again after 8 hours."

**Sliding expiry** — session expiry resets with each request. "Session expires after 30 minutes of inactivity."

Most applications use sliding expiry for active users plus an absolute maximum ("never more than 7 days, regardless of activity").

### Destroying a session (logout)

```
1. User clicks logout
2. Server reads session ID from cookie
3. Server deletes session from storage
4. Server sends Set-Cookie with an expired date to clear the browser's cookie
```

**Critical**: just clearing the client-side cookie is not enough. If the session record still exists on the server, someone with the session ID can still use it. You must delete the server-side record.

---

## Session fixation attack

This is subtle but important.

**Scenario**: An attacker visits your site and gets a session ID (even without logging in). They trick a victim into using that same session ID (e.g., via a crafted link). When the victim logs in, the server associates that session ID with the victim's account. The attacker, who already has that session ID, is now logged in as the victim.

**Defense**: Always generate a **new session ID** on login, even if the user already had a session. Discard the old session ID.

```python
# On successful login:
old_session_data = session.get(old_session_id)
session.delete(old_session_id)
new_session_id = generate_secure_random_id()
session.set(new_session_id, {userId: user.id, ...})
set_cookie("session_id", new_session_id)
```

---

## Session vs Token: when to use sessions

Sessions are ideal when:

- **You need instant revocation** — delete the server-side record and the session is dead immediately
- **You have a traditional web app** — server renders HTML, cookies are natural
- **You don't need to scale massively** — Redis handles millions of sessions easily, but it's still a dependency

Sessions are less ideal when:

- **You have a distributed microservices system** — every service needs access to the session store, or you need a central auth service
- **You're building a pure API** — mobile apps and third-party clients often prefer tokens
- **You need stateless auth** — tokens don't require a shared session store

---

## Common mistakes

1. **Session IDs that are guessable** — must be cryptographically random, at least 128 bits
2. **Storing session data in the cookie** — you're giving the data to the client; they can tamper with it
3. **Missing HttpOnly** — JavaScript can steal the session ID on XSS
4. **Missing Secure** — session ID transmitted in plain text over HTTP
5. **Not invalidating old session ID on login** — session fixation vulnerability
6. **Not deleting server-side session on logout** — clearing the cookie is not logout
7. **Sessions that never expire** — permanent sessions are a permanent attack window

---

## Next

Sessions store state server-side. Tokens are the alternative — the client carries their own proof of identity. The most common token format is JWT: `04-tokens-and-jwt.md`
