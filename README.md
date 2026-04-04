# Nexus Traffic Intelligence Node v9.0

A massively scalable, enterprise-grade traffic, routing, and parking management simulation built in modern C++. This project evolved from a procedural C script into a deeply modular, multithreaded, Object-Oriented C++ architecture, complete with advanced graph algorithms, real-time background simulations, and a fully stylized Terminal UI.

## Ì≥ë Table of Contents
1. [Key Enterprise Features](#-key-enterprise-features)
2. [Role-Based Access Control (RBAC)](#-role-based-access-control-rbac)
3. [Interactive Terminal Capabilities](#Ô∏è-interactive-terminal-capabilities)
    * [Nexus Core Functionality](#nexus-core-functionality-global-access)
    * [Law Enforcement Protocols](#law-enforcement-protocols-restricted)
    * [Administrative & Financial Suite](#administrative--financial-suite-restricted)
4. [Architectural Subsystems](#Ô∏è-architectural-subsystems-newcodecpp)
5. [Build & Run Instructions](#Ô∏è-build--run-instructions)
6. [Default Test Credentials](#-default-test-credentials)
7. [Logs & Auditing](#-logs--auditing)

---

## Ì∫Ä Key Enterprise Features

* **Vibrant Terminal UI (CLI):** Fully styled console interface utilizing ANSI escape codes, animated progress bars, padded tabular layouts, and ASCII art dashboards for maximum readability and immersion.
* **Multithreaded Live Simulation:** A background process utilizing native system threads continuously simulates dynamic traffic congestion and weather shifts over time without interrupting the user.
* **Advanced Graph Algorithms:**
  * **A* (A-Star) Pathfinding:** Replaced Dijkstra with geographical A* utilizing Haversine distance heuristics, factoring in weather severity, road tolls, and realtime congestion.
  * **Prim's Algorithm (MST):** Used by Analysts/Admins to calculate the Minimum Spanning Tree for the most cost-effective infrastructure connections across the entire graph.
  * **BFS Radar Scans:** Calculates the maximum reachable range (all viable nodes) from an origin given a strict travel-time budget.
* **Dynamic Weather Subsystem:** Live weather states (Clear, Rain, Snow, Storm, Fog) that drastically alter edge weights and travel times in the routing engine.
* **Complete Economy & Ticketing:** 
  * Multi-tiered parking (VIP, Premium, Standard) tied to a digital wallet.
  * Highway tolls calculated during A* traversal.
  * Law Enforcement module allowing Police to issue citations, deduct fines, and lower driver reputation points.
* **Thread-Safe Logging:** A singleton `Logger` utilizing `<mutex>` to create asynchronous, thread-safe persistent system event logs (`system_events.log`).

---

## Ì¥ê Role-Based Access Control (RBAC)

The system implements strict permission gating through four core operational clearances:
* Ì¥¥ **Admin:** Highest clearance. Unrestricted access to all features, infrastructure management, and economy oversight.
* Ì¥µ **Police:** Law Enforcement. Authorized to alter traffic flow (blockades) and issue financial citations to users.
* Ìø¢ **Driver:** Basic civilian clearance. Authorized to calculate routes, reserve parking, and manage personal vehicles. Emergency Medical Services (EMS) inherit this role with special bypass flags.
* Ìø£ **Analyst:** Engineering and accounting clearance. Permitted to process graph calculations (MST) and view global tolls/revenues.

---

## Ì∂•Ô∏è Interactive Terminal Capabilities

Below is the detailed list of runtime functions available via the primary application interface. Functions are strictly scoped based on the operator's role.

### Nexus Core Functionality (Global Access)
> **Permits:** `Admin`, `Police`, `Driver`, `Analyst`

| Command | Function Name | Description & Mechanics |
| :--- | :--- | :--- |
| **[1]** | **View Active City Network Tracker** | Prints a live topological matrix of the entire road network. Displays current traffic congestion (scaled 0-100%), speed limits, and incident blockades. |
| **[2]** | **Calculate Fastest Route (A*)** | Predicts the optimal path between two city nodes using Haversine distance heuristics. Factors traversing times, dynamic weather multipliers, and live congestion. Checks the user's digital wallet to deduct toll costs automatically. *Note: Emergency EMS vehicles bypass roadblocks and tolls.* |
| **[3]** | **Parking Grid Management** | Prints a live table of vacant and occupied parking zones. Permits `Driver` roles to securely buy blocks of time across Standard, Premium, and VIP arrays. |
| **[4]** | **Radar Scan (BFS)** | Performs a Breadth-First Search sweep calculating the maximum geographical range achievable from a given departure node under a strict time budget constraint. |
| **[5]** | **View Personal Driving Record** | Displays the current operator's reputation points (out of 100) and prints copies of all traffic citations and fines issued to them to date. |

### Law Enforcement Protocols (Restricted)
> **Permits:** `Admin`, `Police`

| Command | Function Name | Description & Mechanics |
| :--- | :--- | :--- |
| **[6]** | **Report Incident & Blockade Highway** | Temporarily isolates an edge connecting two City nodes. Renders the route impassable to standard routing, forcing the A* pathfinder to adapt via extended detours. |
| **[7]** | **Resolve Incident & Retract Blockade** | Clears barricades from an isolated edge, restoring natural graph weights and traffic flow. |
| **[8]** | **Issue Driver Infraction/Citation** | Permits officers to dispatch violations to a `Driver`, instantly docking their reputation points and deducting a custom fine amount directly from their wallet. |

### Administrative & Financial Suite (Restricted)
> **Permits:** `Admin`, `Analyst` *(Exception: Node execution limited to Admin)*

| Command | Function Name | Description & Mechanics |
| :--- | :--- | :--- |
| **[9]** | **Process Minimal Spanning Tree** | Executes Prim's Algorithm to locate the cheapest interconnected link across all City nodes, calculating the minimal graph distance required to sustain edge connectivity. |
| **[10]** | **Retrieve Global Revenue Audits** | Connects to the parking and toll subsystems, printing aggregate gross revenue extracted from network usage metrics. |
| **[11]** | **Spawn New Graph Node (City)** | *(Requires `Admin`)* Dynamically allocates RAM to inject a brand-new City object into the hashmap via user input. Incorporates population, Lat/Lon coordinates, and expands the routing graph on the fly. |

---

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

---

## ‚öôÔ∏è Build & Run Instructions

### Prerequisites
* A C++ compiler supporting C++11 or higher (GCC/Clang/MSVC).
* POSIX/ANSI compliant terminal for UI colors (Git Bash, macOS, Linux, or modern Windows Terminal).

### Compilation
Navigate to the repository and compile using `g++`. Standard native threading is handled automatically conditionally depending on OS targets.

```bash
g++ newcode.cpp -o nexustraffic -std=c++11
```

### Execution
Run the compiled executable output (Use `./nexustraffic` on Linux/Bash, `.\nexustraffic.exe` on Windows CMD).

```bash
./nexustraffic.exe
```

---

## Ì¥ë Default Test Credentials

| Username | Password | Role | Features |
| :--- | :--- | :--- | :--- |
| `admin` | `admin123` | **Admin** | Full system control, infrastructure tools, infinite funds |
| `driver1` | `pass` | **Driver** | View routes, pay tolls, reserve parking, receive citations |
| `ems1` | `pass` | **Driver (EMS)** | Possesses emergency vehicle override capability |
| `cop1` | `pass` | **Police** | Declare accidents, isolate nodes, issue tickets |
| `analyst` | `data` | **Analyst** | Execute Prim's MST, view global revenue metrics |

---

## Ì≥ä Logs & Auditing
All system events, logins, ticket distributions, routing traces, and infrastructure errors are pushed to `system_events.log` in real-time. Monitor this file for strict auditing records.
