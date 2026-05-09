# 09 — Modern Auth Patterns

## Multi-Factor Authentication (MFA)

MFA requires users to prove their identity using two or more different factors. Even if an attacker steals a password, they can't log in without the second factor.

### TOTP — Time-Based One-Time Passwords

TOTP generates a 6-digit code that changes every 30 seconds. Both the server and the user's authenticator app independently compute the same code using a shared secret and the current time.

**How it works:**
1. During setup, the server generates a shared secret (e.g., `JBSWY3DPEHPK3PXP`)
2. This secret is shared with the user's authenticator app (via QR code)
3. Every 30 seconds, both parties compute: `TOTP = HMAC(secret, floor(time / 30))`
4. The codes match without any network communication

Apps: Google Authenticator, Authy, 1Password, Bitwarden.

**Implementation:**
```
Registration:
  1. Server generates a random secret
  2. Server stores secret for the user (encrypted at rest)
  3. Server displays a QR code encoding: otpauth://totp/AppName:user@email?secret=XXX&issuer=AppName
  4. User scans with authenticator app
  5. User enters a code to confirm setup worked
  6. Server generates recovery codes

Verification:
  1. User enters their password (factor 1)
  2. Server prompts for TOTP code (factor 2)
  3. Server computes expected TOTP for current time (and ±30s window for clock drift)
  4. If code matches → authenticated
```

**Recovery codes**: Always give users one-time recovery codes at setup (10-20 codes). If they lose their phone, they can use these to get back in. Store them hashed, like passwords.

### SMS/Email OTP

A one-time code sent to the user's phone or email. Easier to use than TOTP, but weaker:
- **SMS**: Vulnerable to SIM-swapping attacks (attacker ports your number to their SIM), and phone network interception
- **Email**: Only as secure as the email account

Use TOTP or hardware keys when security requirements are high. SMS/email OTP is acceptable for low-to-medium risk.

### Hardware security keys (FIDO2/WebAuthn)

The strongest second factor. A physical device (YubiKey, Google Titan Key) plugged into USB or touched for NFC.

Uses public-key cryptography:
- Key pair is generated on the device during registration
- Private key never leaves the device
- During login, the server sends a challenge; the device signs it with the private key
- Server verifies with the stored public key

**Phishing-resistant**: The key is tied to the specific domain. Even if a user visits a convincing fake site, the key won't authenticate because the domain doesn't match.

---

## Passkeys (WebAuthn / FIDO2)

Passkeys are the next generation of authentication — they replace passwords entirely.

### What is a passkey?

A passkey is a FIDO2 credential stored on your device (phone, laptop, security key). It uses public-key cryptography — the private key stays on the device, the public key is registered with the server.

When you log in:
1. The server sends a challenge
2. Your device authenticates you locally (Face ID, Touch ID, PIN, or hardware key)
3. Your device signs the challenge with the private key
4. The server verifies the signature with the stored public key

**No password is ever sent.** No secret is ever transmitted. Even if the server is compromised, the private key can't be stolen from it (it was never there).

### Why passkeys are better

| Property | Passwords | Passkeys |
|----------|-----------|---------|
| Phishing | Phishable — users enter on fake sites | Phishing-resistant — tied to domain |
| Breach exposure | Hashes can be cracked if stolen | Public keys are useless without private key |
| Reuse | Users reuse passwords | Unique key pair per site |
| Sharing | Users share passwords (bad) | Private key can't be shared |
| UX | Remember and type | Touch ID or Face ID |

### Synced vs device-bound passkeys

**Synced passkeys** (iCloud Keychain, Google Password Manager, 1Password) — passkey syncs across your devices. Convenient, and recovery is possible through account recovery. The private key is encrypted in the cloud.

**Device-bound passkeys** (hardware security keys) — passkey lives only on the device. Highest security, but if you lose the device, you lose the passkey.

### Implementation (high level)

WebAuthn is the browser API for FIDO2. It's now supported in all major browsers.

```javascript
// Registration
const credential = await navigator.credentials.create({
  publicKey: {
    challenge: serverChallenge,
    rp: { name: "My App", id: "myapp.com" },
    user: { id: userId, name: "alice@example.com", displayName: "Alice" },
    pubKeyCredParams: [{ type: "public-key", alg: -7 }] // ES256
  }
});
// Send credential.response to server for storage

// Authentication
const assertion = await navigator.credentials.get({
  publicKey: {
    challenge: serverChallenge,
    rpId: "myapp.com",
    allowCredentials: [{ type: "public-key", id: storedCredentialId }]
  }
});
// Send assertion to server for verification
```

Use a library for the server-side logic — `SimpleWebAuthn` (Node.js), `py_webauthn` (Python), `webauthn` (Ruby).

---

## Magic Links

No password at all. When a user wants to log in, they enter their email address and receive a link. Clicking the link logs them in.

```
1. User enters email address
2. Server generates a random, single-use token
3. Server sends email: "Click to log in: https://app.com/auth?token=abc123"
4. User clicks link
5. Server verifies: token exists? not expired? not used?
6. Server marks token as used
7. Server logs user in (creates session or issues JWT)
```

**Pros:**
- No passwords to remember or forget
- No password database to breach
- Phishing resistance (they need access to the email account)
- Great UX for infrequent users

**Cons:**
- Requires access to email
- Dependent on email deliverability
- Slightly slower (wait for email)
- Email itself can be compromised

**Implementation details:**
- Tokens must be cryptographically random
- Tokens must expire (5-15 minutes is typical)
- Tokens must be single-use (mark as used immediately on first click)
- Rate limit the generation endpoint (prevent spam)

---

## SSO — Single Sign-On

SSO lets users log in once and access multiple applications without re-entering credentials.

**Enterprise SSO**: Your company uses Okta, Azure AD, Google Workspace, or a similar identity provider. Employees log in once to the IdP, and all company apps — Slack, GitHub, Salesforce, Jira — are accessible without separate logins.

**Consumer SSO**: "Login with Google" or "Login with Apple." One account, many apps.

### How enterprise SSO works (SAML or OIDC)

**SAML 2.0** — XML-based, older, common in enterprise. Uses signed XML "assertions" instead of JWTs.

**OIDC** — JSON-based, modern, consumer-friendly. Uses JWTs. Preferred for new implementations.

The flow (OIDC SSO):
```
1. User visits your app (Service Provider / SP)
2. Your app detects they're not logged in
3. Your app redirects to the IdP (e.g., Okta)
4. User is already logged in to Okta (or logs in now)
5. Okta redirects back with an authorization code
6. Your app exchanges code for ID token
7. Your app creates its own session
```

If the user visits your app again later, they go through steps 3-7 again, but step 4 doesn't require them to re-enter credentials — Okta still has their session.

### Implementing SSO for your app

If you're adding SSO support to your app (so enterprise customers can use their own IdP):
- Support OIDC (required for modern providers)
- Support SAML 2.0 (required for legacy enterprise)
- Use a library or service (Auth0, Okta, Keycloak) rather than implementing yourself
- Allow customers to register their IdP metadata (OIDC discovery URL or SAML metadata XML)

---

## API Keys

API keys are long-lived credentials for machine-to-machine access (not user auth). Common for developer APIs.

```
X-API-Key: sk_live_4a8f3b2c9d1e7f6a...
```

### Generation

```python
import secrets
api_key = "sk_live_" + secrets.token_hex(32)  # prefix + 64 random hex chars
```

The prefix (`sk_live_`, `pk_test_`, `gh_`) helps users identify what kind of key it is and lets you grep logs for accidentally exposed keys.

### Storage

Never store the raw API key — store a hash of it:
```python
stored = sha256(api_key)  # store this in DB
```

When the user makes a request with the key, hash it and look up the hash. The raw key is shown only once at creation time.

### Scoping

Give API keys scopes, just like OAuth tokens. A key with `read:data` can't write. A key with `admin` can do everything.

### Rotation and revocation

Users should be able to create and revoke keys. Treat rotation as routine (like password rotation). Monitor for keys used from unexpected IPs or in unexpected patterns.

### Detecting exposed keys

Use tools like `git-secrets` or GitHub's secret scanning to detect keys accidentally committed. Some services (Stripe, AWS) automatically revoke keys when they're detected in public GitHub repositories.

---

## Passwordless with Email/Phone Verification

Similar to magic links, but sends a short code instead of a link:

```
1. User enters phone number
2. Server sends SMS: "Your code is 847291"
3. User enters code
4. Server verifies and logs user in
```

Common for mobile apps where you want low friction. The code should be 6 digits, short-lived (5 minutes), rate-limited, and single-use.

---

## Step-Up Authentication

For high-risk actions (transferring money, changing email, accessing sensitive data), require the user to re-verify their identity even if they're already logged in.

```
User is logged in (regular session)
User tries to change their email
  → "Please re-enter your password to continue"
User enters password
  → Change email action is allowed (for the next 5 minutes)
```

This is step-up auth: the user "steps up" to a higher assurance level for sensitive operations. Implemented by:
1. Keeping a timestamp of the last password verification in the session
2. Checking if that verification was recent enough for the requested action
3. Prompting for re-verification if not

---

## Choosing the right auth method

| Scenario | Recommended |
|----------|-------------|
| Consumer web app | Email/password + MFA optional |
| High-security consumer | Passkeys or FIDO2 keys |
| Enterprise app | Email/password + SSO + MFA required |
| Internal tools | SSO (Google/Okta) only |
| Infrequent users | Magic links |
| Developer API | API keys with scopes |
| M2M / service accounts | OAuth2 client credentials or API keys |
| Very high security | Hardware security keys + SSO |

---

## Next

All the pieces together — making architecture decisions for real systems: `10-putting-it-together.md`
