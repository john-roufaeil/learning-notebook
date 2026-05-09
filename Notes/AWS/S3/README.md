# Amazon S3

Simple Storage Service
Amazon S3 is one of the main building blocks of AWS

- it's advertised as "infinitely scaling" storage
- many websites use Amazon S3
- many AWS services use Amazon S3 as integration

Use Cases

- Backup & storage
- Disaster recovery
- Archive
- Hybrid cloud storage
- application hosting
- media hosting
- data lakes & big data analytics
- software delivery
- static website

## Amazon S3 - Storage

### Buckets

- Store objects (files) in buckets (directories)
- Buckets must have a globally unique name (across all regions & accs)
- Buckets are defined at region level
- Naming convention: no uppercase/underscore, 3-63 characters, not IP, start with lowercase/number, not start with xn--, not end with -s3alias

### Objects

- Files have a key (FULL path: s3://my-bucket/my_file.txt)
- There's no concept of directories within buckets
- Object values are the content of the body:
  - Max. Object size = 5TB (5000GB)
  - If uploading more than 5GB, must use "multi-part upload"
- Metadata (list of text key/value pairs - system or user metadata)
- Tags (unicode key/ value pair - up to 10) - useful for security/lifecycle
- Version ID (if versioning is enabled)

---

## Amazon S3 - Security

- User-based
  - IAM Policies - which API calls should be allowed for a specific user from IAM
- Resource based
  - Bucket Policies - bucket wide rules from the S3 console - allows cross account
  - Object Access Control List (ACL) - finer grain can be disabled
  - Bucket Access Control List (ACL) - less common can be disabled
- Encryption: encrypt objects in Amazno S3 using encryption keys

Note: an IAM Principal can access an S3 Object if:
- The user IAM persmissions ALLOW it OR the resource policy ALLOWS it AND there's no explicit DENY

### JSON-Based Policies

- Version
- Statement
  - Sid
  - Effect: Allow/Deny
  - Principal: The account or user to apply the policy to
  - Action: Set of API to Allow or Deny
  - Resource: buckets and objects

Use S3 bucket for policy to:
- Grant public access to bucket
- Force objects to be encrypted at upload
- Grant access to another account (cross account)

Connecter to S3 bucket could be:
- Public User: Bucket policy
- Cross-acc Access: Bucket policy
- IAM User: check IAM policy
- Resource (EC2): IAM role

---

## Amazon S3 - Static Website Hosting

- S3 can host static websites and have them accessible on the internet
- The website URL will be:
  - http://bucket-name.s3-website-aws-region.amazonaws.com
  - http://bucket-name.s3-website.aws-region.amazonaws.com
  
## Amazon S3 - Versioning

- You can version your files in Amazon S3
- It is enabled at the bucket level
- Same key overwrite will change the version: 1,2,3...
- It is best practice to version your buckets
  - protect against unintended deletes
  - easy rollback to previous version
- Any file that is not versioned prior to enabling versioning will have version "null"
- Suspending versioning doesn't delete previous versions

## Amazon S3 - Replication (CRR & SRR)

CRR - cross region replication
SRR - same region replication

- Must enable Versioning in source and destination buckets
- Buckets can be in different AWS accounts
- Copying is ansynchronous: happens in background 
- Must give proper IAM permissions to S3
- Use cases:
  - CRR - compliance, lower latency access, replication across acounts
  - SRR - log aggregation, live replication between productionand test accounts

---

## Amazon S3 Storage Classes

1. Standard - General Purpose
2. Standard - Infrequent Access (IA)
3. One Zone - Infrequent Access
4. Glacier Instant Retrieval
5. Glacier Flexible Retrieval
6. Glacier Deep Archive
7. Intelligent Tiering

When creating an object in S3, you can choose its class or modify its class manually or use S3 lifecycle configurations to move automatically between classes.

*Durability*:
  - How many times an item is going to be lost by Amazon S3. Extremely high durability 99.999999999%
  - If you store 10 million u expect avg loss of 1 object every 10,000 years
  - Same for all storage classes

*Availability*:
  - Measures how readily available a service is
  - Varies depending on storage class
  - ex: S3 Standard has 99.99% availaility, ie, not available 53 minutes in a year.

### 1. S3 Standard
#### I. S3 Standard General Purpose
- 99.99% Availability
- Used for frequently accessed data
- Low latency and high throughput
- Sustain 2 concurrent facility failures
- Use Cases: Big data analytics, mobile & gaming apps, content distribution, ...

#### II. S3 Standard - Infrequent Access (S3 Standard-IA)
- Data less frequently accessed, but required rapid access when needed
- Lower costs than S3 Standard
- 99.9% Availability
- Use Cases: Disaster recovery, backup

#### III. S3 Standard - One Zone Infrequent Access (S3 One Zone-IA)
- Data less frequently accessed, but required rapid access when needed
- Lower costs than S3 Standard
- Data lost when AZ is destroyed
- 99.5% Availability
- Use Cases: Storing secondary backup of on-premise data, or recreatable data

### 2. S3 Glacier
- Low cost object storage meant for achiving/backup
- Pricing: price for storage + object retrieval cost

#### I. S3 Glacier Instant Retrieval
- Millisecond retrieval, great for data accessed once a quarter
- Minimum storage duration is 90 days

#### II. S3 Glacier Flexible Retrieval (formerly Amazon S3 Glacier)
- Types:
  - Expedited (1 to 5 minutes)
  - Standard (3 to 5 hours)
  - Bulk (5 to 12 hours) - free
- Minimum storage duration is 90 days

#### III. S3 Glacier Deep Archive - for long term storage
- Standard (12 hours), Bulk (48 hours)
- Minimum storage duration is 180 days

### 3. S3 Intelligent Tiering
- Small monthly monitoring and auto-tiering fee
- Moves objects automatically between access tiers based on usage
- There are no retrieval charges in S3 intelligent tiering
- Tiers:
  - Frequent Access (automatic): default tier
  - Infrequent Access (automatic): objects not accessed for 30 days
  - Archive Instant Access (automatic): objects not accessed for 90 days
  - Archive Access (optional): configurable from 90 days to 700+ days
  - Deep Archive Access (optional): configurable from 180 days to 700+ days


### 4. S3 Express One Zone
- High performance, single AZ storage class
- Objects stored in a directory bucket (bucket in a single AZ)
- handle 10,000s requests per second with single digit millisecond latency
- Up to 10x better performance than S3 standard (50% lower costs)
- High durability, availability 99.95%
- Co-locate your storage and compute resources in the same AZ (reduces latency)
- Use cases: latency-sensitive apps, data-intensive apps, AI & ML training, financial modeling, media processing, HPC...
- Best integrated with SageMaker Model Training, Athena, EMR, Glue...

---

brief on Edge computing & snowball edge compute

---

AWS Storage Cloud Native Options

Block
- Amazon EBS
- EC2 Instance Store

File
- Amazon EFS

Object
- Amazon S3
- Glacier

AWS Storage Gateway bridges between on-premise data and cloud data in S3

--- 

## Amazon S3 Summary

- Buckets vs Objects: global unique name, tied to a region
- S3 Security: IAM policy, S3 Bucket policy (public access), S3 Encryption
- S3 Websites: host a static website on Amazon S3
- S3 Versioning: multiple versions for files, prevent accidential deletes
- S3 Replication: same-region or cross-region, must enable versioning
- S3 Storage Classes: Standard, IA, IZ-IA, Intelligent, Glacier (Instant, Flexible, Deep)
- Snowball: import data onto S3 through a physical device, edge computing
- Storage Gateway: hybrid solution to extend on-premises storage to S3