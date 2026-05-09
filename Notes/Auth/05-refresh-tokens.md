# 05 — Refresh Tokens

## The problem: short-lived tokens expire

If access tokens expire in 15 minutes for security, users would have to log in every 15 minutes. That's unusable.

If you make access tokens last 7 days to avoid this, you lose the ability to meaningfully revoke them — a stolen token is valid for days.

**Refresh tokens** solve this. They're a second token, held alongside the access token, whose only job is to get you a new access token when the current one expires.

---

## The two-token pattern

The system issues two tokens on login:

```
Access Token:
  - Short-lived: 5-15 minutes
  - Carried on every API request
  - Stateless JWT
  - When it expires, use the refresh token

Refresh Token:
  - Long-lived: hours, days, or weeks
  - Sent only to the auth server's /refresh endpoint
  - Stored server-side (can be revoked)
  - Used to get a new access token
```

### Flow diagram

```
LOGIN
  User → Auth Server: credentials
  Auth Server → User: access_token (15min) + refresh_token (7 days)

NORMAL REQUESTS (access token is fresh)
  User → API: request + access_token
  API → User: response

ACCESS TOKEN EXPIRES
  User → API: request + expired access_token
  API → User: 401 Unauthorized

GET NEW ACCESS TOKEN
  User → Auth Server: POST /refresh, body: {refresh_token}
  Auth Server → User: new access_token (15min)
  (possibly also: new refresh_token — see Token Rotation below)

REPEAT NORMAL REQUESTS with new access token
```

---

## What refresh tokens actually are

Refresh tokens are typically:

- A random, opaque, cryptographically secure string (not a JWT)
- Stored in a database on the server with the associated user ID and expiry
- Sent to the client as a secure, HttpOnly cookie (not accessible to JavaScript)

```
database: refresh_tokens table
| token_hash       | user_id | created_at | expires_at  | revoked |
|------------------|---------|------------|-------------|---------|
| 8f3a2c9b1d7e4f6a | 42      | 2024-01-15 | 2024-01-22  | false   |
```

Note: store the **hash** of the refresh token, not the token itself. Same principle as passwords — if your database leaks, raw tokens aren't exposed.

---

## Refresh token rotation

**Token rotation** means: every time you use a refresh token, you get a new refresh token back (in addition to the new access token). The old refresh token is immediately invalidated.

```
User → Auth Server: old_refresh_token
Auth Server:
  1. Validates old_refresh_token
  2. Marks old_refresh_token as revoked
  3. Issues new_access_token
  4. Issues new_refresh_token
  5. Returns both to user
```

Why? If a refresh token is stolen, rotation gives you **automatic detection**. Here's how:

```
Timeline:
  User gets refresh_token_A

  Attacker steals refresh_token_A

  User uses refresh_token_A → gets access_token_1 + refresh_token_B
  (refresh_token_A is now revoked)

  Attacker tries to use refresh_token_A → FAILS (it's revoked)

  ← Attack detected, user is safe
```

But what if the attacker uses the token first?

```
  Attacker uses refresh_token_A → gets access_token_1 + refresh_token_B
  (refresh_token_A is now revoked)

  User tries to use refresh_token_A → FAILS

  ← Breach detected! The token was already used.
  → Server should revoke the entire refresh token family for this user
  → User must log in again
```

With rotation, any use of an already-revoked refresh token signals a breach in progress. The server should revoke all refresh tokens for that user, forcing re-authentication.

---

## Refresh token families

To implement the breach detection above, you group refresh tokens into a **family** (a chain of tokens, each descending from the original).

```
login → refresh_token_A (family: family_001)
use A → refresh_token_B (family: family_001, parent: A)
use B → refresh_token_C (family: family_001, parent: B)
```

If an already-used token in the family is presented, revoke the entire family. The user has to log in again.

---

## Storing refresh tokens on the client

Refresh tokens need to be stored securely because they're long-lived and powerful.

### Best approach: HttpOnly Secure cookie

```http
Set-Cookie: refresh_token=abc123...; HttpOnly; Secure; SameSite=Strict; Path=/auth/refresh; Max-Age=604800
```

- `HttpOnly` — JavaScript can't read it (XSS-proof)
- `Secure` — HTTPS only
- `SameSite=Strict` — not sent on cross-site requests (CSRF protection)
- `Path=/auth/refresh` — only sent to the refresh endpoint, not all API calls

This is the most secure option for web apps.

### For native mobile apps

Use the platform's secure storage:
- iOS: Keychain
- Android: EncryptedSharedPreferences / Android Keystore

Never store in plain file storage, SharedPreferences (Android), or NSUserDefaults (iOS) — those can be read if the device is compromised.

---

## Refresh token expiry and renewal

Refresh tokens themselves expire. There are different strategies:

### Absolute expiry

Refresh token is valid until a fixed time (e.g., 30 days from issue). After that, user must log in again. Simple and predictable.

### Sliding expiry with rotation

Every time a refresh token is used (rotated), the new refresh token gets a full new expiry window. Active users never get logged out. Inactive users (who haven't used the refresh token) eventually expire.

This is probably the most common production approach: "If you've been using the app, you stay logged in. If you close the app for 30 days, you need to log in again."

### Maximum lifetime cap

Regardless of rotation, tokens expire after an absolute maximum (e.g., 90 days). This forces a full re-authentication periodically, even for active users. Useful for high-security apps.

---

## Refresh endpoint implementation

```
POST /auth/refresh

Input:
  - refresh_token (from cookie or request body)

Process:
  1. Extract refresh token
  2. Hash it
  3. Look up hash in database
  4. Check: exists? not revoked? not expired? belongs to a valid user?
  5. If rotation is enabled:
     a. Mark old token as revoked
     b. Generate new refresh token
     c. Store new token hash in DB
  6. Generate new access token (JWT, 15min)
  7. Return: new access token (body) + new refresh token (HttpOnly cookie)

Errors:
  - Token not found → 401
  - Token revoked → 401 + revoke entire family → alert user
  - Token expired → 401 → tell user to log in
```

---

## Logout and revocation

**Logout** should:
1. Revoke the refresh token on the server (mark as revoked or delete)
2. Clear the refresh token cookie (set expired)
3. Discard the access token client-side (it'll expire on its own within 15 min)

For cases where you need to revoke *all* a user's sessions (e.g., account compromise, password change):
- Mark all refresh tokens for that user as revoked
- Optionally, add current access token JTIs to a blocklist

---

## Silent refresh in SPAs

In a Single Page App, when the access token expires mid-session, you don't want the user to see an error. You implement **silent refresh**: automatically refresh the access token in the background.

```javascript
// Simplified example
async function apiRequest(url, options) {
  let response = await fetch(url, {
    ...options,
    headers: { Authorization: `Bearer ${accessToken}` }
  });

  if (response.status === 401) {
    // Access token expired — try to refresh
    const refreshed = await refreshAccessToken();
    if (refreshed) {
      // Retry the original request with new token
      response = await fetch(url, {
        ...options,
        headers: { Authorization: `Bearer ${accessToken}` }
      });
    } else {
      // Refresh failed — redirect to login
      redirectToLogin();
    }
  }

  return response;
}
```

The refresh token cookie is sent automatically by the browser to `/auth/refresh`. The new access token is stored in memory. The original request is retried. The user sees nothing.

---

## Common mistakes

1. **Making access tokens too long-lived** — defeats the whole purpose of the two-token pattern
2. **Storing refresh tokens in localStorage** — if XSS can steal your access token, it can also steal your refresh token, and the refresh token lasts much longer
3. **Not implementing rotation** — no breach detection
4. **Not revoking the old token on rotation** — rotation without revocation does nothing
5. **Refresh tokens that never expire** — permanent refresh tokens = permanent access
6. **Not revoking all tokens on password change** — if someone changes their password after a breach, old refresh tokens should be invalidated
7. **Same endpoint for refresh and API** — refresh token cookie should have `Path=/auth` so it's only sent to the auth service

---

## Summary

```
Access token:  short (15min), stateless JWT, travels with every request
Refresh token: long (days), stored in DB, only travels to /refresh endpoint
Rotation:      every use generates a new refresh token, old one is revoked
Breach detect: if a revoked token is used, the entire family is revoked
Storage:       refresh token in HttpOnly cookie; access token in memory
```

---

## Next

JWTs and refresh tokens cover your own auth system. But what about letting users log in with Google, GitHub, etc.? Or building an API that third-party apps can access? That's OAuth 2.0: `06-oauth2-and-oidc.md`
