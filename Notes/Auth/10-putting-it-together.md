# 10 — Putting It All Together

## Architecture decisions: a framework

When building auth for a new system, work through these questions in order:

**1. Who are the users?**
- Real humans with accounts → passwords or passkeys + sessions/JWTs
- Other services → API keys or OAuth2 client credentials

**2. Do you have a backend?**
- Yes → Authorization Code flow (sessions or JWTs both work)
- Pure SPA / mobile → Authorization Code + PKCE (tokens stored in memory/secure storage)

**3. Do you need federated identity (Google/GitHub/enterprise SSO)?**
- Yes → Implement OIDC; consider an auth service (Auth0, Okta, Keycloak, Supabase Auth)
- No → Build your own auth or use a library

**4. How many services need to know who the user is?**
- One service → sessions + server-side storage, very simple
- Multiple services → JWTs (each service verifies independently) or centralized auth service

**5. How sensitive is the data?**
- Moderate → MFA optional, standard session length
- High → MFA required, step-up auth for sensitive operations, shorter sessions

---

## Architecture patterns

### Pattern 1: Traditional web app (the classic)

Best for: server-rendered apps, simple backends, CMS, internal tools.

```
Client ←→ Server ←→ Database
              ↓
           Redis (sessions)
```

- Users log in with email/password (+ optional MFA)
- Server creates a session in Redis on successful auth
- Session ID stored in a secure HttpOnly cookie
- Every request: server reads cookie → looks up session in Redis → gets user
- Logout: delete the Redis record

**Tools**: Express-session, Flask-Session, Rails sessions, Django sessions.

**Pros**: Simple, instant revocation, no token management.
**Cons**: Requires Redis or shared DB for multi-server.

---

### Pattern 2: SPA + REST API

Best for: React/Vue/Angular frontends with a separate backend API.

```
Browser (SPA)
  ↓ login
Auth API → issues: access_token (JWT, 15min) + refresh_token (HttpOnly cookie)
  ↓
Resource API (stateless — verifies JWT on each request)
```

- Access token stored in memory (SPA JS variable, not localStorage)
- Refresh token in HttpOnly cookie (browser sends it automatically to /auth/refresh)
- When access token expires → silently call /auth/refresh → get new access token
- Resource API verifies JWT signature — no shared session store needed

**Pros**: Stateless API, scales easily, separation of concerns.
**Cons**: More complex, silent refresh logic required.

---

### Pattern 3: Microservices

Best for: large systems with many services.

```
Client → API Gateway → Auth Service
                     → Service A
                     → Service B
                     → Service C
```

**Option A: JWT with public key**
- Auth Service issues JWTs signed with a private key
- All services verify JWTs using the published public key (from JWKS endpoint)
- No shared state, fully stateless

**Option B: Token introspection**
- Auth Service issues opaque tokens
- Other services call Auth Service to validate tokens on each request
- More overhead, but supports instant revocation

**Option C: API Gateway handles auth**
- Gateway validates the token and sets trusted headers (`X-User-ID`, `X-User-Roles`)
- Backend services trust these headers (they only accept requests from the gateway)
- Services don't need JWT libraries

---

### Pattern 4: Using an identity provider / auth service

Building auth from scratch is complex and error-prone. For most teams, using a managed auth service is the right call.

Options:
- **Auth0** — mature, feature-rich, generous free tier
- **Okta** — enterprise-focused, SSO-heavy
- **Supabase Auth** — open source, integrates with Supabase DB
- **Clerk** — developer-friendly, great UX for passkeys/MFA
- **Keycloak** — self-hosted, open source, very configurable
- **Cognito** — AWS-native, integrates well with AWS ecosystem

When to DIY:
- You have very specific requirements that services can't meet
- Data sovereignty requirements
- Cost at extreme scale

When to use a service:
- You're moving fast and auth isn't your core business
- You need SSO, MFA, and social login out of the box
- You want compliance (SOC2, HIPAA) without the overhead

---

## Security checklist

### Passwords
- [ ] Hashed with bcrypt (cost ≥ 12) or Argon2id
- [ ] Never logged or stored in plain text
- [ ] Check against breach databases on registration
- [ ] Minimum length 8 chars; no maximum below 64 chars
- [ ] Allow password manager paste

### Sessions
- [ ] Session IDs are cryptographically random (128+ bits)
- [ ] HttpOnly + Secure + SameSite on session cookies
- [ ] Session regenerated after login (prevent fixation)
- [ ] Server-side session invalidated on logout
- [ ] Sessions expire (absolute and/or sliding)

### JWT
- [ ] Short expiry (15 min for access tokens)
- [ ] Explicit algorithm validation (reject `none`)
- [ ] Verify signature + exp + iss + aud
- [ ] Never put sensitive data in payload
- [ ] Stored in HttpOnly cookie or JS memory (not localStorage for critical apps)

### Refresh tokens
- [ ] Long random value, not a JWT
- [ ] Stored hashed in DB
- [ ] Token rotation on every use
- [ ] Entire token family revoked on breach detection
- [ ] Revoked on logout and password change

### OAuth/OIDC
- [ ] State parameter validated (CSRF protection)
- [ ] ID token validated (signature, iss, aud, exp)
- [ ] Redirect URIs strictly whitelisted
- [ ] Using Authorization Code + PKCE (not implicit flow)

### API
- [ ] Every endpoint checks authentication
- [ ] Every endpoint checks authorization
- [ ] Authorization checks resource ownership (not just role)
- [ ] No sensitive data in URL parameters (appears in logs)
- [ ] Rate limiting on auth endpoints

### Transport
- [ ] HTTPS everywhere, no HTTP fallback
- [ ] HSTS header set
- [ ] TLS 1.2+ only

### General
- [ ] Auth events logged (login, logout, failures, password changes)
- [ ] Alerts on anomalies (many failures, unusual location)
- [ ] MFA available (required for high-risk users)
- [ ] Dependency updates tracked (JWT library vulnerabilities)

---

## Common architecture mistakes at scale

**Mistake: Long-lived access tokens to avoid refresh complexity**

The fix is not longer tokens — it's proper silent refresh implementation. 15-minute access tokens with silent refresh on 401 is the pattern. Don't extend token lifetime because the refresh logic is annoying.

**Mistake: Putting everything in the JWT to avoid DB lookups**

JWTs have a size limit (they go in cookies/headers). More importantly, large JWTs with many claims mean stale data. Roles and permissions change; you don't want a user with a revoked role to retain it until their JWT expires. Use minimal claims in the JWT and look up detailed permissions from DB or cache when needed.

**Mistake: Trusting JWTs from users without verifying the algorithm**

An attacker who knows your public key can take an RS256 token, change `"alg": "HS256"`, and forge a valid HMAC signature using the public key as the HMAC secret. Always explicitly specify `algorithms=["RS256"]` in your verification call.

**Mistake: Homegrown token formats**

A homebrew format that's "like JWT but simpler" typically skips expiry, audience, or signature entirely. Use JWT with a well-maintained library.

**Mistake: Same session/token for different security levels**

Regular browsing and "access to payment info" shouldn't use the same auth level. Use step-up auth.

---

## How the pieces fit together: a complete example

Alice logs in to an SPA that talks to a REST API:

```
1. Alice enters email + password
   → POST /auth/login {email, password}

2. Server:
   a. Looks up Alice's user record
   b. Verifies password against bcrypt hash
   c. Checks MFA if enabled
   d. Generates access_token (JWT, 15 min, signed with RS256 private key)
   e. Generates refresh_token (random 256-bit hex, stored hashed in DB)
   f. Returns access_token in response body
   g. Sets refresh_token as HttpOnly Secure SameSite=Strict cookie

3. SPA stores access_token in memory (JS variable)

4. Alice makes API calls:
   GET /api/profile
   Authorization: Bearer eyJhbGci...

5. API server:
   a. Extracts token from Authorization header
   b. Verifies signature using RS256 public key
   c. Checks exp, iss, aud claims
   d. Reads sub (user ID) and roles from payload
   e. Checks if user has permission for this endpoint
   f. Returns Alice's profile data

6. Alice is idle for 15 minutes. Access token expires.

7. Alice makes another API call:
   GET /api/articles
   Authorization: Bearer eyJhbGci... (expired)

8. API returns 401 Unauthorized

9. SPA catches the 401, calls the refresh endpoint:
   POST /auth/refresh
   (refresh_token cookie sent automatically by browser)

10. Auth server:
    a. Reads refresh_token from cookie
    b. Hashes it, looks up in DB
    c. Checks not expired, not revoked
    d. Marks old refresh_token as revoked (rotation)
    e. Issues new refresh_token, stores hash in DB
    f. Issues new access_token (JWT, 15 min)
    g. Returns new access_token in body
    h. Sets new refresh_token cookie

11. SPA retries original request with new access_token → success

12. Alice clicks "Log out":
    POST /auth/logout

13. Server:
    a. Reads refresh_token from cookie
    b. Marks it as revoked in DB
    c. Sets expired refresh_token cookie (clears it)

14. SPA discards in-memory access_token. Alice is logged out.
```

Every piece has a reason. No step is optional.

---

## Quick reference: which token goes where

| Token | Stored | Sent to | Lifetime |
|-------|--------|---------|----------|
| Session ID | HttpOnly cookie | Server (every request) | Session or fixed duration |
| Access token (JWT) | JS memory | API (Authorization header) | 5-15 minutes |
| Refresh token | HttpOnly cookie | Auth server only (/refresh) | Hours to weeks |
| API key | Client secret store | API (header) | Long (manual rotation) |
| TOTP secret | Authenticator app | Never transmitted after setup | Permanent |
| Magic link token | DB (hashed) | Email (in link) | 5-15 minutes, single-use |

---

## Final advice

1. **Don't roll your own crypto.** Use established libraries. Use established services if you can.

2. **Boring auth is good auth.** The most secure systems use well-understood, simple patterns. Clever custom solutions introduce subtle bugs.

3. **Threat model before building.** Know what you're protecting and from whom. Don't add complexity you don't need.

4. **Security is a process.** Update dependencies. Audit regularly. Have an incident response plan. Monitor for anomalies.

5. **Test auth thoroughly.** Write tests that confirm unauthorized requests are rejected. Test that revoked sessions don't work. Test that expired tokens are rejected.

6. **"Secure by default" means something fails closed.** When in doubt, deny access. It's better for a legitimate user to get a 403 than for an attacker to get unauthorized access.

---

## The full reading list

| File | Topics |
|------|--------|
| `01-foundations.md` | AuthN vs AuthZ, threat model |
| `02-passwords-and-hashing.md` | bcrypt, Argon2, salting |
| `03-sessions-and-cookies.md` | Sessions, cookie attributes, CSRF |
| `04-tokens-and-jwt.md` | JWT structure, signing, validation, pitfalls |
| `05-refresh-tokens.md` | Two-token pattern, rotation, storage |
| `06-oauth2-and-oidc.md` | OAuth2 flows, OIDC, ID token |
| `07-authorization-models.md` | RBAC, ABAC, ACL, ReBAC |
| `08-common-attacks.md` | CSRF, XSS, IDOR, injection |
| `09-modern-patterns.md` | Passkeys, MFA, magic links, SSO, API keys |
| `10-putting-it-together.md` | Architecture, checklists, examples |
