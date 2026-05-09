# Database

Storing data on disk (EFS, EBS, Instance Store, S3) can have its limits

Database allow you to
- build indexes to efficiently query/search through the data
- define relationships between data

---

## Relational Databases


## NoSQL Databaess

- Key-value
- Document
- Graph
- In-memory
- Search

## RDS

- Relational Database Service
- It's a managed DB service for DB use SQL as query language
- Allows you to create dbs in the cloud managed by AWS:
  - Postgres, MySQL, MariaDB, Oracle, Microsoft SQL Server, IBM DB2, Aurora

RDS vs deploying db on EC2
- RDS is managed service
- automated provisioning, OS patching
- continuous backups and restore to specific timestamp
- monitoring dashboards
- read replicas for improved read performance
- multi AZ setup for DR (disaster recovery)
- maintenance windows for upgrades
- scaling capability
- storaged backed by EBS
- CAN'T SSH into RDS

### Amazon Aurora

- AWS proprietary (not open source)
- PostgreSQL & MySQL are supported as Aurora DB
- Aurora is cloud optimized and claims 5x MySQL, 3x PostgreSQL
- Aurora storage automtically grows in increments of 10GB up to 256TB
- Aurora costs 20% more than RDS but is more efficient

#### Amazon Aurora Serverless

- Automated db instantiation & auto-scaling based on actual usage
- No capacity planning needed
- Least management overhead
- Pay per second, can be more cost-effective
- Use cases: infrequent intermittent unpredictable workloads
