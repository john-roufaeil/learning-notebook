# 04 — Tokens and JWT

## What is a token?

A **token** is a self-contained piece of data that proves identity or authorization. Instead of the server storing your session state and the client carrying just an ID, a token carries everything the server needs — encoded into the token itself.

The flow:

```
Login:
  Client sends credentials
  Server verifies credentials
  Server creates a token: "userId=42, role=admin, expires=1hr, signed with secret_key"
  Server sends token to client

Subsequent requests:
  Client sends token in each request
  Server verifies the token's signature (no DB lookup needed!)
  Server reads the claims from the token
  Server processes the request
```

The key insight: the server doesn't store anything. The token is the session. This makes the server **stateless**.

---

## JWT — JSON Web Token

JWT (pronounced "jot") is the most widely used token format. It's defined in RFC 7519. Almost every modern auth system uses JWTs.

### Structure

A JWT looks like this:

```
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiI0MiIsIm5hbWUiOiJBbGljZSIsImV4cCI6MTcwNTMyODAwMH0.SflKxwRJSMeKKF2QT4fwpMeJf36POk6yJV_adQssw5c
```

It's three base64url-encoded parts separated by dots:

```
HEADER.PAYLOAD.SIGNATURE
```

### Part 1: Header

```json
{
  "alg": "HS256",
  "typ": "JWT"
}
```

- `alg` — the signing algorithm (HS256, RS256, ES256, etc.)
- `typ` — token type, always "JWT"

### Part 2: Payload (Claims)

```json
{
  "sub": "42",
  "name": "Alice",
  "email": "alice@example.com",
  "role": "admin",
  "iat": 1705324400,
  "exp": 1705328000
}
```

The payload contains **claims** — statements about the user or the token itself. Standard registered claims:

| Claim | Name | Description |
|-------|------|-------------|
| `sub` | Subject | The user ID this token is about |
| `iss` | Issuer | Who created the token (e.g., "auth.example.com") |
| `aud` | Audience | Who the token is intended for (e.g., "api.example.com") |
| `exp` | Expiration | Unix timestamp when the token expires |
| `iat` | Issued At | Unix timestamp when the token was created |
| `nbf` | Not Before | Token is not valid before this timestamp |
| `jti` | JWT ID | Unique ID for this token (used for revocation) |

You can add any custom claims: `role`, `permissions`, `orgId`, whatever your app needs.

**⚠️ The payload is NOT encrypted — just base64-encoded. Anyone can decode it.**

```bash
echo "eyJzdWIiOiI0MiIsIm5hbWUiOiJBbGljZSJ9" | base64 -d
# {"sub":"42","name":"Alice"}
```

Never put sensitive data (passwords, payment info, PII beyond what's necessary) in a JWT payload.

### Part 3: Signature

The signature proves the token hasn't been tampered with. It's computed as:

```
signature = HMAC_SHA256(
  base64url(header) + "." + base64url(payload),
  secret_key
)
```

Or with asymmetric algorithms:

```
signature = RSA_SHA256_sign(
  base64url(header) + "." + base64url(payload),
  private_key
)
```

When the server receives a JWT, it:
1. Re-computes what the signature should be
2. Compares it to the signature in the token
3. If they match, the token is authentic and unmodified
4. If they don't match, reject it

---

## Signing algorithms

### HS256 (HMAC + SHA-256) — Symmetric

Uses a single secret key to both sign and verify. Simple but requires all servers to share the same secret.

```
sign:   signature = HMAC(header.payload, secret)
verify: compute HMAC(header.payload, secret) → does it match?
```

Use when: single service issuing and consuming tokens, or microservices that all trust the same secret.

### RS256 (RSA + SHA-256) — Asymmetric

Uses a **private key** to sign, **public key** to verify. The auth server keeps the private key secret; it can publish the public key anywhere. Any service can verify tokens without being able to create them.

```
sign:   signature = RSA_sign(header.payload, private_key)
verify: RSA_verify(header.payload, signature, public_key)
```

Use when: multiple services need to verify tokens but only one service should issue them. The public key can be distributed safely.

### ES256 (ECDSA + SHA-256) — Asymmetric

Like RS256 but uses Elliptic Curve cryptography. Shorter signatures, faster operations, same security properties.

**Recommendation**: Use RS256 or ES256 in production, especially in multi-service architectures.

---

## The `none` algorithm vulnerability

This is infamous. The JWT spec includes `"alg": "none"` — a token with no signature.

Some early JWT libraries, when they saw `"alg": "none"`, would skip signature verification. An attacker could take any JWT, change the payload to say `"role": "admin"`, set `"alg": "none"`, and the server would accept it.

**Defense**: Always explicitly specify which algorithms you accept. Never accept `none`. Most modern libraries handle this correctly by default, but verify your library version.

```python
# Wrong — accepts any algorithm including 'none'
jwt.decode(token, key)

# Right — explicitly specify
jwt.decode(token, key, algorithms=["HS256"])
```

---

## Storing JWTs on the client

Where should the browser store a JWT? This is a genuine trade-off.

### Option 1: Cookie (HttpOnly)

```http
Set-Cookie: access_token=eyJ...; HttpOnly; Secure; SameSite=Strict
```

**Pros:**
- Can't be stolen by XSS (JavaScript can't read it)
- Browser sends it automatically

**Cons:**
- Vulnerable to CSRF (browser automatically sends it on cross-site requests)
- Requires CSRF protection (though `SameSite=Strict` largely solves this)

### Option 2: localStorage / sessionStorage

```javascript
localStorage.setItem("access_token", token);
// then on each request:
headers: { "Authorization": `Bearer ${localStorage.getItem("access_token")}` }
```

**Pros:**
- Not vulnerable to CSRF (you manually add it to headers)
- Works naturally with SPAs and mobile apps

**Cons:**
- Accessible to JavaScript — stolen by XSS attacks
- If there's any XSS vulnerability on your site, the token is gone

### Option 3: In-memory (JS variable)

```javascript
let accessToken = null;

async function login(credentials) {
  const response = await fetch("/auth/login", ...);
  accessToken = response.json().access_token;
}
```

**Pros:**
- Not in localStorage, so not accessible to other scripts
- Gone when the page reloads (can't be stolen from persistent storage)

**Cons:**
- Lost on page refresh — must refresh using a cookie-stored refresh token
- Requires a more complex auth architecture

**The honest answer**: HttpOnly cookie wins for most web apps because XSS is more common than CSRF, and CSRF is largely mitigated by `SameSite`. For pure APIs consumed by mobile apps or third-party clients, tokens in headers (from secure native storage) is fine.

---

## Sending JWTs in requests

The standard way is the `Authorization` header:

```http
GET /api/profile HTTP/1.1
Host: api.example.com
Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...
```

`Bearer` is the token type defined in RFC 6750. The server parses this header, extracts the token, and validates it.

---

## JWT validation — what the server must check

When a JWT arrives, the server must verify ALL of the following:

```
1. Is the structure valid? (three parts, valid base64)
2. Is the algorithm what we expect? (not 'none', not something unexpected)
3. Is the signature valid? (proves it wasn't tampered with)
4. Has it expired? (check exp claim)
5. Is it being used too early? (check nbf claim if present)
6. Is the issuer correct? (check iss claim if you set it)
7. Is the audience correct? (check aud claim if you set it)
```

Skipping any of these is a vulnerability. Most JWT libraries do all of this if you configure them correctly — but read the docs.

---

## The revocation problem

Here's the fundamental tension with JWTs: **you cannot revoke a stateless JWT before it expires**.

Scenario: A user's account gets compromised. You want to kick them out immediately. You can delete server-side sessions in milliseconds. But if they have a JWT that expires in 1 hour, there's nothing to delete — the token is valid until its `exp` timestamp passes.

Workarounds:

### Short expiry times

Issue JWTs that expire in 5-15 minutes. The attacker's window is small. The downside: users need to get new tokens frequently. This is why refresh tokens exist (next file).

### Token blocklist (denylist)

Maintain a set of revoked JWT IDs (`jti` claims). Check incoming tokens against this list.

```
blocklist = {"jti_abc123", "jti_xyz456", ...}

on request:
  if token.jti in blocklist:
    reject
```

This requires a shared store (Redis) — now you have state again, which partially defeats the purpose of stateless JWTs. But it's a practical solution for when you need revocation.

### Asymmetric + short-lived access tokens + refresh tokens

The most common real-world pattern. Access tokens are short-lived (15 min). Refresh tokens are long-lived and stored server-side (can be revoked). The access token's short lifespan limits exposure; the refresh token enables revocation.

More on this in `05-refresh-tokens.md`.

---

## Common JWT mistakes

1. **Not validating the `alg` field** — accepting `"alg": "none"` or algorithm confusion attacks (RS256 vs HS256)
2. **Trusting the payload without verifying the signature** — the payload is just base64, anyone can decode it
3. **Putting secrets in the payload** — payload is readable by anyone who has the token
4. **Long expiry times (days/weeks) on access tokens** — massive revocation problem
5. **Not checking `exp`, `iss`, `aud`** — a valid signature doesn't mean a valid token
6. **Using `HS256` with a weak secret** — short/guessable secrets make HMAC crackable
7. **Storing in localStorage without understanding XSS risk** — one vulnerable page = token theft

---

## JWT vs Session: quick summary

| | Sessions | JWT |
|--|---------|-----|
| State | Server-side | Client-side (stateless) |
| Revocation | Instant (delete session) | Hard (wait for expiry or use blocklist) |
| Storage | Redis/DB | Cookie or client memory |
| Scalability | Requires shared session store | No shared state needed |
| Size | Tiny session ID | ~200-500 bytes |
| Multi-service | Needs session store access | Any service can verify with public key |
| Inspection | Opaque to client | Readable (but not writable) by client |

---

## Next

Access tokens expire quickly. What keeps users logged in without forcing them to re-enter credentials? Refresh tokens: `05-refresh-tokens.md`
