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

## Mapping to Tables

> After designing an ERD, the next step is to **convert (map)** it into a **relational schema**, a set of database tables with relationships represented using keys.

![tables](tables_example.png)

### **1. Strong Entity → Table**
- Each **strong entity** becomes a **table**.  
- **Attributes** become table columns.  
- The **primary key** of the entity becomes the **primary key** of the table.  

**Example:**  
Entity: `Student (StudentID, Name, Email)`  
→  
Table:  `STUDENT(StudentID [PK], Name, Email)`

### **2. Weak Entity → Table**
- Each **weak entity** also becomes a table.  
- Include a **foreign key** from its **owner entity** (the strong one).  
- The **primary key** is the combination of the **partial key + owner’s primary key**.  

**Example:**  
Weak Entity: `Dependent (DepName)` depends on `Employee (EmpID)`  
→  
Table:  `DEPENDENT(EmpID [FK], DepName, PRIMARY KEY (EmpID, DepName))`

### **3. Attributes → Columns**
- **Simple attributes:** become individual columns.  
- **Composite attributes:** split into separate columns.  
  - e.g., `Name → FirstName, LastName`  
- **Multivalued attributes:** require a **separate table** with a foreign key reference.  
  - e.g., `Student_Phone(StudentID [FK], PhoneNumber)`  
- **Derived attributes:** usually **not stored**, since they can be computed.

### **4. Relationships → Foreign Keys or New Tables**

#### **a. One-to-One (1:1)**  
- Add the **primary key of one entity** as a **foreign key** in the other.  
- Choose the side that makes logical sense or has total participation.  

#### **b. One-to-Many (1:N)**  
- Add the **primary key** of the “one” side as a **foreign key** in the “many” side.  

**Example:**  
`Department (DeptID)` —< `Employee (EmpID, DeptID [FK])`  

#### **c. Many-to-Many (M:N)**  
- Create a **new table (intersection/junction table)**.  
- Include **foreign keys** from both entities, and optionally other relationship attributes.  

**Example:**  
`Student` —< `Enrollment` >— `Course`  
ENROLLMENT(StudentID [FK], CourseID [FK], Grade)
PRIMARY KEY (StudentID, CourseID)

**d. Ternary Relationships**  
- Create a **new table** with foreign keys from all three entities.  

**Example:**  
ASSIGNMENT(EmployeeID [FK], ProjectID [FK], SkillID [FK])


### **5. Specialization / Generalization (Optional)**
For **inheritance hierarchies**, common mapping approaches include:

1. **Single Table:** One table for all entities (add a type attribute).  
2. **Class Table:** One table per subclass with a foreign key to the superclass.  
3. **Separate Tables:** Each subclass has its own table with duplicated shared fields.

### **Summary**

| ERD Element | Mapping in Tables |
|--------------|-------------------|
| Strong Entity | Table with attributes and PK |
| Weak Entity | Table + FK to owner + composite PK |
| Simple Attribute | Column |
| Composite Attribute | Split into multiple columns |
| Multivalued Attribute | Separate table |
| Derived Attribute | Not stored |
| 1:1 Relationship | FK in one table |
| 1:N Relationship | FK in “many” side |
| M:N Relationship | New intersection table |
| Ternary Relationship | New table with 3 FKs |

![mapping to tables instructions](mapping_instructions.png)

#### **How ERD, SQL, and Tables Match the Three-Schema Architecture**

| Concept / Artifact | Schema Level | Data Model Type | Description |
|--------------------|---------------|------------------|--------------|
| **ERD (Entity Relationship Diagram)** | **Conceptual Schema** | **Logical / Conceptual Model** | Represents *what data exists* and *how entities relate*. No details on how it’s stored. |
| **SQL DDL (CREATE TABLE, ALTER TABLE, etc.)** | **Physical Schema** | **Physical Model** | Defines *how data is implemented* in the database — table structures, data types, indexes, and constraints. |
| **Database Tables (actual stored data)** | **Physical Schema** | **Physical Model** | The *real physical storage* of data in files or pages on disk, managed by the DBMS. |
| **Views (SQL VIEW)** | **External Schema** | — | Defines *what part of the data a user or application sees* and *how it is presented*. |
