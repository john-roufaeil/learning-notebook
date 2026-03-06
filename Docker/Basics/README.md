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

**alpine** — a minimal Linux distro (~5MB). Use `node:18-alpine` instead of `node:18` to keep images small.

```bash
docker build -t myapp:1.0 .      # build image, tag it
docker images                    # list images
docker rmi myapp:1.0             # delete image
```

---

## Volumes

By default, data inside a container is **lost when it stops**. Volumes solve this.

**What is a volume?**
A volume is a folder managed by Docker that lives *outside* the container filesystem. Data written there persists even if the container is deleted.

**Three types:**

| Type | Syntax | When to use |
|---|---|---|
| Named volume | `mydata:/app/data` | Databases, persistent app data |
| Bind mount | `/host/path:/container/path` | Dev — live code syncing |
| tmpfs | `--tmpfs /tmp` | Sensitive/temp data, RAM only |

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
# $(docker ps -a -q) is a shell command substitution — not Docker-specific.
# It runs the inner command first, then passes its output as arguments.
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
| Registry | Remote library of images |
| Dockerfile | Script to build an image |
| Compose | Tool to run multi-container apps |
| alpine | Tiny Linux base image — keeps containers lean |