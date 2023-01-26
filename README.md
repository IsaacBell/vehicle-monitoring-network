Vehicle fleet monitoring and management network, written in C++, using Boost and Cassandra.

By nature, a monitoring system will be extremely write-heavy, making Cassandra a natural database choice. For more on the reasoning behind this, see [Why Cassandra?](#docs/cassandra.md#why-cassandra)

Server code is in the lib/ folder.

Embedded device code is in the arduino/ folder.

## Environment Variables

- CASSANDRA_HOST
- CASSANDRA_PORT

## Dependendecies

- C++ 2020
- Boost.Geometry
- Boost.PropertyTree (JSON)
- Boost.ASIO
- Boost.System
- Cmake 3.1 or higher
