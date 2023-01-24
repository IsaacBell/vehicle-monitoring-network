# Why Cassandra?

Cassandra is designed to handle write-heavy workloads and is optimized for high write throughput. It is a highly available and highly scalable NoSQL database that uses a distributed architecture to distribute data across multiple nodes.

Cassandra's write performance is based on a log-structured storage system which allows for writes to be performed in-memory and then asynchronously flushed to disk, this improves write performance as disk access is reduced. Additionally, Cassandra uses a write-ahead log which allows for quick recovery in case of a node failure.

Cassandra also has a tunable consistency model, allowing for a trade-off between consistency and performance. In a write-heavy system, it is common to use a lower consistency level such as "quorum" or "any" to increase write performance.
