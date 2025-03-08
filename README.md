# 🚦 Traffic Management System

<p align="center">
  <img src="https://img.shields.io/badge/language-C-blue.svg" alt="Language">
  <img src="https://img.shields.io/badge/platform-Cross--Platform-lightgrey.svg" alt="Platform">
  <img src="https://img.shields.io/badge/license-MIT-green.svg" alt="License">
  <img src="https://img.shields.io/badge/status-Stable-success.svg" alt="Status">
</p>

A comprehensive traffic management solution designed to optimize city navigation, analyze traffic patterns, and manage parking infrastructure. This system employs graph algorithms to determine optimal routes between cities based on traffic density, provides road status information, and offers a parking reservation system.

## 🌟 Features

### 🧭 Navigation & Routing
- **Optimal Path Finding**: Utilizes Dijkstra's algorithm to calculate routes with minimal traffic congestion
- **Traffic Index Analysis**: Evaluates and compares traffic density across different cities
- **Road Information**: Provides detailed status about roads including lane configuration, construction status, and safety warnings

### 🚗 Parking Management
- **Slot Reservation**: Secure parking slot booking system with user authentication
- **Availability Tracking**: Real-time monitoring of parking slot availability
- **User Reservation History**: Maintains records of user parking activities

### 🔒 Security System
- **User Authentication**: Multi-level login system with attempt limitation
- **Admin Controls**: Privileged access for system management and road status updates

### 🔍 Analysis Tools
- **City Reachability**: BFS-based algorithm to determine cities accessible within a specified number of road connections
- **Traffic Sorting**: Multiple sort methods (alphabetical, traffic index) for data analysis
- **Road Status Updates**: System for maintenance crews to update construction status

## 🖥️ Technical Architecture

```
┌─────────────────────────────┐
│    Authentication Module    │
└─────────────┬───────────────┘
              │
              ▼
┌─────────────────────────────┐       ┌───────────────────────┐
│   Traffic Routing Engine    │◄─────►│  Graph Data Structure  │
└─────────────┬───────────────┘       └───────────────────────┘
              │
              ├────────────────┬────────────────┐
              ▼                ▼                ▼
┌─────────────────┐  ┌─────────────────┐  ┌────────────────────┐
│ Parking System  │  │ Road Info System │  │ City Analysis Tool │
└─────────────────┘  └─────────────────┘  └────────────────────┘
```

## 💾 Data Structure

The system uses multiple specialized data structures:

- **Adjacency Matrix**: Represents the connectivity between cities with weighted edges for traffic density
- **Linked Lists**: Manages parking slot reservations
- **Arrays and Structs**: Organizes city information and road status details

## 📊 Algorithmic Approach

- **Dijkstra's Algorithm**: Finds shortest paths while accounting for traffic conditions
- **Breadth-First Search (BFS)**: Determines reachable cities within a set number of road connections
- **Heap Sort**: Efficiently sorts cities based on traffic indices for analysis

## 🚀 Getting Started

### Prerequisites

- C Compiler (GCC recommended)
- Standard C libraries
- Text files for initialization data

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/traffic-management-system.git
   cd traffic-management-system
   ```

2. Compile the program:
   ```bash
   gcc traffic_management.c -o traffic_system
   ```

3. Prepare the required data files:
   - `adjacency_matrix.txt`: Contains the connectivity information between cities
   - `cities_list.txt`: Contains city IDs and names

4. Run the application:
   ```bash
   ./traffic_system
   ```

## 📘 Usage Guide

### Authentication

The system requires authentication with a valid username and password:
- Default admin credentials: `admin`/`admin123`
- Sample user credentials: `user1`/`password1`, `user2`/`password2`

### Main Menu

After successful authentication, users can access the following features:

1. **Display Cities**: View all cities in the system
2. **City Information**: Get details about cities and sort them by different criteria
3. **Path Finding**: Find optimal routes between source and destination cities
4. **Parking Slot Reservation**: Book parking slots for specific durations
5. **Parking Status**: View current parking slot availability
6. **User Search**: Find parking reservations by username
7. **Road Information**: Get detailed information about roads connecting cities
8. **Reachability Analysis**: Find cities accessible within a specific number of roads
9. **Road Status Updates**: Update information about road conditions

### Example: Finding the Optimal Route

```
Enter Start Location: CityA
Enter The Destination: CityE

The shortest Route is:
    CityA-->(3)-->CityB-->(2)-->CityC-->(4)-->CityE

Average Traffic-index: 3
```

## 🧪 Test Data Configuration

The system requires two data files to function properly:

### adjacency_matrix.txt
This file contains the traffic density matrix:
```
0,3,5,0,0,...
3,0,2,4,0,...
5,2,0,6,1,...
...
```

### cities_list.txt
This file maps city IDs to names:
```
0 CityA
1 CityB
2 CityC
...
```

## 🔄 Road Status Information

The system maintains multiple attributes for each road:
- Two-lane status (yes/no)
- One-way status (yes/no)
- Construction status (yes/no)
- Accident-prone status (yes/no)

## 🛠️ Future Enhancements

- Graphical User Interface for better visualization
- Real-time traffic updates from external APIs
- Mobile application integration
- Predictive traffic analysis using historical data
- Integration with GPS systems
- Support for public transportation routing


## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request



<p align="center">
  <sub>Built with ❤️ for better urban transportation management</sub>
</p>

