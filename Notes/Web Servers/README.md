# Web Servers

A computer that serves content (HTML, images, files) over HTTP/HTTPS.

---

## Fundamentals

### IP Address
Unique numerical label for every device on a network. e.g. `192.168.1.1`

### Port
Virtual "door" on a computer. One IP, many ports.
- `80` → HTTP
- `443` → HTTPS
- `22` → SSH
- `3306` → MySQL

### How a Website Works
1. User types URL
2. Browser asks DNS for the IP
3. Browser sends HTTP request to that IP
4. Server returns response
5. Browser renders HTML/CSS/JS

### DNS & Domains
**DNS** — Translates `google.com` → `142.250.80.46`. Phone book for the internet.  
**Domain** — Human-readable name purchased from a registrar (Namecheap, GoDaddy).

```
User → google.com → DNS → 142.250.80.46 → Server → Response
```

### HTTP vs HTTPS
| | HTTP | HTTPS |
|---|---|---|
| Port | 80 | 443 |
| Encrypted | ❌ | ✅ TLS/SSL |
| SEO | Penalised | Preferred |

### HTTP Status Codes
| Code | Meaning |
|---|---|
| `200` | OK |
| `201` | Created |
| `301` | Moved Permanently |
| `302` | Temporary Redirect |
| `400` | Bad Request |
| `401` | Unauthorized |
| `403` | Forbidden |
| `404` | Not Found |
| `500` | Internal Server Error |
| `502` | Bad Gateway |
| `503` | Service Unavailable |

### GET vs POST
| | GET | POST |
|---|---|---|
| Data | URL params | Request body |
| Cached | ✅ | ❌ |
| Use | Fetch | Submit |

```
GET /search?q=cats HTTP/1.1
Host: google.com
```
```
POST /login HTTP/1.1
Content-Type: application/json

{ "username": "john", "password": "secret" }
```

### What is a Web Server?
Software that listens on a port, accepts HTTP requests, returns responses.

```
Code → Server (Apache/Nginx) → HTTP/HTTPS → Browser
```

### Performance
- **Latency** — Time to first byte
- **Throughput** — Requests/second
- **Concurrency** — Simultaneous connections
- Improved via: caching, gzip, CDN, load balancing

---

## Apache

### Pros
- Free, open source (including commercial)
- Stable, battle-tested since 1995
- Module-based — extend with `mod_rewrite`, `mod_ssl`, etc.
- Beginner-friendly
- Cross-platform
- Native `.htaccess` support

### Cons
- High memory usage under heavy traffic (process-per-connection)
- Misconfigured options can introduce vulnerabilities

---

### Configuration

Plain text files with **directives** — keyword + arguments.

#### Config File Locations
| OS | Path |
|---|---|
| Ubuntu/Debian | `/etc/apache2/apache2.conf` |
| RedHat/CentOS | `/etc/httpd/conf/httpd.conf` |

Split into smaller files loaded via:
```apache
Include conf/extra/httpd-vhosts.conf
```

---

#### Core Directives

```apache
ServerRoot "/etc/apache2"          # base dir for config/logs
ServerName www.example.com         # hostname of your site
DocumentRoot "/var/www/html"       # where website files live
Listen 80                          # port to bind to
Listen 192.168.1.1:8080            # specific IP + port
PidFile /var/run/apache2/apache2.pid
```

---

#### Directive Contexts

| Context | Where |
|---|---|
| **Server config** | `httpd.conf` only |
| **Virtual host** | Inside `<VirtualHost>` |
| **Directory** | Inside `<Directory>`, `<Location>`, `<Files>` |
| **.htaccess** | Per-directory files (if `AllowOverride` permits) |

---

#### Virtual Hosts

Host multiple sites on one server.

```apache
<VirtualHost *:80>
    ServerName example.com
    ServerAlias www.example.com
    DocumentRoot /var/www/example
    ErrorLog ${APACHE_LOG_DIR}/example_error.log
    CustomLog ${APACHE_LOG_DIR}/example_access.log combined
</VirtualHost>
```

```bash
sudo a2ensite example.conf
sudo systemctl reload apache2
```

---

#### Directory Blocks & Access Control

```apache
<Directory "/var/www/html">
    Options -Indexes FollowSymLinks
    AllowOverride All
    Require all granted
</Directory>
```

**`Options`**
- `Indexes` — show dir listing (disable with `-Indexes`)
- `FollowSymLinks` — allow symlinks
- `ExecCGI` — allow CGI scripts

**`AllowOverride`**
```apache
AllowOverride None        # .htaccess ignored
AllowOverride All         # .htaccess overrides anything
AllowOverride AuthConfig  # auth directives only
```

**`Require`**
```apache
Require all granted
Require all denied
Require ip 192.168.1.0/24
Require host example.com
```

---

#### `.htaccess`

Per-directory config — affects directory + subdirectories.  
Only active if `AllowOverride` is not `None`.

> ⚠️ Apache reads `.htaccess` on every request — prefer main config when possible.

```apache
Options -Indexes
ErrorDocument 404 /404.html
Redirect 301 /old /new

<Files "config.php">
    Require all denied
</Files>
```

---

#### `htpasswd` — Basic Auth

```bash
htpasswd -c /etc/apache2/.htpasswd alice   # create file + user
htpasswd /etc/apache2/.htpasswd bob        # add more users
```

```apache
AuthType Basic
AuthName "Restricted Area"
AuthUserFile /etc/apache2/.htpasswd
Require valid-user
```

---

#### Modules

```bash
sudo a2enmod rewrite
sudo a2enmod ssl
sudo a2dismod autoindex
sudo systemctl restart apache2
```

| Module | Purpose |
|---|---|
| `mod_rewrite` | URL rewriting |
| `mod_ssl` | HTTPS/TLS |
| `mod_headers` | HTTP headers |
| `mod_deflate` | Gzip compression |
| `mod_cache` | Caching |
| `mod_proxy` | Reverse proxy |
| `mod_autoindex` | Directory listing |

---

#### URL Rewriting

```apache
<IfModule mod_rewrite.c>
    RewriteEngine On

    # HTTP → HTTPS
    RewriteCond %{HTTPS} off
    RewriteRule ^(.*)$ https://%{HTTP_HOST}%{REQUEST_URI} [L,R=301]

    # Pretty URLs
    RewriteCond %{REQUEST_FILENAME} !-f
    RewriteCond %{REQUEST_FILENAME} !-d
    RewriteRule ^(.*)$ index.php?url=$1 [QSA,L]
</IfModule>
```

---

#### SSL / HTTPS

```bash
sudo a2enmod ssl
sudo a2ensite default-ssl
```

```apache
<VirtualHost *:443>
    ServerName example.com
    DocumentRoot /var/www/html
    SSLEngine on
    SSLCertificateFile      /etc/ssl/certs/example.crt
    SSLCertificateKeyFile   /etc/ssl/private/example.key
    SSLCertificateChainFile /etc/ssl/certs/chain.pem
</VirtualHost>
```

Free SSL via Let's Encrypt:
```bash
sudo apt install certbot python3-certbot-apache
sudo certbot --apache -d example.com
```

---

#### Users & Permissions

Apache runs as `www-data` (Ubuntu) or `apache` (CentOS).

```bash
sudo chown -R www-data:www-data /var/www/html
sudo find /var/www/html -type d -exec chmod 755 {} \;
sudo find /var/www/html -type f -exec chmod 644 {} \;
```

---

#### Logging

```apache
ErrorLog ${APACHE_LOG_DIR}/error.log
CustomLog ${APACHE_LOG_DIR}/access.log combined
LogLevel warn
```

```bash
tail -f /var/log/apache2/error.log
tail -f /var/log/apache2/access.log
```

---

#### Commands

```bash
sudo systemctl start apache2
sudo systemctl stop apache2
sudo systemctl restart apache2
sudo systemctl reload apache2     # graceful, no downtime
sudo apachectl configtest         # test config → should say: Syntax OK
sudo systemctl status apache2
```

---

#### Directory Structure

```
/etc/apache2/
├── apache2.conf
├── ports.conf
├── sites-available/
│   └── example.conf
├── sites-enabled/        # symlinks to active sites
├── mods-available/
└── mods-enabled/         # symlinks to active modules

/var/www/html/            # default DocumentRoot
/var/log/apache2/
├── access.log
└── error.log
```

---

## Nginx