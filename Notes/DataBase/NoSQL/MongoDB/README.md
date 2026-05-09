# MongoDB

benefits:
- we store data without having nulls, without many columns
- stored data is lightweight

limitations:

types of nosql:

- key-value: ....explain, redis, ignite, riak, memcached
- column: ... explain, cassandra, apache hbase, scylla
- document: ..explain, mongodb, couchdb, arangodb
- graph: ..explain, allegrograph, neo4j

has many drivers: C, C++ , JS, php, ... (what does this mean?)
built by C++
built using BSON (what is it?)

when to use and not to use MongoDB?

DB -> DB
Table -> Collection
Tuple/Row -> Document
Column -> Field
Table join -> embedded documents


mysqld/oracle vs mysql/sqlplus and mongod vs mongo

mongodb automatic id generated: 12 hexadecimal digits
4 -> timestamp
3 -> machine id
2 -> process id
3 -> incremental #

why is horizontal scaling in sql difficult
