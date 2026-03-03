# Amazon Elastic Compute Cloud (EC2)
> AWS Cloud Practitioner Exam Study Guide

---

## Table of Contents
1. [AWS Budget Setup](#1-aws-budget-setup)
2. [EC2 Basics](#2-ec2-basics)
3. [EC2 Instance Types](#3-ec2-instance-types)
4. [Security Groups](#4-security-groups)
5. [User Data](#5-user-data)
6. [EC2 Instance Roles](#6-ec2-instance-roles)
7. [SSH](#7-ssh)
8. [EC2 Instance Purchase Options](#8-ec2-instance-purchase-options)
9. [Shared Responsibility Model for EC2](#9-shared-responsibility-model-for-ec2)
10. [Quick Reference Cheat Sheet](#10-quick-reference-cheat-sheet)

---

## 1. AWS Budget Setup

Setting up billing alerts prevents unexpected charges during exam practice.

**Steps:**
1. AWS Console → Billing & Cost Management → Budgets
2. Click **Create Budget** → Cost Budget
3. Set a monthly amount (e.g., $10)
4. Add an email alert at 80% and 100% threshold

> ⚠️ **Exam Tip:** The Free Tier covers 750 hrs/month of `t2.micro` or `t3.micro`. Always stop or terminate instances when done to avoid charges.

---

## 2. EC2 Basics

Amazon EC2 (Elastic Compute Cloud) provides resizable virtual machines in the cloud called **instances**. It is an **IaaS** (Infrastructure as a Service) — you manage the OS and above; AWS manages the underlying hardware.

**Key Concepts:**
- **AMI (Amazon Machine Image):** Template containing OS, software, and config used to launch instances
- **Instance:** A running virtual server launched from an AMI
- **Region & Availability Zone:** EC2 instances run in a specific AZ within a Region
- **Elastic IP:** A static public IPv4 address you can attach to an instance
- **Key Pair:** Public/private key used for SSH authentication
- **User Data:** Scripts that run on first boot to automate configuration
- **Instance Metadata:** Info about the running instance, accessible at `http://169.254.169.254/latest/meta-data/`

> 📝 **Exam Tip:** EC2 is **IaaS**. You are responsible for the OS, applications, and data. AWS manages the physical hardware and hypervisor.

---

## 3. EC2 Instance Types

AWS instance naming follows the pattern: `[Family][Generation][Attributes].[Size]` — e.g., `m5.xlarge`

| Family | Examples | Characteristic | Use Case |
|---|---|---|---|
| General Purpose | t2, t3, m5, m6 | Balanced CPU, RAM, network | Web servers, code repos, small DBs |
| Compute Optimized | c5, c6g | High CPU-to-RAM ratio | Batch jobs, HPC, ML inference, gaming |
| Memory Optimized | r5, r6g, x1e | Large RAM | In-memory DBs (Redis), real-time analytics |
| Storage Optimized | i3, d2, h1 | High I/O, NVMe SSDs | OLTP, NoSQL, data warehousing, Hadoop |
| Accelerated Computing | p4, g5, inf1 | GPU / custom hardware | ML training, video rendering, HPC |

> 📝 **Exam Tip:** Remember the families — **G**eneral, **C**ompute, **M**emory, **S**torage, **A**ccelerated. Match the workload to the family for scenario questions.

---

## 4. Security Groups

Security Groups act as a **virtual firewall** for EC2 instances, controlling inbound and outbound traffic.

**Key Rules:**
- Security groups are **stateful** — if inbound traffic is allowed, the return response is automatically allowed
- By default: **ALL inbound denied**, **ALL outbound allowed**
- Rules can only **ALLOW** traffic — there are no explicit deny rules
- One instance can have multiple security groups; one group can be attached to many instances
- Scoped to a **VPC** — cannot span VPCs
- Changes take effect **immediately**

**Common Ports:**

| Port | Protocol | Use |
|---|---|---|
| 22 | TCP | SSH (Linux) / SFTP |
| 3389 | TCP | RDP (Windows Remote Desktop) |
| 80 | TCP | HTTP |
| 443 | TCP | HTTPS |
| 3306 | TCP | MySQL / Aurora |
| 5432 | TCP | PostgreSQL |

> 📝 **Exam Tip:** **Timeout error** = Security Group issue. **Connection refused** = instance is reachable but the service isn't running.

---

## 5. User Data

User Data lets you automate instance configuration on first launch (**bootstrapping**).

**Key Facts:**
- Runs **only once**, on the first start (not on reboots by default)
- Runs as **root** — no `sudo` needed
- Must begin with `#!/bin/bash` for shell scripts
- Viewable at: `http://169.254.169.254/latest/user-data`

**Example Script:**
```bash
#!/bin/bash
yum update -y
yum install -y httpd
systemctl start httpd
systemctl enable httpd
echo "<h1>Hello from EC2</h1>" > /var/www/html/index.html
```

> 📝 **Exam Tip:** User Data is used for **bootstrapping** — automating software installation and configuration on launch.

---

## 6. EC2 Instance Roles

IAM Roles allow EC2 instances to securely call AWS services **without storing credentials on the instance**.

**Why Use IAM Roles?**
- **Never** hard-code Access Key ID or Secret Access Key on an EC2 instance
- Credentials stored on an instance can be stolen (e.g., from a compromised application)
- IAM Roles provide **temporary, automatically rotated credentials** via the instance metadata service
- Follows the **principle of least privilege** — attach only the permissions the instance needs

**How It Works:**
1. Create an IAM Role with the required policy (e.g., `AmazonS3ReadOnlyAccess`)
2. Attach the IAM Role to your EC2 instance (at launch or afterwards)
3. The instance automatically receives temporary credentials
4. The AWS SDK/CLI on the instance uses these credentials automatically — no config needed

> 🔐 **Exam Tip:** If asked how an EC2 instance should access S3 or DynamoDB — the answer is always **attach an IAM Role**. Never use access keys directly on an instance.

---

## 7. SSH

SSH is one of the most important functions for EC2. It allows you to connect to and manage a remote Linux instance from your terminal.

**SSH Command:**
```bash
ssh -i <path-to-key.pem> ec2-user@<public-ip>
```

**Notes:**
- The `.pem` file must have permissions `400`: `chmod 400 key.pem`
- Default user for **Amazon Linux 2**: `ec2-user`
- Default user for **Ubuntu**: `ubuntu`
- **Windows** instances use RDP (port 3389), not SSH

### SSH Summary Table

| OS | SSH | PuTTY | EC2 Instance Connect |
|---|---|---|---|
| Linux | ✓ | | ✓ |
| Mac | ✓ | | ✓ |
| Windows < 10 | | ✓ | ✓ |
| Windows ≥ 10 | ✓ | ✓ | ✓ |

### SSH Troubleshooting

| Error | Cause & Fix |
|---|---|
| **Connection Timeout** | Security Group missing inbound rule on port 22, or a firewall is blocking the connection. Check your Security Group; use EC2 Instance Connect as fallback. |
| **ssh: command not found** | OpenSSH not installed (Windows < 10). Use PuTTY or EC2 Instance Connect. |
| **Connection Refused** | Instance is reachable but SSH daemon is not running. Restart the instance; if it persists, terminate and recreate using Amazon Linux 2. |
| **Permission denied (publickey)** | Wrong key file or wrong username. Confirm the key pair assigned to the instance and use `ec2-user` for Amazon Linux 2. |
| **Unprotected key file** | Key permissions too open. Run `chmod 400 key.pem`. |

---

## 8. EC2 Instance Purchase Options

Choosing the right purchase option is one of the **most commonly tested topics** on the Cloud Practitioner exam.

| Option | Commitment | Discount | Best For |
|---|---|---|---|
| **On-Demand** | None | None (baseline price) | Short, unpredictable workloads |
| **Reserved (1 & 3 yr)** | 1 or 3 years | Up to 72% | Steady-state / predictable workloads |
| **Savings Plans (1 & 3 yr)** | 1 or 3 years | Up to 72% | Flexible usage across instance families |
| **Spot Instances** | None (interruptible) | Up to 90% | Fault-tolerant batch jobs |
| **Dedicated Host** | On-demand or reserved | — (most expensive) | Compliance, bring-your-own-license (BYOL) |
| **Dedicated Instances** | None | Higher than shared | Hardware not shared with other AWS accounts |
| **Capacity Reservations** | None | On-demand rate | Guarantee capacity in a specific AZ |

**Details:**

- **Spot Instances:** AWS can reclaim the instance with a 2-minute warning. Best for batch processing, data analysis, and flexible start/end jobs. Not suitable for critical workloads.
- **Reserved vs Savings Plans:** Reserved locks in a specific instance type/region/OS. Savings Plans are more flexible — you commit to a $/hr spend, not a specific instance.
- **Convertible Reserved:** Can change instance family/type, but offers a lower discount (~54%).
- **Dedicated Host:** You get an entire physical server. Required for software with server-bound licensing (e.g., Windows Server, SQL Server per-core licensing).

> 📝 **Exam Tip:**
> - *"Steady-state database, lowest 1-year cost"* → **Reserved**
> - *"Flexible workloads across instance types"* → **Savings Plans**
> - *"Fault-tolerant big data job"* → **Spot**
> - *"Compliance / BYOL"* → **Dedicated Host**

---

## 9. Shared Responsibility Model for EC2

AWS and the customer **share** security responsibilities. Understanding the division is critical for the exam.

| AWS Responsibility (Security **OF** the Cloud) | Customer Responsibility (Security **IN** the Cloud) |
|---|---|
| Physical security of data centers | Guest OS patching & updates |
| Hardware infrastructure (servers, networking, storage) | Application software & security |
| Virtualization layer (hypervisor) | Security Group & firewall configuration |
| Managed services patching (RDS OS, Lambda runtime) | IAM users, roles, and permissions |
| | Data encryption (in transit & at rest) |
| | Network configuration (VPC, subnets, routing) |

> 📝 **Exam Tip:** AWS is responsible for everything **below** the hypervisor (hardware, data centers, network infrastructure). You are responsible for everything **above** it (OS, apps, data, IAM).

---

## 10. Quick Reference Cheat Sheet

| Topic | Key Fact |
|---|---|
| EC2 service type | IaaS |
| Default Security Group inbound | All traffic **DENIED** |
| Default Security Group outbound | All traffic **ALLOWED** |
| Security Groups are... | Stateful |
| SSH port | 22 (TCP) |
| RDP port | 3389 (TCP) |
| User Data runs... | Once, at first boot, as root |
| Cheapest purchase option | Spot Instances (up to 90% off) |
| Most expensive purchase option | On-Demand |
| For compliance / BYOL | Dedicated Host |
| Credentials on EC2 | Use IAM Roles — **NEVER** access keys |
| AMI | OS + configuration template for launching instances |
| EC2 Instance Connect | Browser-based SSH — no key file needed |
| Timeout error | Security Group issue |
| Connection refused | SSH daemon not running on the instance |