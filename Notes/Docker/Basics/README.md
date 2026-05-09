# Docker — Complete Concise Guide

---

## Why Docker?

Apps break when environments differ — "works on my machine" is the classic problem.
Docker packages your app **and everything it needs** into one portable unit that runs the same everywhere: dev, staging, production.

---

## Containers vs Virtual Machines

| | Container | Virtual Machine |
|---|---|---|
| **What it is** | Isolated process on the host OS | Full OS running on a hypervisor |
| **Size** | MBs | GBs |
| **Boot time** | Milliseconds | Minutes |
| **Isolation** | Process-level | Full hardware-level |
| **Overhead** | Very low | High |

**Hypervisor** — software that creates and runs VMs by emulating hardware (e.g. VMware, VirtualBox, Hyper-V). Each VM needs its own OS on top.

Containers share the host OS kernel — no OS duplication, so they're lighter and faster.

---

## Docker Architecture

```
Docker Client  ──►  Docker Daemon (Host)
                        ├── Images
                        ├── Containers
                        └── Volumes

                    Registry (Docker Hub)
                        └── Stores & distributes images
```

- **Client** — CLI you type commands into (`docker ...`)
- **Daemon** — background service that does the actual work
- **Image** — read-only blueprint (like a class)
- **Container** — running instance of an image (like an object)
- **Registry** — remote store for images; Docker Hub is the default public one

---

## Images & Dockerfile

An image is built from a `Dockerfile` — a recipe of instructions.

```dockerfile
FROM node:18-alpine        # base image
WORKDIR /app               # set working directory
COPY package*.json ./      # copy dependency files
RUN npm install            # run command during build
COPY . .                   # copy app source
EXPOSE 3000                # document which port the app uses
CMD ["node", "server.js"]  # command to run when container starts
```

### COPY vs ADD

Both copy files into the image. **Prefer `COPY`** — it's explicit and predictable.

Use `ADD` only when you need its extra powers:
- Automatically extracts `.tar` archives into the destination
- Can fetch files from a remote URL

```dockerfile
COPY app.js /app/          # plain copy — use this by default
ADD archive.tar.gz /app/   # copies AND extracts the archive
```

### RUN vs CMD vs ENTRYPOINT

| Instruction | When it runs | Purpose |
|---|---|---|
| `RUN` | **Build time** | Install packages, compile code, set up the image |
| `CMD` | **Run time** | Default command/args when container starts |
| `ENTRYPOINT` | **Run time** | The fixed executable the container always runs |

```dockerfile
RUN npm install                          # executes during docker build

CMD ["node", "server.js"]               # runs when container starts

ENTRYPOINT ["node"]                     # always runs node...
CMD ["server.js"]                       # ...with this default arg
```

**`CMD` vs `ENTRYPOINT` — the key difference:**

- `CMD` is fully replaceable: `docker run myapp index.js` → ignores CMD, runs `index.js`
- `ENTRYPOINT` is not replaced, only appended to: `docker run myapp index.js` → runs `node index.js`
- Use `ENTRYPOINT` when the container has one clear purpose (e.g. always run `node`). Use `CMD` for flexible defaults.

### ENV

Sets environment variables available during build and at runtime:

```dockerfile
ENV NODE_ENV=production
ENV PORT=3000
```

Override at runtime:
```bash
docker run -e NODE_ENV=development myapp
```

### EXPOSE

Documents which port the app listens on — **it does not actually publish the port**. It's informational, like a comment, but tools like Docker Compose and IDE plugins read it. You still need `-p 8080:3000` at runtime to actually expose it to your host.

### Multi-Stage Builds

A technique to keep final images small. You use multiple `FROM` stages — each is a separate environment. Only the last stage ends up in the final image.

**Problem without it:** build tools (compilers, test frameworks, dev dependencies) bloat your production image.

**Solution:** build in one stage, copy only the output to a clean final stage.

```dockerfile
# Stage 1 — build
FROM node:18 AS builder
WORKDIR /app
COPY . .
RUN npm install && npm run build      # produces /app/dist

# Stage 2 — production image (starts fresh, no build tools)
FROM node:18-alpine
WORKDIR /app
COPY --from=builder /app/dist ./dist
COPY package*.json ./
RUN npm install --production
CMD ["node", "dist/server.js"]
```

The final image only contains the alpine base + production deps + compiled output. Build tools never make it in.

**alpine** — a minimal Linux distro (~5MB). Use `node:18-alpine` instead of `node:18` to keep images small.

```bash
docker build -t myapp:1.0 .      # build image, tag it
docker images                    # list images
docker rmi myapp:1.0             # delete image
```

---

## Networking

By default, Docker containers are isolated from each other and the host. Networks control how they communicate.

### Network Types

| Driver | What it does | Use case |
|---|---|---|
| `bridge` | Private internal network on the host (default) | Most containers; isolated communication |
| `host` | Container shares the host's network stack directly | Performance-sensitive apps; removes network isolation |
| `none` | No networking at all | Fully isolated containers |

### Bridge (default)

When you run containers without specifying a network, they join the default bridge. Containers can reach each other by IP, but **not by name** on the default bridge.

**User-defined bridge networks** are better — containers can resolve each other by service name:

```bash
docker network create mynet

docker run --network=mynet --name web myapp
docker run --network=mynet --name db postgres

# Inside 'web', you can now reach the db as:
# postgres://db:5432
```

### Host Network

Container bypasses Docker's virtual network and uses your machine's network directly:

```bash
docker run --network=host nginx
# nginx now listens on your host's port 80 directly — no -p needed
```

Faster, but removes isolation — the container can see all host interfaces. Linux only (not supported on Mac/Windows Docker Desktop).

### None

```bash
docker run --network=none myapp    # no network interfaces at all
```

### Key Commands

```bash
docker network ls                         # list networks
docker network create mynet               # create a user-defined bridge
docker network inspect mynet              # details (connected containers, IPs)
docker network connect mynet <container>  # attach a running container
docker network disconnect mynet <container>
```

### Gateway & Bridge Internals

When Docker creates a bridge network, it sets up a virtual ethernet bridge (`docker0` by default) on the host. Each container gets a virtual ethernet pair — one end inside the container, one end on the bridge. The **gateway** is the bridge's IP (typically `172.17.0.1`) — it's how containers route traffic out to the internet.

You can see this with:
```bash
docker network inspect bridge
# Look for "Gateway" under IPAM config
```

---

## Volumes

By default, data inside a container is **lost when it stops**. Volumes solve this.

**What is a volume?**
A volume is a folder managed by Docker that lives *outside* the container filesystem. Data written there persists even if the container is deleted.

### Three Types

| Type | Syntax | When to use |
|---|---|---|
| Named volume | `mydata:/app/data` | Databases, persistent app data |
| Bind mount | `/host/path:/container/path` | Dev — live code syncing |
| tmpfs | `--tmpfs /tmp` | Sensitive/temp data, RAM only, never written to disk |

```bash
docker volume create mydata          # create a named volume
docker volume ls                     # list volumes
docker volume inspect mydata         # details
docker volume rm mydata              # delete

# Run with a named volume
docker run -v mydata:/var/lib/mysql mysql

# Run with a bind mount (dev workflow)
docker run -v $(pwd):/app node:18 node server.js
```

Volumes are the correct way to share data between containers too — mount the same volume in two containers.

---

## Key Commands

### Running Containers

```bash
docker run nginx                          # run a container
docker run -d nginx                       # detached (background)
docker run -p 8080:80 nginx               # map host:container port
docker run -e NODE_ENV=production myapp   # set environment variable
docker run --name mycontainer nginx       # give it a name
docker run --rm nginx                     # auto-delete when stopped

# Resource limits
docker run --memory=100m --cpus=2 nginx
```

### Managing Containers

```bash
docker ps                    # running containers
docker ps -a                 # all containers (including stopped)
docker stop <id>             # graceful stop
docker kill <id>             # force stop
docker rm <id>               # delete container
docker container prune       # delete ALL stopped containers

# Bulk delete all stopped containers
docker rm $(docker ps -a -q)
# $(docker ps -a -q) is shell command substitution — runs inner command first,
# then passes its output as arguments to docker rm
```

### Inspecting & Debugging

```bash
docker logs <id>                  # view container logs
docker logs -f <id>               # follow logs (live)
docker container inspect <id>     # full JSON details (IP, mounts, config...)
docker exec -it <id> bash         # open a shell inside running container
docker exec -it <id> sh           # use sh if bash isn't available (e.g. alpine)

# Inside the container you can run normal Linux commands:
service nginx status
cat /etc/nginx/nginx.conf
```

`-it` = interactive + TTY (gives you a real terminal session)

---

## Docker Compose

For multi-container apps, use `docker-compose.yml` instead of long `docker run` commands.

```yaml
version: "3"
services:
  web:
    build: .
    ports:
      - "3000:3000"
    volumes:
      - .:/app
    environment:
      - NODE_ENV=development
    depends_on:
      - db

  db:
    image: postgres:15
    volumes:
      - pgdata:/var/lib/postgresql/data
    environment:
      - POSTGRES_PASSWORD=secret

volumes:
  pgdata:      # named volume — persists data across restarts
```

```bash
docker compose up -d      # start all services
docker compose down       # stop and remove containers
docker compose logs -f    # follow all logs
```

In Compose, services are automatically placed on a shared user-defined bridge network and can reach each other by service name (e.g. `web` can connect to `db:5432`).

---

## Quick Reference

```bash
# Full workflow
docker build -t myapp .
docker run -d -p 8080:80 --name myapp myapp

# Clean up everything
docker container prune      # stopped containers
docker image prune          # dangling images
docker system prune -a      # everything unused
```

---

## Cheat Sheet Summary

| Concept | In plain words |
|---|---|
| Image | Snapshot/blueprint of an app |
| Container | A running image |
| Volume | Persistent storage outside the container |
| Bind mount | Mount a host folder into a container |
| tmpfs | In-memory storage — gone when container stops |
| Registry | Remote library of images |
| Dockerfile | Script to build an image |
| `RUN` | Executes at build time (installs deps, compiles) |
| `CMD` | Default command at run time — overridable |
| `ENTRYPOINT` | Fixed executable at run time — args are appended |
| `ENV` | Set environment variables in the image |
| `EXPOSE` | Documents a port — doesn't publish it |
| Multi-stage build | Use multiple FROM stages; final image only gets what you copy in |
| Bridge network | Default isolated network; user-defined bridges support DNS by name |
| Host network | Container shares host network stack directly |
| Compose | Tool to run multi-container apps |
| alpine | Tiny Linux base image — keeps containers lean |