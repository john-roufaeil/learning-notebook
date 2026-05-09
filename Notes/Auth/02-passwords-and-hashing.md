# 02 — Passwords and Hashing

## Why you never store passwords in plain text

If you store `password: "hunter2"` in your database and that database leaks, every user's password is immediately exposed. Worse — because people reuse passwords across sites, attackers will try those credentials on Gmail, banks, everything.

The solution: **never store the password itself. Store a hash of it.**

---

## What is a hash function?

A hash function takes any input and produces a fixed-length output (the "digest"). It has three important properties:

1. **Deterministic** — same input always gives same output
2. **One-way** — you can't reverse the output back to the input
3. **Avalanche effect** — changing one character in the input completely changes the output

```
SHA-256("hunter2")     → "f52fbd32b2b3b86ff88ef6c490628285f482af15ddcb29541f94bcf526a3f6c7"
SHA-256("hunter3")     → "06b36e8a8fd5e3d8b4cf7a7f89d5b7db9f52c7db8c35c4f042e6e4d0c92c7dc8"
```

The outputs look nothing alike despite the inputs differing by one character.

**So if someone leaks the hash, they can't recover the password... right?**

Almost. The problem is rainbow tables and brute force.

---

## The problem with plain hashing

If you hash passwords with a plain fast hash (MD5, SHA-256), you're still vulnerable:

### Rainbow tables

A rainbow table is a precomputed list of millions of common passwords and their hashes. An attacker who gets your database just looks up the hash and finds the password instantly.

```
"hunter2"   → f52fbd3... ← found in table. Password: hunter2.
"password"  → 5f4dcc3... ← found in table. Password: password.
```

### Brute force is fast

SHA-256 is designed to be *fast*. That's great for checksums, terrible for passwords. A modern GPU can compute **10+ billion** SHA-256 hashes per second. Given a leaked hash, an attacker can try every 8-character password combination in hours.

---

## Salting

A **salt** is a random value generated uniquely for each user and stored alongside the hash. The password is hashed together with the salt.

```
salt = "xK9#mP2z"  (random, stored in DB)
hash = SHA256("hunter2" + "xK9#mP2z")  → stored in DB
```

This defeats rainbow tables because precomputed tables don't account for your unique salts. The attacker has to brute-force each password individually.

**Important**: salts don't need to be secret. They just need to be unique per user. Storing the salt in the same database as the hash is fine — the salt's job is to prevent precomputed attacks, not to hide.

---

## Password hashing algorithms

Even with salting, fast hash functions are still brute-forceable. The solution: use algorithms specifically designed to be **slow**.

### bcrypt

Released in 1999, still widely used. Designed from the start for password storage.

Key properties:
- Built-in salting (the salt is part of the output)
- **Work factor** (cost factor) — a number that controls how slow the hash is
- Automatically includes the salt in the hash output

```
bcrypt("hunter2", cost=12) → "$2b$12$LQv3c1yqBWVHxkd0LHAkCOYz6TtxMQJqhN8/Lewkm4U3yHoUMSXX2"
                              ↑   ↑  ↑
                          version cost  salt+hash combined
```

A cost of 12 means 2^12 = 4096 rounds of hashing. Increasing cost by 1 doubles the time.

**Recommended cost**: tune it so hashing takes ~100-300ms on your hardware. As hardware gets faster, increase the cost factor.

**Limitation**: bcrypt truncates passwords at 72 bytes. Very long passwords get silently cut off.

### Argon2

The winner of the 2015 Password Hashing Competition. The modern recommendation.

Three variants:
- **Argon2d** — resistant to GPU attacks, vulnerable to side-channel attacks. Use for cryptocurrency.
- **Argon2i** — resistant to side-channel attacks. Use for passwords.
- **Argon2id** — hybrid of both. **Use this one for most purposes.**

Argon2id parameters:
- **Time cost** — number of iterations
- **Memory cost** — how much RAM to use (this is the key advantage over bcrypt — it's memory-hard, making GPU attacks expensive)
- **Parallelism** — how many threads to use

```python
# Pseudocode
hash = argon2id(
    password="hunter2",
    salt=random_bytes(16),
    time_cost=3,        # iterations
    memory_cost=65536,  # 64 MB
    parallelism=4,      # threads
)
```

The memory requirement makes it very expensive to run billions of attempts on GPUs, which excel at parallelism but have limited memory.

### scrypt

Memory-hard like Argon2, designed by Colin Percival. Older than Argon2 and harder to configure correctly. Prefer Argon2id for new systems.

### PBKDF2

Recommended by NIST, used in many older systems. No memory-hardness, but still much better than plain hashing. The iteration count needs to be very high (600,000+ for SHA-256 as of 2023 NIST guidelines).

### What to use

| Algorithm | Recommendation |
|-----------|---------------|
| Argon2id | ✅ Best choice for new systems |
| bcrypt | ✅ Fine, widely supported, proven |
| scrypt | ✅ Acceptable |
| PBKDF2 | ⚠️ Acceptable, increase iterations |
| SHA-256/MD5 (even salted) | ❌ Never use for passwords |
| Plain storage | ❌ Absolutely never |

---

## Verification flow

```
Registration:
  user sends password "hunter2"
  server generates random salt
  server computes hash = bcrypt("hunter2", salt, cost=12)
  server stores (username, salt, hash) — never the password

Login:
  user sends password "hunter2"
  server looks up user record, gets stored hash
  server computes bcrypt("hunter2", stored_salt, cost=12)
  server compares result to stored hash
  if they match → authenticated
```

**Important**: use a constant-time comparison function when comparing hashes. A regular string comparison can leak timing information — if it returns early when finding the first mismatched character, an attacker can measure response times to guess hashes byte by byte. Most crypto libraries provide a constant-time compare.

---

## Password policies

What makes a good password policy? Modern NIST guidelines (SP 800-63B) are surprisingly permissive:

**Do:**
- Require a minimum length (8 characters minimum; longer is better)
- Allow all characters including spaces and special chars
- Check against lists of known-breached passwords (haveibeenpwned API, etc.)
- Let users paste passwords (discouraging password managers hurts security)

**Don't:**
- Require complex character mixes (they lead to predictable patterns: "P@ssw0rd!")
- Force periodic password changes (causes minor variations: "hunter2!" → "hunter3!")
- Use security questions (easily guessable or researched from social media)
- Truncate long passwords silently (looking at you, bcrypt 72-byte limit)

**Checking against breach databases**: you can check whether a user's password appears in known data breaches using the [HaveIBeenPwned API](https://haveibeenpwned.com/API/v3#PwnedPasswords). They use a k-anonymity model so you send only the first 5 characters of the SHA-1 hash — they never see the actual password.

---

## Upgrading hashes in production

If you have an existing system with weak hashes (MD5, SHA-1, etc.) and need to migrate without forcing everyone to reset their passwords:

**Rehash on login** — when a user logs in and you verify their old hash, immediately rehash their password with the new algorithm and update the record. Over time, most active users migrate. Force a reset for the rest.

**Double-hashing** — wrap the old hash in a new one: `argon2id(old_md5_hash)`. This immediately protects all hashes at rest, without needing to know the original password.

---

## Common mistakes

1. **Hashing with SHA-256 and calling it secure** — fast hashes are not for passwords
2. **Storing the salt separately and "securing" it** — missing the point; salts just need to be unique
3. **Using the same salt for all users** — defeats the purpose
4. **Not using a constant-time compare** — timing oracle attack
5. **Rolling your own crypto** — use established libraries like `bcrypt`, `argon2-cffi`, `passlib`
6. **Logging the password** — input validation errors can log request bodies; make sure passwords aren't in logs

---

## Next

Passwords get you authenticated — but HTTP is stateless. How does the server *remember* you're authenticated on the next request? That's sessions and cookies: `03-sessions-and-cookies.md`
