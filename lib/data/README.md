# Lib.Data

A private lib for DB transactions and general-purpose data structures.

## Vehicle

A Vehicle has many diagnostics. Diagnostics are constantly coming in, so we update constantly as JSON POST data hits the server.

## Database

### Configuration



| Property  | Dev Default | Prod Default
| ------------- | ------------- | ------------- |
| Port  | 9042  | TBA |
| Replication Strategy  |  SimpleStrategy | NetworkTopologyStrategy |
| Replication Factor  |  1 | 3 |
| QuadTree Max # Layers  | 50  | 500 |

### Tables

- "users.attributes"
- "vehicles.diagnostics"
