# Authentication & Authorization — Deep Dive Series

> A complete reference for mid-level software engineers. Read in order or jump to any topic.

---

## Table of Contents

| # | File | Topics Covered |
|---|------|---------------|
| 1 | [01-foundations.md](./01-foundations.md) | What auth is, AuthN vs AuthZ, threat model, why it's hard |
| 2 | [02-passwords-and-hashing.md](./02-passwords-and-hashing.md) | Password storage, hashing, salting, bcrypt, Argon2 |
| 3 | [03-sessions-and-cookies.md](./03-sessions-and-cookies.md) | HTTP sessions, cookies, flags, session fixation, CSRF |
| 4 | [04-tokens-and-jwt.md](./04-tokens-and-jwt.md) | Tokens, JWT deep dive, signing, claims, pitfalls |
| 5 | [05-refresh-tokens.md](./05-refresh-tokens.md) | Refresh tokens, rotation, revocation, storage strategies |
| 6 | [06-oauth2-and-oidc.md](./06-oauth2-and-oidc.md) | OAuth 2.0 flows, OpenID Connect, scopes, consent |
| 7 | [07-authorization-models.md](./07-authorization-models.md) | RBAC, ABAC, ACLs, ReBAC, policy engines |
| 8 | [08-common-attacks.md](./08-common-attacks.md) | CSRF, XSS, session hijacking, token theft, injection |
| 9 | [09-modern-patterns.md](./09-modern-patterns.md) | Passkeys, MFA, magic links, SSO, API keys |
| 10 | [10-putting-it-together.md](./10-putting-it-together.md) | Architecture decisions, when to use what, checklists |

---

## How to use this guide

Each file is self-contained. If you already know the basics, skip to whatever you need. If you're building something new, read `10-putting-it-together.md` first to orient yourself, then come back to the relevant deep dives.

Every topic is explained **from scratch** — no assumed knowledge beyond basic web development.
