# Nexus Traffic Intelligence System

A highly scalable, enterprise-grade traffic and parking management simulation built in modern C++. Evolving from a procedural C-based prototype, this system features a robust Object-Oriented Architecture capable of handling massive city graphs (up to 8,000+ line scale capabilities).

## ��� Key Enterprise Features

* **Role-Based Access Control (RBAC):** Distinct privileges for `Admin`, `Police`, and `Driver` roles affecting what actions can be taken in the network.
* **Advanced Heuristic Routing (A*):** Replaced legacy Dijkstra calculations with a geographical A* (A-Star) search algorithm for vastly superior pathfinding performance over massive nodes.
* **Dynamic Incident Management:** Police and Admins can trigger live road closures, instantly updating edge weights and dynamically forcing the routing engine to detour traffic.
* **Smart Parking & Economy Simulation:** Multi-tier parking slots (VIP, Premium, Standard) combined with a digital wallet deduction system.
* **Thread-Safe Logging System:** A singleton `Logger` utilizing `<mutex>` to create asynchronous, thread-safe persistent system event logs (`system_events.log`).
* **Optimized Data Structures:** Replaced static multidimensional arrays with STL `<unordered_map>`, `<queue>`, and `<vector>` for infinite graph scaling ($O(1)$ lookups and memory efficiency).

## ���️ Architectural Subsystems

The codebase (`newcode.cpp`) is deeply modularized into single-responsibility domains:

### 1. Utility & Logging Subsystem
* `Logger`: Singleton class handling file streams with mutex locks to record logins, transactions, and system modifications securely across concurrent tasks.

### 2. User & Authentication Subsystem
* `AuthManager`: Handles memory-safe credential verification and session tracking.
* Capabilities: Distinguishes between Authorities (managing roads/incidents) and Clients (paying tolls and parking).

### 3. Graph Model & Geography Subsystem
* `City` & `Coordinate`: Embeds real-world latitude/longitude properties to drastically optimize geographical sorting and heuristics.
* `Road`: Encapsulates dynamic traits `tollCost`, `currentTrafficLevel`, and `isClosed`.

### 4. Traffic Engine (Core Routing)
* `TrafficEngine`: Manages the graph adjacency list completely dynamically. Performs A* evaluations dynamically calculating Haversine distance heuristics combined with traffic congestion multipliers and node isolation checks.

### 5. Smart Parking Management
* `ParkingSystem`: Handles time-based tracking ($O(n)$ optimization) checking dynamic slot tiers and verifying wallet constraints.

## ���️ Build & Run Instructions

### Prerequisites
* A C++ compiler supporting C++11 or higher (GCC/Clang/MSVC).

### Compilation
Navigate to the repository and compile using `g++`:

```bash
g++ newcode.cpp -o nexustraffic -std=c++11 -pthread
```

### Execution
```bash
./nexustraffic
```

### Default Test Credentials
| Username | Password | Role | Features |
| :--- | :--- | :--- | :--- |
| `admin` | `admin123` | **Admin** | Full system control, infinite funds |
| `driver1` | `pass` | **Driver** | View routes, pay tolls, reserve parking |
| `cop1` | `pass` | **Police** | Declare accidents, isolate city nodes |

## ��� Logs & Auditing
All system events are pushed to `system_events.log`. Monitor this file to audit identity logins, infrastructure errors, and parking allocations in real-time.
