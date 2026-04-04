# Nexus Traffic Intelligence Node v9.0

A massively scalable, enterprise-grade traffic, routing, and parking management simulation built in modern C++. This project evolved from a procedural C script into a deeply modular, multithreaded, Object-Oriented C++ architecture, complete with advanced graph algorithms, real-time background simulations, and a fully stylized Terminal UI.

## Ì∫Ä Key Enterprise Features

* **Vibrant Terminal UI (CLI):** Fully styled console interface utilizing ANSI escape codes, animated progress bars, padded tabular layouts, and ASCII art dashboards for maximum readability and immersion.
* **Multithreaded Live Simulation:** A background process utilizing `std::thread` continuously simulates dynamic traffic congestion and weather shifts over time without interrupting the user.
* **Advanced Graph Algorithms:**
  * **A* (A-Star) Pathfinding:** Replaced Dijkstra with geographical A* utilizing Haversine distance heuristics, factoring in weather severity, road tolls, and realtime congestion.
  * **Prim's Algorithm (MST):** Used by Analysts/Admins to calculate the Minimum Spanning Tree for the most cost-effective infrastructure connections across the entire graph.
  * **BFS Radar Scans:** Calculates the maximum reachable range (all viable nodes) from an origin given a strict travel-time budget.
* **Dynamic Weather Subsystem:** Live weather states (Clear, Rain, Snow, Storm, Fog) that drastically alter edge weights and travel times in the routing engine.
* **Complete Economy & Ticketing:** 
  * Multi-tiered parking (VIP, Premium, Standard) tied to a digital wallet.
  * Highway tolls calculated during A* traversal.
  * Law Enforcement module allowing Police to issue citations, deduct fines, and lower driver reputation points.
* **Role-Based Access Control (RBAC):** Distinct operational clearances for `Admin`, `Driver`, `Police`, and `Analyst`. Actions like blocking highways, extracting revenue audits, or injecting new city nodes are heavily permission-gated.
* **Thread-Safe Logging:** A singleton `Logger` utilizing `<mutex>` to create asynchronous, thread-safe persistent system event logs (`system_events.log`).

## ÌøóÔ∏è Architectural Subsystems (`newcode.cpp`)
1. **Terminal UI & Styling:** Abstracts cross-platform console clearing, color rendering, and data table layouts.
2. **Utility & Logging:** Async file streams with mutex locks to record system events.
3. **Weather Subsystem:** Random number generators mapping environmental conditions to traffic multipliers.
4. **User & Auth Manager:** Memory-safe credential verification, session management, and wallet/vehicle storage.
5. **Graph Model & Geography:** Embeds latitude/longitude coords in `City` nodes and dynamic edge traits in `Road` objects.
6. **Smart Parking System:** $O(n)$ optimization handling time-based pricing tiers and tracking parking revenues.
7. **Law Enforcement:** Enforces speeding tickets, blocks/opens routes, and tracks the criminal record (reputation) of drivers.
8. **Core Traffic Engine:** The graph processor dynamically recalculating paths, processing Prim's MST, executing BFS bounds, and computing Haversine heuristics.
9. **Master Application Controller:** Orchestrates the UI loop and manages the concurrent simulation threads.

## ‚öôÔ∏è Build & Run Instructions

### Prerequisites
* A C++ compiler supporting C++11 or higher (GCC/Clang/MSVC).
* POSIX/ANSI compliant terminal for UI colors (Linux, macOS, or modern Windows Terminal).

### Compilation
Navigate to the repository and compile using `g++`. **Note: You must link the pthread library for the background sim to run.**

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
| `admin` | `admin123` | **Admin** | Full system control, infrastructure tools, infinite funds |
| `driver1` | `pass` | **Driver** | View routes, pay tolls, reserve parking, receive citations |
| `ems1` | `pass` | **Driver (EMS)** | Possesses emergency vehicle override capability |
| `cop1` | `pass` | **Police** | Declare accidents, isolate nodes, issue tickets |
| `analyst` | `data` | **Analyst** | Execute Prim's MST, view global revenue metrics |

## Ì≥ä Logs & Auditing
All system events, logins, ticket distributions, routing traces, and infrastructure errors are pushed to `system_events.log` in real-time. Monitor this file for strict auditing records.
