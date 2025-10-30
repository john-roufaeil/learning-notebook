# Database Basics

## Introduction

### Limitations of File Based System

- Separation & Isolation of data
- Duplication of data
- Program Data dependence
- Incompatible file format

### Terminologies

> Database -> A collection of related data


> DBMS (Data Base Management System) -> A software package /system to facilitate the creation and mgmt of a database

> Database System -> DBMS + data

### DBMS Components

![DB Components](database_components.png)

DB Metadata:
- Table name
- Column name
- Column Data type
- Constraints
- Access Privileges
- Username
- Password
- User's Privileges
- DB objects structure
- Log files

Sored DB: data itself

### Advantages of DBMS

- Controlling redundancy: data written once, and several users can access it
- Restricting Access: we create the users and give specific access
- Enforcing integrity constraints
- Providing backup and recovery

### DB Users

- **System Analyst**: analyses and gathers requirements
- **Database Designer**: creates database design (conceptual schema)
- **Database Administrator (DBA)**: Install DBMS, create DB schema and populate data, create users and their authorization, maintain DB performance
- **Application Programmer**: develop, test, debug app
- **End user**

### DBMS Architecture (Three-Schema Architecture)

1. External Schema (View level)
    - Defines what each user or group can access and how data is displayed.
    - Multiple external schemas exist for different users or applications.
2. Conceptual Schema (Logical level)
    - Defines what data is stored and the relationships between data.
    - Describes database structure — entities, attributes, and constraints.
3. Physical Schema (Internal level)
    - Defines how data is physically stored on disk.
    - Handles storage allocation, indexing, and access paths.

#### Mappings

- **External ↔ Conceptual Mapping:** Translates user views into the logical structure of the database.
- **Conceptual ↔ Physical Mapping:** Translates the logical design into physical storage structures.

### Data Models

1. Logical/Conceptual Model
    - Represents the structure of data and relationships independent of physical storage.
    - Example: Entity Relationship Diagram (ERD)

2. Physical Model
    - Represents how data is actually stored and accessed in the database.
    - Example: Table structures, indexes, partitions, storage paths

### Other Functions of a DBMS

Modern DBMSs provide more than just basic data storage and retrieval. They support advanced data types and operations for diverse applications:
    - Spatial Data Management:
        - Handles geographical and geometric data (maps, coordinates, regions).
        - Supports queries like distance, area, and spatial relationships.
    - Time-Series Data Management:
        - Manages data that changes over time (sensor readings, stock prices, logs).
        - Optimized for time-based queries, trends, and historical analysis.
    - Multimedia Data Management:
        - Stores and retrieves images, audio, video, and documents.
        - Supports metadata indexing, compression, and content-based search
    - Data Mining and Analytics:
        - Integrates data analysis and pattern discovery functions.
        - Enables clustering, classification, and prediction within the DBMS.

### Database Environment

#### Centralized Database Environment

All data is stored and managed on a **single central server**. This setup is simpler but has limited scalability and fault tolerance.

1. Mainframe Environment
    - All processing is handled by **one central server**.
    - Performance is **slow** due to the heavy load on a single machine.
    - **Single point of failure**: if the server fails, both the application and database become unavailable.
2. Client/Server Environment
    - **Thick client**: the application is installed locally on user machines, while the database is hosted on a central server.
    - Database server remains a single point of failure — if it fails, all clients lose access.
    - Application layer is distributed, so it’s not a single point of failure.
    - High support and maintenance cost due to managing many client installations.
3. Internet Computing Environment (Three-tier architecture)
    - Consists of three layers:
        - Database Server
        - Application Server
        - Thin Client (Web Browser)
    - Application Server handles the logic and can become a single point of failure if only one is used.
    - Lower cost for maintenance and support (clients only need a browser).
    - Three-tier: one application server.
    - N-tier: multiple application servers for load balancing and fault tolerance.
    - If one application server fails, users can be redirected to another running server.
    - The database is still a single point of failure unless replicated.

#### Distributed Database Environment

Data is stored across **multiple interconnected databases** on different machines or locations.

- Provides high availability and fault tolerance; database is no longer a single point of failure.
- Can be homogeneous (same DBMS vendor) or heterogeneous (different vendors).
- High setup and licensing cost, but much better reliability and scalability.

    1. Database Replication
        - Data is copied (partially or fully) across multiple databases.
        - Ensures data redundancy and faster local access.

    2. Database Fragmentation
        - Data is divided into fragments and distributed across different databases.
        - Can be:
            - Horizontal fragmentation: divides rows.
            - Vertical fragmentation: divides columns.
            - Hybrid fragmentation: mix of both.
        - All fragments remain logically connected and accessible as a single database.

## Entity Relationship Diagram

![ERD Example](erd_example.png)

### Core Concepts

- **Data Model**: a conceptual representation of how data is structured and related within a system.
- **Entity**: something in the real world with an **independent existence**. Could be physical or conceptual. can be described with attributes or characteristics.
- **Attribute**: particular properties that describe the entity.
- **Relationship**: an association between two or more entities (e.g., Employee works in Department).

### Steps to build a data model

1. What entities need to be described in the model?
2. What attributes of these entities need to be recorded?
3. Can an attribute or set of attributes uniquely identify one specific occurence of an entity?
4. What relationships exist between entities?

### Entity Types

1. Strong Entity
    - Exists independently of other entities.
    - Has a primary key that uniquely identifies it.
2. Weak Entity (Doubled Rectangle)
    - Depends on a strong entity for its existence.
    - Identified by a partial key combined with the key of the related strong entity.

### Attribute Types

![Attribute Types](attribute_types.png)

- Simple Attribute: Cannot be divided further (Name).
- Composite Attribute: Can be divided into smaller parts (FullName → FirstName, LastName).
- Derived Attribute: Value can be derived from another (Age from DateOfBirth).
- Multivalued Attribute: Can have multiple values (PhoneNumbers).

### Key Types

1. Candidate Key: Attribute(s) that can uniquely identify an entity.
2. Primary Key: The chosen candidate key used for identification.
3. Foreign Key: Attribute that refers to a primary key in another entity to represent a relationship.

### Relations

> A connection bewteen entity classes

Each relationship needs:

1. Degree of relationship: Number of entities participating in this relationship
    - Unary (1 Entity): e.g., an Employee supervises another Employee.
    - Binary (2 Entities): e.g., an Employee works in a Department.
    - Ternary (3 Entities): e.g., an Employee uses Skills in a Project.
2. Cardinality Ratio: Maximum number of relationships
    - 1:1 (One-to-One)
    - 1:N (One-to-Many)
    - N:1 (Many-to-One)
    - M:N (Many-to-Many)
    - In a ternary relationship, if cardinalities differ, it’s usually better to split into multiple binary relationships.
3. Participation: Minimum number of relationships
    - Total (Must Participate): shown with double lines.
    - Partial (May Participate): shown with single lines.

