# Red Hat System Administration II – Study Notes

## Table of Contents
1. [Network Interfaces](#1-network-interfaces)
2. [Hostname Resolution](#2-hostname-resolution)
3. [Network Configuration](#3-network-configuration)
4. [Network Troubleshooting](#4-network-troubleshooting)
5. [File Permissions & Special Bits](#5-file-permissions--special-bits)
6. [Access Control Lists (ACL)](#6-access-control-lists-acl)
7. [OpenSSH Server](#7-openssh-server)

---

## 1. Network Interfaces

### Common Interface Names

| Interface | Type |
|-----------|------|
| `eth0`, `eth1` | Wired Ethernet |
| `wlan0` | Wireless (Wi-Fi) |
| `virbr0` | Virtual Bridge (KVM/libvirt internal network) |
| `lo` | Loopback (127.0.0.1) |

> **Modern naming:** RHEL 7+ uses predictable names like `enp3s0` (PCI bus 3, slot 0) instead of `eth0`. The old naming is still common in exam environments.

### MAC (Hardware) Address
- Also called **Ethernet address** or **hardware address**
- Globally unique 48-bit identifier assigned to a network interface card (NIC)
- Format: `52:54:00:XX:XX:XX`

### Useful Commands

```bash
ifconfig                   # Legacy (net-tools package)
ip addr show               # Modern equivalent
ip link show               # Show link-layer info (MAC, state)
/sbin/ip                   # Full path if not in $PATH
```

---

## 2. Hostname Resolution

### Viewing & Setting the Hostname

```bash
hostname                   # Display current hostname (resets on reboot)
hostnamectl set-hostname myserver.example.com   # Permanent change (RHEL 7+)
```

### Resolution Order

Resolution is controlled by `/etc/nsswitch.conf` (line: `hosts: files dns`):

1. `/etc/hosts` – static local mappings (checked first)
2. DNS servers listed in `/etc/resolv.conf`

### Key Files

| File | Purpose |
|------|---------|
| `/etc/hosts` | Static hostname-to-IP mappings |
| `/etc/resolv.conf` | DNS server addresses (`nameserver 8.8.8.8`) |
| `/etc/hostname` | Persistent hostname |
| `/etc/nsswitch.conf` | Controls resolution order |

```
# /etc/hosts example
127.0.0.1   localhost
192.168.1.10  myserver.example.com  myserver
```

---

## 3. Network Configuration

### NetworkManager (Recommended)

- GUI: `nm-connection-editor` or GNOME Network Settings
- TUI: `nmtui`
- CLI: `nmcli`

```bash
nmcli connection show               # List connections
nmcli device status                 # Show device states
nmcli connection reload             # Reload config files
```

### Interface Config Files

Location: `/etc/sysconfig/network-scripts/ifcfg-<interface>`

```ini
# /etc/sysconfig/network-scripts/ifcfg-eth0
DEVICE=eth0
ONBOOT=yes
HWADDR=52:54:00:AB:CD:EF
MODE=Managed
BOOTPROTO=dhcp          # or: static
# For static, also add:
# IPADDR=192.168.1.100
# NETMASK=255.255.255.0
# GATEWAY=192.168.1.1
# DNS1=8.8.8.8
```

> **Note:** `BOOTPROTO=none` or `BOOTPROTO=static` are equivalent for static configuration.

### Applying Changes

```bash
nmcli connection down eth0 && nmcli connection up eth0
# or
systemctl restart NetworkManager
```

---

## 4. Network Troubleshooting

Work through these layers **in order**:

### Layer 1 – IP Address & Subnet Mask

```bash
ip addr show                      # Check assigned IP
ping 127.0.0.1                    # Test loopback
ping <own-ip>                     # Test local NIC
```

**Fix:** Edit `/etc/sysconfig/network-scripts/ifcfg-<iface>` then reload.

### Layer 2 – Routing / Default Gateway

```bash
ip route show                     # Check routing table (look for 'default via')
traceroute <destination>          # Trace path to host
```

**Fix:** Add/correct `GATEWAY=` in the interface config file.

### Layer 3 – Name Resolution

```bash
host google.com                   # DNS lookup test
dig google.com                    # Detailed DNS query
cat /etc/resolv.conf              # Check DNS servers
cat /etc/hosts                    # Check static entries
```

**Fix:** Add `DNS1=` and `DNS2=` in the interface config, or edit `/etc/resolv.conf` directly.

### Quick Reference Table

| Problem | Test Command | Config Location |
|---------|-------------|-----------------|
| No IP / wrong IP | `ip addr show` | `ifcfg-<iface>` |
| Can't reach gateway | `traceroute <gw>` | `ifcfg-<iface>` (GATEWAY=) |
| DNS fails | `host <name>` | `/etc/resolv.conf` or `ifcfg-<iface>` (DNS1=) |

---

## 5. File Permissions & Special Bits

### Why Can Users Change Their Own Password?

The `passwd` command modifies `/etc/shadow`, which is only readable/writable by root. This works because `passwd` has the **SUID (Set User ID) bit** set:

```bash
ls -l /usr/bin/passwd
# -rwsr-xr-x. 1 root root ... /usr/bin/passwd
#    ^-- 's' = SUID bit
```

When a normal user runs `passwd`, it **temporarily executes with root's effective UID**, allowing it to write to `/etc/shadow`.

### Special Permission Bits

Visible in `ls -l` output in the execute position:

| Symbol | Name | On File | On Directory |
|--------|------|---------|--------------|
| `s` (where x was) | SUID / SGID | Runs as owner/group | New files inherit group |
| `S` (where x was) | SUID/SGID, no execute | Same effect, but file is not executable | Same, not searchable |
| `t` (on others x) | Sticky Bit | No effect | Only owner can delete their files |

```bash
chmod u+s file        # Set SUID
chmod g+s dir         # Set SGID on directory
chmod +t /tmp         # Set sticky bit
# Octal: 4=SUID, 2=SGID, 1=sticky  →  chmod 1755 /tmp
```

---

## 6. Access Control Lists (ACL)

### What Are ACLs?

ACLs extend standard Unix permissions to grant access to **specific users or groups** beyond the owner/group/other model.

### Prerequisites

The filesystem must be mounted with ACL support:

```bash
mount -o remount,acl /home
# Or add 'acl' to options in /etc/fstab for persistence
```

> **Note:** On ext4/XFS (common in RHEL), ACLs are often enabled by default.

### Commands

```bash
getfacl file                          # View ACL entries
setfacl -m u:alice:rw file            # Grant alice read+write
setfacl -m g:devs:rx dir             # Grant devs group read+execute
setfacl -m m::r file                  # Set mask (limits effective permissions)
setfacl -x u:alice file               # Remove alice's ACL entry
setfacl -b file                       # Remove all ACL entries
setfacl -R -m u:alice:rw dir/         # Recursive
```

### ACL Mask

The **mask** entry defines the maximum effective permissions for named users/groups (not the owner or other):

```bash
getfacl file
# user::rw-
# user:alice:rwx    #effective:rw-   ← masked!
# group::r--
# mask::rw-
# other::r--
```

---

## 7. OpenSSH Server

### What Is OpenSSH?

OpenSSH provides encrypted remote access using the SSH protocol. It replaces insecure tools like `telnet` and `rsh`.

- **Server daemon:** `sshd` (listens on TCP port 22)
- **Client tools:** `ssh`, `scp`, `sftp`, `ssh-keygen`, `ssh-copy-id`

### Basic Usage

```bash
ssh user@hostname                    # Remote shell
ssh -X user@hostname                 # With X11 forwarding (run GUI apps remotely)
scp localfile user@host:/remote/path # Copy local → remote
scp user@host:/remote/path localfile # Copy remote → local
rsync user@host:/remote/path local/  # Sync remote → local (efficient, incremental)
```

### SSH Key Authentication

**1. Generate a key pair:**
```bash
ssh-keygen -t rsa -b 4096
# Private key: ~/.ssh/id_rsa      (NEVER share this)
# Public key:  ~/.ssh/id_rsa.pub  (place on remote hosts)
```

**2. Copy public key to remote host:**
```bash
ssh-copy-id -i ~/.ssh/id_rsa.pub username@remotehost
# Appends key to ~/.ssh/authorized_keys on the remote host
```

**3. Test:**
```bash
ssh username@remotehost    # Should not prompt for password
```

### Server Configuration: `/etc/ssh/sshd_config`

**Always back up before editing:**
```bash
cp /etc/ssh/sshd_config /etc/ssh/sshd_config.original
chmod a-w /etc/ssh/sshd_config.original   # Make backup read-only
```

**Key directives:**

```ini
PermitRootLogin no              # Disable direct root login (best practice)
PasswordAuthentication no       # Force key-based auth only
PubkeyAuthentication yes        # Enable key auth
Banner /etc/issue.net           # Display banner before login
Port 22                         # Default port (change to reduce noise)
AllowUsers alice bob            # Whitelist specific users
```

**Reload after changes:**
```bash
systemctl reload sshd
```

### Security Best Practices

- Disable root login (`PermitRootLogin no`)
- Use key-based authentication instead of passwords
- Set a login banner (`Banner /etc/issue.net`)
- Restrict allowed users with `AllowUsers` or `AllowGroups`
- Consider changing the default port (22) in high-exposure environments

---

*These notes cover core RHSA II topics. Always consult the official Red Hat documentation for exam preparation.*