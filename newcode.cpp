#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <climits>
#include <ctime>
#include <iomanip>
#include <memory>
#include <cmath>
#include <thread>
#include <mutex>

using namespace std;

// ==========================================
// UTILITY & LOGGING SUBSYSTEM
// ==========================================
class Logger {
private:
    static Logger* instance;
    ofstream logFile;
    mutex logMutex;
    Logger() { logFile.open("system_events.log", ios::app); }
public:
    static Logger* getInstance() {
        if (!instance) instance = new Logger();
        return instance;
    }
    void log(const string& level, const string& message) {
        lock_guard<mutex> guard(logMutex);
        time_t now = time(0);
        char* dt = ctime(&now);
        string dateStr(dt);
        dateStr.pop_back(); // remove newline
        logFile << "[" << dateStr << "] [" << level << "] " << message << "\n";
    }
    ~Logger() { if(logFile.is_open()) logFile.close(); }
};
Logger* Logger::instance = nullptr;

// ==========================================
// USER & AUTHENTICATION SUBSYSTEM
// ==========================================
enum class UserRole { ADMIN, DRIVER, POLICE };

class User {
public:
    string username;
    string password;
    UserRole role;
    double walletBalance;

    User() : username(""), password(""), role(UserRole::DRIVER), walletBalance(0.0) {}
    User(string u, string p, UserRole r, double w) : username(u), password(p), role(r), walletBalance(w) {}
};

class AuthManager {
private:
    unordered_map<string, User> users;
    User* currentUser;
public:
    AuthManager() : currentUser(nullptr) {
        users["admin"] = User("admin", "admin123", UserRole::ADMIN, 9999.0);
        users["driver1"] = User("driver1", "pass", UserRole::DRIVER, 500.0);
        users["cop1"] = User("cop1", "pass", UserRole::POLICE, 0.0);
    }
    
    bool login() {
        string user, pass;
        int attempts = 0;
        while(attempts < 3) {
            cout << "\n--- Identity Node Login ---\nUser: "; cin >> user;
            cout << "Pass: "; cin >> pass;
            if(users.count(user) && users[user].password == pass) {
                currentUser = &users[user];
                Logger::getInstance()->log("INFO", "User logged in: " + user);
                return true;
            }
            cout << "Invalid credentials. Try again.\n";
            attempts++;
        }
        return false;
    }
    
    User* getCurrentUser() { return currentUser; }
    void logout() { currentUser = nullptr; }
};

// ==========================================
// GRAPH MODEL & GEOGRAPHY SUBSYSTEM
// ==========================================
struct Coordinate {
    double lat, lon;
    Coordinate(double la=0, double lo=0): lat(la), lon(lo) {}
};

class City {
public:
    int id;
    string name;
    int baseTrafficIndex;
    Coordinate coords;
    
    City() : id(-1), name(""), baseTrafficIndex(0) {}
    City(int i, string n, int t, Coordinate c) : id(i), name(n), baseTrafficIndex(t), coords(c) {}
};

enum class VehicleType { CAR, TRUCK, BIKE, EMERGENCY };

class Road {
public:
    int id;
    int src;
    int dest;
    double distance;
    int currentTrafficLevel; // 0-100
    bool isOneWay;
    double tollCost;
    bool isClosed; // Incidents
    
    Road(int id, int s, int d, double dist, bool oneWay, double toll) 
        : id(id), src(s), dest(d), distance(dist), currentTrafficLevel(10), isOneWay(oneWay), tollCost(toll), isClosed(false) {}
        
    double getEffectiveWeight() const {
        if(isClosed) return 999999.9;
        return distance * (1.0 + (currentTrafficLevel / 50.0));
    }
};

// ==========================================
// PARKING MANAGEMENT SUBSYSTEM
// ==========================================
enum class ParkingTier { STANDARD, PREMIUM, VIP };

struct ParkingRecord {
    int slotId;
    string owner;
    time_t startTime;
    int durationHours;
    ParkingTier tier;
    bool active;
};

class ParkingSystem {
private:
    vector<ParkingRecord> capacity;
    int maxSlots;
public:
    ParkingSystem(int max_s = 50) : maxSlots(max_s) {
        for(int i=0; i<max_s; i++) {
            capacity.push_back({i, "", 0, 0, (i<10 ? ParkingTier::VIP : (i<20 ? ParkingTier::PREMIUM : ParkingTier::STANDARD)), false});
        }
    }
    
    void showAvailability() {
        int avail = 0;
        for(auto& s : capacity) if(!s.active) avail++;
        cout << "\n[ Parking System ] " << avail << " / " << maxSlots << " slots available.\n";
    }

    void reserve(User* user) {
        if(!user || user->role != UserRole::DRIVER) { cout << "Only drivers can reserve parking.\n"; return; }
        cout << "[Slot Tiers] 1. VIP ($50/h)  2. Premium ($20/h)  3. Standard ($5/h)\nChoose tier: ";
        int c; cin >> c;
        ParkingTier pt = (c==1) ? ParkingTier::VIP : (c==2) ? ParkingTier::PREMIUM : ParkingTier::STANDARD;
        
        for(auto& slot : capacity) {
            if(!slot.active && slot.tier == pt) {
                cout << "Allocating Slot #" << slot.slotId << ". Hours required: ";
                int h; cin >> h;
                double cost = h * (pt == ParkingTier::VIP ? 50 : (pt == ParkingTier::PREMIUM ? 20 : 5));
                if(user->walletBalance >= cost) {
                    user->walletBalance -= cost;
                    slot.active = true;
                    slot.owner = user->username;
                    slot.durationHours = h;
                    slot.startTime = time(0);
                    cout << "Reservation confirmed. Wallet bal: $" << user->walletBalance << "\n";
                    Logger::getInstance()->log("INFO", user->username + " reserved parking slot " + to_string(slot.slotId));
                    return;
                } else {
                    cout << "Insufficient funds. Need $" << cost << "\n";
                    return;
                }
            }
        }
        cout << "No slots available in that tier.\n";
    }
};

// ==========================================
// CORE TRAFFIC GRAPH AND ROUTING
// ==========================================
class TrafficEngine {
private:
    unordered_map<int, City> cities;
    unordered_map<int, vector<Road>> adjList;
    unordered_map<string, int> nameIndex;
    int edgeCounter = 0;

    double heuristic(Coordinate a, Coordinate b) {
        return sqrt(pow(a.lat - b.lat, 2) + pow(a.lon - b.lon, 2)) * 111.0; // lazy haversine
    }

public:
    TrafficEngine() { initMockData(); }

    void initMockData() {
        addCity(0, "Metropolis", Coordinate(40.7128, -74.0060));
        addCity(1, "Gotham", Coordinate(41.8781, -87.6298));
        addCity(2, "StarCity", Coordinate(34.0522, -118.2437));
        addCity(3, "CentralCity", Coordinate(47.6062, -122.3321));
        
        addRoad("Metropolis", "Gotham", 800, false, 15.0);
        addRoad("Gotham", "StarCity", 2000, false, 25.0);
        addRoad("Metropolis", "StarCity", 2800, false, 50.0);
        addRoad("Gotham", "CentralCity", 600, false, 10.0);
    }

    void addCity(int id, string name, Coordinate c) {
        cities[id] = City(id, name, 10, c);
        nameIndex[name] = id;
        adjList[id] = vector<Road>();
    }

    void addRoad(string srcName, string destName, double dist, bool oneway, double toll) {
        int src = nameIndex[srcName];
        int dest = nameIndex[destName];
        adjList[src].push_back(Road(edgeCounter++, src, dest, dist, oneway, toll));
        if(!oneway) adjList[dest].push_back(Road(edgeCounter++, dest, src, dist, oneway, toll));
    }

    void blockRoad(string srcName, string destName) {
        int src = nameIndex[srcName];
        int dest = nameIndex[destName];
        for(auto& r : adjList[src]) if(r.dest == dest) r.isClosed = true;
        for(auto& r : adjList[dest]) if(r.dest == src) r.isClosed = true;
        cout << "[INCIDENT] Road blocked between " << srcName << " and " << destName << "\n";
    }

    void aStarSearch(string a, string b) {
        if(!nameIndex.count(a) || !nameIndex.count(b)) { cout << "Invalid cities.\n"; return; }
        int src = nameIndex[a], dest = nameIndex[b];
        
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
        unordered_map<int, double> gScore, fScore;
        unordered_map<int, int> parent;
        
        for(auto& kv : cities) { gScore[kv.first] = 999999.0; fScore[kv.first] = 999999.0; }
        
        gScore[src] = 0;
        fScore[src] = heuristic(cities[src].coords, cities[dest].coords);
        pq.push({fScore[src], src});

        while(!pq.empty()) {
            int curr = pq.top().second;
            double f = pq.top().first;
            pq.pop();
            
            if(curr == dest) {
                cout << "\n=== OPTIMAL ROUTE (A* Search) ===\n";
                vector<int> path;
                int current = dest;
                while(current != src) { path.push_back(current); current = parent[current]; }
                path.push_back(src);
                reverse(path.begin(), path.end());
                
                double totalToll = 0;
                for(size_t i=0; i<path.size(); i++) {
                    cout << cities[path[i]].name;
                    if(i < path.size()-1) {
                         for(auto& r : adjList[path[i]]) {
                             if(r.dest == path[i+1]) { totalToll += r.tollCost; cout << " --(" << r.distance << "km)-> "; break; }
                         }
                    }
                }
                cout << "\nTotal Route Distance: " << gScore[dest] << "km\nExpected Tolls: $" << totalToll << "\n";
                return;
            }
            
            if(f > fScore[curr]) continue;
            
            for(auto& edge : adjList[curr]) {
                if(edge.isClosed) continue;
                double tentative_g = gScore[curr] + edge.getEffectiveWeight();
                if(tentative_g < gScore[edge.dest]) {
                    parent[edge.dest] = curr;
                    gScore[edge.dest] = tentative_g;
                    fScore[edge.dest] = tentative_g + heuristic(cities[edge.dest].coords, cities[dest].coords);
                    pq.push({fScore[edge.dest], edge.dest});
                }
            }
        }
        cout << "No valid route found. Graph isolated.\n";
    }

    void showNetwork() {
        cout << "\n--- City Network & Congestion Parameters ---\n";
        for(auto& kv : cities) {
            cout << "City: " << kv.second.name << " [Base Traffic: " << kv.second.baseTrafficIndex << "]\n";
            for(auto& r : adjList[kv.first]) {
                cout << "  => to " << cities[r.dest].name << " | Dist: " << r.distance << " | Closed: " << (r.isClosed?"Y":"N") << "\n";
            }
        }
    }
};

// ==========================================
// MASTER APPLICATION CONTROLLER
// ==========================================
class TrafficApp {
private:
    AuthManager auth;
    TrafficEngine network;
    ParkingSystem parking;

public:
    void run() {
        cout << "=========================================================\n";
        cout << "       NEXUS TRAFFIC INTELLIGENCE SYSTEM v8.0\n";
        cout << "            (Enterprise Architecture Base)\n";
        cout << "=========================================================\n";
        
        while(!auth.login()) {}
        
        mainLoop();
    }
    
    void mainLoop() {
        while(true) {
            User* u = auth.getCurrentUser();
            if(!u) break;

            cout << "\n[ SYSTEM MENU ] - Logged in as: " << u->username << " (Role: " << (int)u->role << ") | Wallet: $" << u->walletBalance << "\n";
            cout << "1. View Active City Network & Congestion\n";
            cout << "2. Calculate Fastest Route (A* Heuristic)\n";
            cout << "3. Parking Management System\n";
            
            if(u->role == UserRole::POLICE || u->role == UserRole::ADMIN) {
                cout << "4. Report Incident & Close Road (Police Action)\n";
            }
            if(u->role == UserRole::ADMIN) {
                cout << "5. Add New Node/Infrastructure\n";
            }
            cout << "0. Log Out / Exit\n";
            cout << "Selection: ";
            int choice; cin >> choice;

            if(choice == 0) { cout << "Logging off...\n"; auth.logout(); break; }
            if(choice == 1) { network.showNetwork(); }
            if(choice == 2) {
                string src, dst;
                cout << "Origin City Name: "; cin >> src;
                cout << "Dest City Name: "; cin >> dst;
                network.aStarSearch(src, dst);
            }
            if(choice == 3) {
                parking.showAvailability();
                cout << "Reserve a spot? (y/n): "; char y; cin >> y;
                if(y=='y') parking.reserve(u);
            }
            if(choice == 4 && (u->role == UserRole::POLICE || u->role == UserRole::ADMIN)) {
                string src, dst;
                cout << "Incident Origin City: "; cin >> src;
                cout << "Incident Dest City: "; cin >> dst;
                network.blockRoad(src, dst);
            }
        }
    }
};

int main() {
    Logger::getInstance()->log("INFO", "Nexus Traffic Control Node Started.");
    TrafficApp runtime;
    runtime.run();
    return 0;
}
