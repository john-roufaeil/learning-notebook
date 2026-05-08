# 07 — Authorization Models

## What authorization actually means in code

You've authenticated the user. Now a request arrives: `DELETE /articles/42`. Should you allow it?

That depends on:
- Who the user is (`userId: 7`)
- What role they have (`role: "editor"`)
- What they own (`article 42 was created by userId: 12`)
- What the article's permissions say

Authorization is the system that answers these questions. There are several well-defined models for structuring it.

---

## Model 1: ACL — Access Control List

An ACL is the most granular model. You define, per resource, exactly which users can do what.

```
Article 42:
  - userId 7:  can read, can write
  - userId 12: can read, can write, can delete
  - userId 99: can read

File "report.pdf":
  - userId 7:  can read
  - group "managers": can read, can write
```

**Like Unix file permissions, but more flexible.**

**Pros:**
- Fine-grained control — you can give specific access to specific things
- Intuitive — "this user can access this resource"

**Cons:**
- Doesn't scale — if you have millions of users and millions of resources, the ACL table is enormous
- Hard to manage — "What can user 7 do?" requires scanning many ACL records
- No concept of roles or groups without extra machinery

**When to use**: Document sharing (Google Docs model), filesystem-style permissions, small systems.

---

## Model 2: RBAC — Role-Based Access Control

Instead of assigning permissions directly to users, you assign permissions to **roles**, and assign roles to users.

```
Roles:
  admin:   create, read, update, delete
  editor:  create, read, update
  viewer:  read

Users:
  Alice → admin
  Bob   → editor
  Carol → viewer
```

When Alice tries to delete an article, the check is:
```
Alice has role: admin
admin has permission: delete
→ Allow
```

**Pros:**
- Easy to understand and manage
- "What can an editor do?" is one lookup
- Adding a new user: just assign a role
- Changing permissions for everyone in a role: update the role once

**Cons:**
- Can become unwieldy with many roles ("role explosion")
- Doesn't handle resource ownership naturally — "editors can edit, but only their own articles" requires extra logic
- Binary: you either have the role or you don't

**When to use**: Most applications. The default starting point. Works great for enterprise SaaS with clear user types.

### Hierarchical RBAC

Roles can inherit from other roles:

```
viewer → editor → admin
```

`editor` inherits all of `viewer`'s permissions plus adds write permissions. `admin` inherits all of `editor`'s permissions plus adds delete/admin permissions.

---

## Model 3: ABAC — Attribute-Based Access Control

Instead of roles, decisions are made based on **attributes** of the user, the resource, and the environment.

```
Policy: "An employee can edit a document if:
  - The document belongs to their department AND
  - Their employment status is 'active' AND
  - The current time is between 9am and 6pm"
```

All of these are attributes:
- **User attributes**: department, employment_status, security_clearance, location
- **Resource attributes**: owner, department, classification, sensitivity
- **Environment attributes**: current time, IP address, request origin

The authorization engine evaluates policies against these attributes at request time.

**Pros:**
- Extremely flexible — handles complex real-world scenarios
- No role explosion — one policy can cover many cases
- Time-based, location-based, and other context-aware decisions

**Cons:**
- Complex to implement and reason about
- Policies can be hard to audit ("why was this denied?")
- Performance can be an issue if many attributes need to be fetched per request

**When to use**: Healthcare (complex HIPAA-level access rules), government/defense (security clearances), enterprise with complex org structures.

**Tools**: Open Policy Agent (OPA), AWS IAM (a form of ABAC), Casbin.

---

## Model 4: ReBAC — Relationship-Based Access Control

Inspired by Google Zanzibar (Google's authorization system). Authorization decisions are based on the **relationship graph** between users and resources.

```
Relationships:
  Alice is owner of Folder A
  Folder A is parent of Document 1
  Bob is viewer of Folder A

Question: Can Bob view Document 1?

Reasoning:
  Bob → viewer of Folder A
  Folder A → parent of Document 1
  viewer of a folder can view its children
  → Yes, Bob can view Document 1
```

Permissions flow through relationship graphs, including inherited permissions through hierarchies.

**Pros:**
- Naturally models real-world relationships (folder/document hierarchy, organization charts)
- Scales to billions of relationships (Zanzibar handles Google Docs at Google scale)
- "Who can access this document?" and "What can this user access?" both solvable

**Cons:**
- More complex to implement than RBAC
- Requires careful schema design
- Graph traversal can be complex to optimize

**When to use**: Collaborative systems (file sharing, project management), anything with deep hierarchies or sharing chains.

**Tools**: Google Zanzibar, SpiceDB, Permify, OpenFGA (open source Zanzibar).

---

## Practical implementation patterns

### Centralized vs Distributed authorization

**Centralized**: There's a single authorization service that all other services call.

```
User request → API → Auth Service: "Can userId 7 delete article 42?" → Yes/No → API responds
```

Pros: single source of truth, easy to audit, consistent
Cons: latency (extra network hop), single point of failure

**Distributed**: Each service makes authorization decisions locally, based on claims in the access token.

```
User request + JWT → API → API reads claims from JWT → makes local decision
```

Pros: fast (no network hop), resilient
Cons: permissions only as current as the JWT claims (stale if roles change), harder to revoke

**Hybrid**: Coarse authorization from JWT claims (is this user authenticated? what's their org?), fine-grained authorization from a centralized service or local DB.

---

## Implementing RBAC in a real API

### Store roles in your database

```sql
CREATE TABLE users (
  id SERIAL PRIMARY KEY,
  email TEXT
);

CREATE TABLE roles (
  id SERIAL PRIMARY KEY,
  name TEXT  -- 'admin', 'editor', 'viewer'
);

CREATE TABLE user_roles (
  user_id INT REFERENCES users(id),
  role_id INT REFERENCES roles(id),
  PRIMARY KEY (user_id, role_id)
);

CREATE TABLE role_permissions (
  role_id INT REFERENCES roles(id),
  permission TEXT,  -- 'articles:delete', 'articles:write', etc.
  PRIMARY KEY (role_id, permission)
);
```

### Include roles in JWT

```json
{
  "sub": "42",
  "roles": ["editor"],
  "permissions": ["articles:read", "articles:write"]
}
```

Putting permissions in the JWT allows fast, stateless authorization checks — but they're stale until the JWT expires. Don't put permissions in long-lived tokens.

### Middleware check (Express/Node example)

```javascript
function requirePermission(permission) {
  return (req, res, next) => {
    const user = req.user; // set by auth middleware after JWT verification
    if (!user.permissions.includes(permission)) {
      return res.status(403).json({ error: "Forbidden" });
    }
    next();
  };
}

// Route definition
app.delete("/articles/:id",
  authenticate,                         // verify JWT
  requirePermission("articles:delete"),  // check permission
  deleteArticleHandler
);
```

---

## Resource ownership checks

Role-based checks are coarse: "editors can edit articles." But what about "editors can only edit *their own* articles"?

This requires a resource-level check:

```javascript
async function deleteArticle(req, res) {
  const article = await db.articles.findById(req.params.id);

  if (!article) {
    return res.status(404).json({ error: "Not found" });
  }

  // Admins can delete anything; editors can only delete their own
  const canDelete =
    req.user.roles.includes("admin") ||
    (req.user.roles.includes("editor") && article.authorId === req.user.id);

  if (!canDelete) {
    return res.status(403).json({ error: "Forbidden" });
  }

  await db.articles.delete(article.id);
  res.status(204).send();
}
```

This is sometimes called **ownership-based authorization** or combined RBAC + ownership.

---

## Always check authorization on the backend

**The cardinal rule**: never rely on client-side authorization checks for security. The UI might hide an "admin" button from non-admins, but an attacker can call the API directly.

Every sensitive operation must be authorized on the server, every time, regardless of what the UI does.

```
Client hides "Delete" button for non-admins → security theater
API checks authorization on DELETE /articles/:id → actual security
```

---

## Returning the right error

```
401 Unauthorized — not authenticated (missing or invalid token)
403 Forbidden     — authenticated, but not authorized for this action
404 Not Found     — sometimes used instead of 403 to avoid leaking
                    that a resource exists (e.g., "you can't access
                    this file" vs "this file doesn't exist")
```

The choice between 403 and 404 for "resource exists but you can't see it" depends on whether knowing the resource exists is itself sensitive information.

---

## Common authorization mistakes

1. **Authorization only in the UI** — users call APIs directly
2. **Checking roles but not ownership** — any editor can edit any article
3. **Trusting the client-supplied user ID** — always get user ID from verified token, not request body
4. **Mass assignment** — user updates `{role: "admin"}` in their own profile update request; make sure you only allow users to update fields they're authorized to change
5. **IDOR — Insecure Direct Object Reference** — user changes their own ID to someone else's in `GET /users/42` → gets someone else's data. Always verify the requesting user is authorized for the specific resource ID.
6. **Forgetting authorization on batch operations** — checking `GET /articles/42` but not `POST /articles/bulk-delete`

---

## Next

With the core auth concepts covered, let's look at the attacks you're defending against: `08-common-attacks.md`
