# 01 — Foundations: Authentication vs Authorization

## The one-sentence version

**Authentication** asks *"Who are you?"*
**Authorization** asks *"What are you allowed to do?"*

They are different problems solved by different mechanisms. Conflating them is one of the most common mistakes in system design.

---

## Authentication (AuthN)

Authentication is the process of **verifying identity**. You're proving to a system that you are who you claim to be.

Examples:
- Typing your password to log in
- Scanning your fingerprint on your phone
- Clicking a magic link sent to your email
- Plugging in a hardware security key

The system doesn't inherently trust you. Authentication is how you earn that trust — at least temporarily.

### Factors of authentication

Authentication factors are grouped into three categories. Each one answers a different question about *how* you prove identity:

**Something you know** — a secret only you should know
- Password, PIN, security questions (weak), passphrase

**Something you have** — a physical object or device you possess
- Phone (for SMS/TOTP codes), hardware key (YubiKey), smart card

**Something you are** — a biometric characteristic
- Fingerprint, face recognition, voice, iris scan

**Multi-Factor Authentication (MFA)** means requiring two or more of these categories together. Using two passwords is not MFA — both are "something you know." A password + a phone OTP code is MFA.

Why does MFA matter? If an attacker steals your password (something you know), they still can't log in without your phone (something you have). The categories are designed to be independently compromised.

---

## Authorization (AuthZ)

Authorization happens **after** authentication. Once the system knows who you are, authorization decides what you can do.

Examples:
- A regular user can read articles but can't publish them
- An admin user can delete accounts
- A service account can read from a database but not write
- A user can edit their own profile but not someone else's

Authorization is about **permissions, roles, and policies** — not about verifying identity.

### The critical order

```
Request comes in
      ↓
  AuthN: Who is this? → Not verified → Reject (401 Unauthorized)
      ↓
  AuthZ: Can they do this? → Not allowed → Reject (403 Forbidden)
      ↓
  Handle the request
```

Notice the HTTP status codes:
- `401 Unauthorized` — confusingly named; actually means "not authenticated"
- `403 Forbidden` — means "authenticated, but not authorized"

This naming confusion in HTTP is historical. Don't let it confuse your mental model.

---

## Why auth is hard

Auth feels simple until you build it. Here's why it's deceptively complex:

### 1. Stateless HTTP

HTTP is stateless. Every request arrives with no inherent memory of previous requests. After you log in, the server has no automatic way to remember that on the next request. You have to invent a way to carry identity information — and every approach has trade-offs (sessions, tokens, cookies, etc.).

### 2. The attacker model

You have to think about adversaries. Auth is a direct target for attackers because breaking it gives them access to everything else. The threats include:

- Someone guessing or stealing passwords
- Someone intercepting network traffic
- Someone stealing session cookies from the browser
- Someone tricking users into performing actions they didn't intend (CSRF)
- Someone injecting malicious scripts to steal tokens (XSS)
- Replay attacks (reusing a stolen valid token)
- Timing attacks (inferring secrets from response speed)

Building auth means thinking about all of these continuously.

### 3. Revocation is hard

Proving identity is much easier than *un-proving* it. If you issue someone a token that's valid for an hour and they log out, how do you make that token invalid before the hour is up? If you store nothing server-side (stateless tokens), you often can't — you have to design around it.

### 4. Secrets must be kept secret

Your auth system's security depends on keeping certain things secret: private keys, password hashes, session IDs, secrets used to sign tokens. If any of these leak, the entire system is compromised. Secure storage and transmission of secrets is a discipline of its own.

### 5. UX pressure pushes toward insecurity

Users hate friction. "Just keep me logged in forever" is a constant product demand. But longer sessions = longer windows of opportunity for attackers. You're always balancing security against convenience, and the business usually pushes toward convenience.

---

## The threat model mindset

Before building any auth system, ask these questions:

1. **What are you protecting?** Personal data, financial data, health records? The sensitivity determines how strict you need to be.

2. **Who are the legitimate users?** Employees of a company, anonymous members of the public, machines calling an API?

3. **Who are the attackers?** Script kiddies running off-the-shelf tools? Sophisticated nation-state actors? Disgruntled former employees with inside knowledge?

4. **What does a breach cost?** Leaked photos are bad. Leaked bank accounts are worse. Leaked medical records have regulatory consequences.

5. **What's the acceptable friction?** An enterprise internal tool can demand hardware keys. A consumer app demanding the same would lose all its users.

Answering these shapes every decision about which auth mechanisms to use.

---

## A simple mental model

Think of auth like a building:

- **Authentication** is the front desk that checks your ID. It issues you a visitor badge.
- **Authorization** is the set of locked doors that your badge does or doesn't open.
- **The badge** is the token/session that carries your verified identity around.
- **The security policy** is the rulebook that says who gets access to what rooms.

The front desk doesn't decide which rooms you can enter. That's the door locks' job. And the door locks don't verify your identity — they just check if your badge has the right permissions.

---

## What comes next

The rest of this series breaks down every piece of this system:

- How passwords should actually be stored → `02-passwords-and-hashing.md`
- How sessions work and how cookies carry them → `03-sessions-and-cookies.md`
- What tokens are and the JWT format → `04-tokens-and-jwt.md`
- Refresh tokens and how to handle expiry → `05-refresh-tokens.md`
- OAuth2 and OpenID Connect → `06-oauth2-and-oidc.md`
- Permission systems → `07-authorization-models.md`
- Attacks and defenses → `08-common-attacks.md`
- Modern patterns like passkeys and MFA → `09-modern-patterns.md`
- Putting it all together → `10-putting-it-together.md`
