# 06 — OAuth 2.0 and OpenID Connect

## What problem does OAuth 2.0 solve?

Two different but related problems:

**Problem 1: "Login with Google"** — Users want to log in to your app using their existing Google account without creating a new password.

**Problem 2: Third-party API access** — A user wants to let your app read their Google Calendar. Without OAuth, they'd have to give you their Google password — dangerous and terrible practice. OAuth lets them grant limited, revocable access.

OAuth 2.0 is the standard protocol that solves both. OpenID Connect (OIDC) sits on top of OAuth 2.0 and specifically handles authentication (proving identity).

---

## The cast of characters

OAuth 2.0 defines four roles:

**Resource Owner** — The user. They own the data and grant permission.

**Client** — Your app. The thing that wants access to the user's data.

**Authorization Server** — The server that handles login and issues tokens. This is Google, GitHub, Okta, or your own auth service. Sometimes called the "identity provider" (IdP).

**Resource Server** — The API that holds the actual data (e.g., Google Calendar API). Often the same service as the Authorization Server, but logically separate.

---

## OAuth 2.0 flows (grant types)

Different situations call for different flows. There are four main ones:

### 1. Authorization Code Flow (the most common)

Used when your app has a backend server. The most secure flow.

```
1. User clicks "Login with Google" in your app

2. Your app redirects user to Google's auth page:
   https://accounts.google.com/o/oauth2/auth
     ?client_id=YOUR_APP_ID
     &redirect_uri=https://yourapp.com/callback
     &response_type=code
     &scope=openid email profile
     &state=random_unguessable_string

3. User logs in to Google and approves permissions

4. Google redirects user back to your redirect_uri with:
   https://yourapp.com/callback
     ?code=4/abc123...
     &state=random_unguessable_string

5. Your backend verifies the state (CSRF protection)
   Then exchanges the code for tokens:
   POST https://oauth2.googleapis.com/token
     client_id=YOUR_APP_ID
     client_secret=YOUR_SECRET
     code=4/abc123...
     grant_type=authorization_code
     redirect_uri=https://yourapp.com/callback

6. Google responds with:
   {
     "access_token": "ya29...",
     "refresh_token": "1//...",
     "id_token": "eyJ...",
     "expires_in": 3600
   }

7. Your app uses the access_token to call Google APIs
   And uses the id_token to identify the user
```

**Why the code exchange?** The authorization code is short-lived and single-use. The actual tokens are obtained server-to-server (your backend to Google). This means tokens never travel through the user's browser, which could be logged or stolen.

### 2. Authorization Code Flow + PKCE (for SPAs and mobile apps)

PKCE (Proof Key for Code Exchange, pronounced "pixy") is an extension for clients that can't keep secrets — like a single-page app (JavaScript can't hide a `client_secret`) or a mobile app.

The idea: before starting the flow, the client generates a random `code_verifier` and sends a `code_challenge` (hash of the verifier) to the auth server. When exchanging the code for tokens, the client sends the `code_verifier`. The server verifies it matches the challenge. An attacker who intercepts the code can't exchange it without the original verifier.

```
Client generates:
  code_verifier = random_secure_string()
  code_challenge = base64url(sha256(code_verifier))

Authorization request includes:
  code_challenge=BASE64URL(sha256(verifier))
  code_challenge_method=S256

Token exchange includes:
  code_verifier=ORIGINAL_STRING
```

**PKCE is now recommended for all flows**, including server-side apps. It's just extra security at no cost.

### 3. Client Credentials Flow

Used when **no user is involved** — machine-to-machine (M2M) communication. A backend service authenticating to another backend service.

```
Your service → Auth Server: POST /token
  client_id=YOUR_SERVICE_ID
  client_secret=YOUR_SERVICE_SECRET
  grant_type=client_credentials
  scope=read:data

Auth Server → Your service:
  { "access_token": "...", "expires_in": 3600 }

Your service → Target API: request + access_token
```

No user login, no redirects. Just service authentication.

### 4. Device Code Flow

For devices with no browser (smart TVs, CLI tools, IoT devices).

```
1. Device requests a code from the auth server
   → Gets: device_code, user_code ("ABCD-EFGH"), verification_uri

2. Device displays: "Go to example.com/activate and enter ABCD-EFGH"

3. User goes to that URL on their phone/computer and logs in

4. Device polls the auth server every few seconds:
   "Has the user authorized device_code yet?"

5. Once the user approves, the next poll returns tokens
```

---

## Scopes

Scopes are strings that describe what access is being requested. They're shown to the user on the consent screen.

```
scope=openid email profile calendar.read
```

Common scopes:
- `openid` — request an ID token (OIDC-specific)
- `email` — access to the user's email address
- `profile` — access to name, picture, etc.
- `offline_access` — request a refresh token

For your own API, you define your own scopes:
```
scope=articles:read articles:write admin:users
```

The authorization server issues tokens that are limited to the requested scopes. The resource server checks that the token has the required scope before serving the request.

---

## The `state` parameter and why it matters

When your app starts the OAuth flow, it includes a random `state` value:

```
https://auth-server.com/authorize?...&state=kj3h92kd
```

The auth server returns the `state` in the callback:

```
https://yourapp.com/callback?code=abc&state=kj3h92kd
```

Your app MUST verify that the returned `state` matches what it sent. This prevents CSRF attacks where an attacker tricks your callback endpoint into processing a flow it didn't initiate.

---

## OpenID Connect (OIDC)

OAuth 2.0 was designed for **authorization** (access to APIs), not **authentication** (verifying identity). Using OAuth 2.0 alone for login is often done incorrectly. OpenID Connect fixes this by adding an identity layer on top of OAuth 2.0.

OIDC adds:
1. The `openid` scope — signals that this is an authentication request
2. The **ID Token** — a JWT that contains the user's identity information
3. The **UserInfo endpoint** — an endpoint to get more user claims
4. Standardized claims (`sub`, `name`, `email`, `picture`, etc.)
5. The concept of an **identity provider** (IdP)

### The ID Token

When you include `scope=openid` in an OAuth request, the auth server returns an ID token alongside the access token:

```json
{
  "iss": "https://accounts.google.com",
  "sub": "110169484474386276334",
  "aud": "your-client-id",
  "exp": 1705328000,
  "iat": 1705324400,
  "email": "alice@example.com",
  "email_verified": true,
  "name": "Alice Smith",
  "picture": "https://..."
}
```

The ID token tells you **who the user is**. The access token tells you **what you can do on their behalf**.

**Critical distinction**:
- **ID Token** → for your app to know who the user is. Don't send this to APIs.
- **Access Token** → for calling APIs. Don't read this to know who the user is.

### Validating the ID Token

You must validate the ID token:
1. Verify the signature (using the IdP's public keys, available at their JWKS endpoint)
2. Check `iss` matches the expected issuer
3. Check `aud` matches your `client_id`
4. Check `exp` — not expired
5. Check `iat` — not issued in the future

---

## The JWKS endpoint

When using asymmetric signing (RS256), how do you get the public key to verify the signature?

Auth servers publish their public keys at a **JWKS (JSON Web Key Set)** endpoint:

```
https://accounts.google.com/.well-known/jwks.json
```

```json
{
  "keys": [
    {
      "kty": "RSA",
      "kid": "key-id-1",
      "n": "...",
      "e": "AQAB",
      "alg": "RS256",
      "use": "sig"
    }
  ]
}
```

Your app fetches these keys (and caches them), then uses them to verify JWT signatures. The `kid` (key ID) in the JWT header tells you which key to use.

### Discovery endpoint

OIDC-compliant servers publish a discovery document:

```
https://accounts.google.com/.well-known/openid-configuration
```

This JSON document tells you all the endpoints, supported algorithms, scopes, and claims. Your OIDC library uses this to configure itself automatically.

---

## Putting OAuth2 + OIDC together: "Login with Google"

```
User clicks "Login with Google"
     ↓
Your app redirects to Google:
  /authorize?client_id=X&scope=openid email profile&response_type=code&state=ABC
     ↓
User logs in to Google, approves permissions
     ↓
Google redirects to your callback:
  /callback?code=4/XYZ&state=ABC
     ↓
Your backend:
  1. Verifies state=ABC matches what it sent
  2. POST /token with the code + client_secret
     ↓
Google returns:
  { access_token, refresh_token, id_token }
     ↓
Your backend:
  1. Validates the id_token (signature, iss, aud, exp)
  2. Extracts sub (user ID), email, name from id_token
  3. Looks up or creates user in your DB based on sub
  4. Creates your own session or issues your own JWT
  5. Logs user in
```

**Important**: The access token and refresh token from Google are for **Google's APIs**. They're not for your app. Your app should issue its own session/token after verifying the identity.

---

## Common OAuth mistakes

1. **Not validating the `state` parameter** — CSRF vulnerability
2. **Confusing ID token with access token** — ID token identifies the user; access token accesses APIs
3. **Sending the access token to your own backend and trusting it** — it's opaque; you can't verify it without calling Google's token info endpoint
4. **Not validating the ID token** — skipping signature/claims verification
5. **Using implicit flow** — an older flow that puts tokens in the URL fragment. Deprecated in OAuth 2.1. Use Authorization Code + PKCE instead.
6. **Not checking token audience (`aud`)** — a token issued for another app could be replayed against yours
7. **Storing Google's access token long-term** — if you don't need to call Google APIs, you don't need to store it

---

## When to use what

| Scenario | Flow |
|----------|------|
| Web app with backend | Authorization Code |
| SPA (no backend) | Authorization Code + PKCE |
| Mobile app | Authorization Code + PKCE |
| Backend service to backend service | Client Credentials |
| Smart TV / CLI / IoT device | Device Code |
| "Login with Google/GitHub/etc." | Authorization Code + OIDC |

---

## Next

Once you know who the user is, you need to decide what they can do. That's authorization models: `07-authorization-models.md`
