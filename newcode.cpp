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
#include <random>
#include <chrono>

using namespace std;

// ==========================================
// TERMINAL UI & STYLING SUBSYSTEM
// ==========================================
namespace Color {
    const string RESET   = "\033[0m";
    const string RED     = "\033[31m";
    const string GREEN   = "\033[32m";
    const string YELLOW  = "\033[33m";
    const string BLUE    = "\033[34m";
    const string MAGENTA = "\033[35m";
    const string CYAN    = "\033[36m";
    const string WHITE   = "\033[37m";
    const string BOLD    = "\033[1m";
}

class UI {
public:
    static void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    static void printHeader() {
        clearScreen();
        cout << Color::CYAN << Color::BOLD;
        cout << "==========================================================================\n";
        cout << " ███╗   ██  ███████╗ ██╗  ██╗ ██╗   ██╗  ███████╗ \n";
        cout << " ████╗  ██  ██╔════╝ ╚██╗██╔╝ ██║   ██║  ██╔════╝ \n";
        cout << " ██╔██╗ ██  █████╗    ╚███╔╝  ██║   ██║  ███████╗ \n";
        cout << " ██║╚██╗██  ██╔══╝    ██╔██╗  ██║   ██║  ╚════██║ \n";
        cout << " ██║ ╚████  ███████╗ ██╔╝ ██╗ ╚██████╔╝  ███████║ \n";
        cout << " ╚═╝  ╚═══  ╚══════╝ ╚═╝  ╚═╝  ╚═════╝   ╚══════╝ \n";
        cout << "   T R A F F I C   I N T E L L I G E N C E   N O D E   v9.0 \n";
        cout << "==========================================================================\n" << Color::RESET;
    }

    static void printSection(const string& title) {
        cout << "\n" << Color::MAGENTA << Color::BOLD << "---[ " << title << " ]---" << Color::RESET << "\n";
    }

    static void printSuccess(const string& msg) {
        cout << Color::GREEN << "[SUCCESS] " << msg << Color::RESET << "\n";
    }

    static void printError(const string& msg) {
        cout << Color::RED << Color::BOLD << "[ERROR] " << msg << Color::RESET << "\n";
    }

    static void printWarning(const string& msg) {
        cout << Color::YELLOW << "[WARNING] " << msg << Color::RESET << "\n";
    }

    static void printInfo(const string& msg) {
        cout << Color::BLUE << "[INFO] " << msg << Color::RESET << "\n";
    }

    static void simulateProgressBar(const string& task, int ms_delay = 50) {
        cout << Color::CYAN << task << " [" << Color::RESET;
        for (int i = 0; i < 20; ++i) {
            this_thread::sleep_for(chrono::milliseconds(ms_delay));
            cout << Color::GREEN << "█" << Color::RESET << flush;
        }
        cout << Color::CYAN << "] DONE!\n" << Color::RESET;
    }

    static void printRow(const vector<string>& cols, const vector<int>& widths) {
        cout << "| ";
        for(size_t i = 0; i < cols.size(); ++i) {
            cout << setw(widths[i]) << left << cols[i] << " | ";
        }
        cout << "\n";
    }
};

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
        if(!dateStr.empty()) dateStr.pop_back();
        logFile << "[" << dateStr << "] [" << level << "] " << message << "\n";
    }
    ~Logger() { if(logFile.is_open()) logFile.close(); }
};
Logger* Logger::instance = nullptr;

// ==========================================
// WEATHER SUBSYSTEM
// ==========================================
enum class WeatherCondition { CLEAR, RAIN, SNOW, STORM, FOG };

class WeatherManager {
private:
    WeatherCondition currentWeather;
    mutex weatherMutex;
    mt19937 rng;
public:
    WeatherManager() : currentWeather(WeatherCondition::CLEAR) {
        rng.seed(time(0));
    }

    void randomizeWeather() {
        lock_guard<mutex> guard(weatherMutex);
        int val = uniform_int_distribution<int>(0, 100)(rng);
        if (val < 50) currentWeather = WeatherCondition::CLEAR;
        else if (val < 70) currentWeather = WeatherCondition::RAIN;
        else if (val < 85) currentWeather = WeatherCondition::FOG;
        else if (val < 95) currentWeather = WeatherCondition::SNOW;
        else currentWeather = WeatherCondition::STORM;
        
        string wMap[] = {"Clear", "Rain", "Snow", "Storm", "Fog"};
        Logger::getInstance()->log("WEATHER", "Weather shifted to " + wMap[(int)currentWeather]);
    }

    double getTrafficMultiplier() {
        lock_guard<mutex> guard(weatherMutex);
        switch(currentWeather) {
            case WeatherCondition::CLEAR: return 1.0;
            case WeatherCondition::RAIN: return 1.25;
            case WeatherCondition::FOG: return 1.4;
            case WeatherCondition::SNOW: return 1.7;
            case WeatherCondition::STORM: return 2.5;
            default: return 1.0;
        }
    }

    string getWeatherString() {
        lock_guard<mutex> guard(weatherMutex);
        string wMap[] = {"Clear", "Rain", "Snow", "Storm", "Fog"};
        return wMap[(int)currentWeather];
    }
    
    string getWeatherColor() {
        lock_guard<mutex> guard(weatherMutex);
        switch(currentWeather) {
            case WeatherCondition::CLEAR: return Color::CYAN;
            case WeatherCondition::RAIN: return Color::BLUE;
            case WeatherCondition::FOG: return Color::WHITE;
            case WeatherCondition::SNOW: return Color::MAGENTA;
            case WeatherCondition::STORM: return Color::RED;
            default: return Color::RESET;
        }
    }
};

// ==========================================
// USER & AUTHENTICATION SUBSYSTEM
// ==========================================
enum class UserRole { ADMIN, DRIVER, POLICE, ANALYST };

struct Vehicle {
    string plateNumber;
    string model;
    int maxSpeed;
    bool isEmergency;
    Vehicle(string p="", string m="", int s=120, bool e=false) : plateNumber(p), model(m), maxSpeed(s), isEmergency(e) {}
};

class User {
public:
    string username;
    string password;
    UserRole role;
    double walletBalance;
    int reputationPoints;
    vector<string> tickets;
    vector<Vehicle> vehicles;

    User() : username(""), password(""), role(UserRole::DRIVER), walletBalance(0.0), reputationPoints(100) {}
    User(string u, string p, UserRole r, double w) : username(u), password(p), role(r), walletBalance(w), reputationPoints(100) {}
};

class AuthManager {
private:
    unordered_map<string, User> users;
    User* currentUser;
public:
    AuthManager() : currentUser(nullptr) {
        users["admin"] = User("admin", "admin123", UserRole::ADMIN, 99999.0);
        users["driver1"] = User("driver1", "pass", UserRole::DRIVER, 500.0);
        users["driver1"].vehicles.push_back(Vehicle("ABC-1234", "Sedan", 140, false));
        users["cop1"] = User("cop1", "pass", UserRole::POLICE, 0.0);
        users["ems1"] = User("ems1", "pass", UserRole::DRIVER, 1000.0);
        users["ems1"].vehicles.push_back(Vehicle("EMS-001", "Ambulance", 160, true));
        users["analyst"] = User("analyst", "data", UserRole::ANALYST, 0.0);
    }
    
    bool login() {
        UI::printHeader();
        UI::printSection("GLOBAL AUTHENTICATION GATEWAY");
        string user, pass;
        int attempts = 0;
        while(attempts < 3) {
            cout << Color::BOLD << "Username: " << Color::RESET; cin >> user;
            cout << Color::BOLD << "Password: " << Color::RESET; cin >> pass;
            
            UI::simulateProgressBar("Authenticating Credentials", 20);
            if(users.count(user) && users[user].password == pass) {
                currentUser = &users[user];
                UI::printSuccess("Access Granted. Welcome, " + user + ".");
                this_thread::sleep_for(chrono::seconds(1));
                Logger::getInstance()->log("INFO", "User logged in: " + user);
                return true;
            }
            UI::printError("Invalid Security Clearances. Attempt " + to_string(attempts+1) + " of 3.");
            attempts++;
        }
        return false;
    }
    
    User* getCurrentUser() { return currentUser; }
    void logout() { currentUser = nullptr; }
    unordered_map<string, User>& getAllUsers() { return users; }
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
    int population;
    int baseTrafficIndex;
    Coordinate coords;
    
    City() : id(-1), name(""), population(0), baseTrafficIndex(0) {}
    City(int i, string n, int pop, int t, Coordinate c) : id(i), name(n), population(pop), baseTrafficIndex(t), coords(c) {}
};

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
    int speedLimit;
    
    Road(int id, int s, int d, double dist, bool oneWay, double toll, int spLimit=80) 
        : id(id), src(s), dest(d), distance(dist), currentTrafficLevel(10), isOneWay(oneWay), tollCost(toll), isClosed(false), speedLimit(spLimit) {}
        
    double getEffectiveWeight(bool isEmergency, double weatherMultiplier) const {
        if(isClosed && !isEmergency) return 999999.9;
        double trafficPenalty = 1.0 + (currentTrafficLevel / 100.0);
        return (distance / ((double)speedLimit / 60.0)) * trafficPenalty * weatherMultiplier; // weight is time in mins
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
    double totalRevenue;
public:
    ParkingSystem(int max_s = 50) : maxSlots(max_s), totalRevenue(0.0) {
        for(int i=0; i<max_s; i++) {
            capacity.push_back({i, "", 0, 0, (i<10 ? ParkingTier::VIP : (i<20 ? ParkingTier::PREMIUM : ParkingTier::STANDARD)), false});
        }
    }
    
    void showAvailability() {
        UI::printSection("PARKING GRID STATUS");
        int avail = 0;
        for(auto& s : capacity) if(!s.active) avail++;
        
        UI::printInfo(to_string(avail) + " out of " + to_string(maxSlots) + " zones open.");
        vector<int> widths = {10, 15, 15, 20};
        UI::printRow({"Slot ID", "Tier", "Status", "Owner/Remaining"}, widths);
        cout << string(70, '-') << "\n";
        
        for(auto& s : capacity) {
            string tierStr = (s.tier == ParkingTier::VIP) ? "VIP" : (s.tier == ParkingTier::PREMIUM) ? "PREMIUM" : "STANDARD";
            string statStr = s.active ? Color::RED + "OCCUPIED" + Color::RESET : Color::GREEN + "AVAILABLE" + Color::RESET;
            string ownStr = s.active ? s.owner : "N/A";
            UI::printRow({to_string(s.slotId), tierStr, statStr, ownStr}, widths);
        }
    }

    void reserve(User* user) {
        if(!user || user->role != UserRole::DRIVER) { UI::printError("Only licensed drivers can secure zones."); return; }
        UI::printSection("PARKING RESERVATION DESK");
        cout << "[Slot Tiers]\n 1. " << Color::MAGENTA << "VIP" << Color::RESET << " ($50/h)\n 2. " << Color::BLUE << "Premium" << Color::RESET << " ($20/h)\n 3. " << Color::GREEN << "Standard" << Color::RESET << " ($5/h)\nTarget Tier Setup: ";
        int c; cin >> c;
        ParkingTier pt = (c==1) ? ParkingTier::VIP : (c==2) ? ParkingTier::PREMIUM : ParkingTier::STANDARD;
        
        for(auto& slot : capacity) {
            if(!slot.active && slot.tier == pt) {
                cout << "-> Auto-assigned Slot #" << slot.slotId << ". Duration Index (Hours): ";
                int h; cin >> h;
                double cost = h * (pt == ParkingTier::VIP ? 50 : (pt == ParkingTier::PREMIUM ? 20 : 5));
                if(user->walletBalance >= cost) {
                    UI::simulateProgressBar("Transacting Payment");
                    user->walletBalance -= cost;
                    totalRevenue += cost;
                    slot.active = true;
                    slot.owner = user->username;
                    slot.durationHours = h;
                    slot.startTime = time(0);
                    UI::printSuccess("Block Confirmed. Remaining Nexus Credits: $" + to_string(user->walletBalance));
                    Logger::getInstance()->log("INFO", user->username + " reserved parking slot " + to_string(slot.slotId));
                    return;
                } else {
                    UI::printError("Insufficient Nexus Credits. Cost constraint: $" + to_string(cost));
                    return;
                }
            }
        }
        UI::printWarning("Sector Full. No zones matching that tier.");
    }

    void viewRevenue() {
        UI::printSection("ECONOMIC METRICS (PARKING)");
        cout << Color::GREEN << ">> Gross Parking Income: $" << fixed << setprecision(2) << totalRevenue << Color::RESET << "\n";
    }
};

// ==========================================
// LAW ENFORCEMENT & TICKETING
// ==========================================
class PoliceDepartment {
public:
    void issueTicket(AuthManager& authManager, const string& targetUser, double fineAmount, const string& reason) {
        auto& users = authManager.getAllUsers();
        UI::printSection("LEO CITATION DESPATCH");
        UI::simulateProgressBar("Filing Citation with Central Database");
        
        if(users.find(targetUser) != users.end()) {
            users[targetUser].walletBalance -= fineAmount;
            users[targetUser].reputationPoints -= 15;
            string ticket = "Fine: $" + to_string(fineAmount) + " | Violation code: " + reason;
            users[targetUser].tickets.push_back(ticket);
            UI::printSuccess("Citation successfully enforced on target (" + targetUser + ").");
            Logger::getInstance()->log("LAW", "Ticket issued to " + targetUser + " for $" + to_string(fineAmount));
        } else {
            UI::printError("Target anomaly. Driver identity not found.");
        }
    }

    void viewTickets(User* u) {
        UI::printSection("DRIVING RECORD (" + u->username + ")");
        string clr = u->reputationPoints > 80 ? Color::GREEN : (u->reputationPoints > 50 ? Color::YELLOW : Color::RED);
        cout << ">> Reputation Index: " << clr << u->reputationPoints << "/100" << Color::RESET << "\n\n";
        
        if(u->tickets.empty()) {
            UI::printInfo("No active infractions arrayed. Record clean.");
        } else {
            cout << Color::RED << "--- VIOLATION HISTORY ---\n" << Color::RESET;
            for(const auto& t : u->tickets) cout << " [!] " << t << "\n";
        }
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
    WeatherManager* weatherRef;
    double totalTollRevenue;

    double heuristic(Coordinate a, Coordinate b) {
        return sqrt(pow(a.lat - b.lat, 2) + pow(a.lon - b.lon, 2)) * 111.0; 
    }

public:
    TrafficEngine(WeatherManager* wm) : weatherRef(wm), totalTollRevenue(0.0) { initMockData(); }

    void initMockData() {
        addCity(0, "Metropolis", 5000000, Coordinate(40.7128, -74.0060));
        addCity(1, "Gotham", 8000000, Coordinate(41.8781, -87.6298));
        addCity(2, "StarCity", 1200000, Coordinate(34.0522, -118.2437));
        addCity(3, "CentralCity", 2500000, Coordinate(47.6062, -122.3321));
        addCity(4, "CoastCity", 900000, Coordinate(32.7157, -117.1611));
        addCity(5, "KeystoneCity", 1500000, Coordinate(39.0997, -94.5786));
        
        addRoad("Metropolis", "Gotham", 800, false, 15.0, 120);
        addRoad("Gotham", "StarCity", 2000, false, 25.0, 100);
        addRoad("Metropolis", "StarCity", 2800, false, 50.0, 130);
        addRoad("Gotham", "CentralCity", 600, false, 10.0, 80);
        addRoad("CentralCity", "CoastCity", 1800, false, 20.0, 110);
        addRoad("StarCity", "CoastCity", 200, false, 5.0, 90);
        addRoad("KeystoneCity", "Gotham", 750, false, 12.0, 100);
    }

    void addCity(int id, string name, int pop, Coordinate c) {
        cities[id] = City(id, name, pop, 10, c);
        nameIndex[name] = id;
        adjList[id] = vector<Road>();
    }

    void addRoad(string srcName, string destName, double dist, bool oneway, double toll, int speedLim) {
        if(!nameIndex.count(srcName) || !nameIndex.count(destName)) return;
        int src = nameIndex[srcName];
        int dest = nameIndex[destName];
        adjList[src].push_back(Road(edgeCounter++, src, dest, dist, oneway, toll, speedLim));
        if(!oneway) adjList[dest].push_back(Road(edgeCounter++, dest, src, dist, oneway, toll, speedLim));
    }

    void blockRoad(string srcName, string destName) {
        if(!nameIndex.count(srcName) || !nameIndex.count(destName)) { UI::printError("Invalid Coordinates mapped."); return; }
        int src = nameIndex[srcName];
        int dest = nameIndex[destName];
        for(auto& r : adjList[src]) if(r.dest == dest) r.isClosed = true;
        for(auto& r : adjList[dest]) if(r.dest == src) r.isClosed = true;
        UI::printWarning("BARRICADE DEPLOYED between " + srcName + " and " + destName);
        Logger::getInstance()->log("INCIDENT", "Road closure: " + srcName + " to " + destName);
    }

    void openRoad(string srcName, string destName) {
        if(!nameIndex.count(srcName) || !nameIndex.count(destName)) { UI::printError("Invalid Coordinates mapped."); return; }
        int src = nameIndex[srcName];
        int dest = nameIndex[destName];
        for(auto& r : adjList[src]) if(r.dest == dest) r.isClosed = false;
        for(auto& r : adjList[dest]) if(r.dest == src) r.isClosed = false;
        UI::printSuccess("BARRICADE RETRACTED clearing " + srcName + " to " + destName);
        Logger::getInstance()->log("INCIDENT", "Road unblocked: " + srcName + " to " + destName);
    }

    void simulateLiveTraffic() {
        mt19937 rng(time(0));
        for(auto& kv : adjList) {
            for(auto& r : kv.second) {
                int delta = uniform_int_distribution<int>(-15, 20)(rng);
                r.currentTrafficLevel = max(0, min(100, r.currentTrafficLevel + delta));
            }
        }
        weatherRef->randomizeWeather();
    }

    void findMinimumSpanningTree() {
        UI::printSection("INFRASTRUCTURE ALGORITHM (PRIM'S MST)");
        if(cities.empty()) return;
                
        unordered_set<int> inMST;
        priority_queue<pair<double, pair<int, int>>, vector<pair<double, pair<int, int>>>, greater<>> pq;
        double totalInfraCost = 0;

        int startNode = cities.begin()->first;
        inMST.insert(startNode);
        for(auto& r : adjList[startNode]) pq.push({r.distance, {startNode, r.dest}});

        UI::simulateProgressBar("Analysing Geographic Topologies", 100);
        
        while(!pq.empty() && inMST.size() < cities.size()) {
            auto edge = pq.top();
            pq.pop();
            double dist = edge.first;
            int u = edge.second.first;
            int v = edge.second.second;

            if(inMST.count(v)) continue;
                        
            inMST.insert(v);
            totalInfraCost += dist;
            cout << Color::CYAN << "  >> Laying Matrix Link: " << Color::RESET 
                 << setw(15) << left << cities[u].name << " <=> " << setw(15) << left << cities[v].name 
                 << " | Span: " << Color::YELLOW << dist << " km" << Color::RESET << "\n";

            for(auto& r : adjList[v]) {
                if(!inMST.count(r.dest)) {
                    pq.push({r.distance, {v, r.dest}});
                }
            }
        }
        cout << "\n" << Color::GREEN << Color::BOLD << "=> MINIMAL GRAPH DISTANCE TO CONNECT ALL NODES: " << totalInfraCost << " km" << Color::RESET << "\n";
    }

    void findReachableRange(string startName, double maxTimeMins, bool isEmergency) {
        if(!nameIndex.count(startName)) { UI::printError("Origin target unknown."); return; }
        int src = nameIndex[startName];
                
        UI::printSection("BFS RADAR SCAN (" + startName + " -> " + to_string(maxTimeMins) + " mins max)");
        UI::simulateProgressBar("Emitting Radar Pulses", 30);
        
        unordered_map<int, double> minTimes;
        queue<pair<int, double>> q;
                
        q.push({src, 0.0});
        minTimes[src] = 0.0;
        double wMulti = weatherRef->getTrafficMultiplier();

        while(!q.empty()) {
            int u = q.front().first;
            double t = q.front().second;
            q.pop();

            for(auto& r : adjList[u]) {
                double travelTime = r.getEffectiveWeight(isEmergency, wMulti);
                if(t + travelTime <= maxTimeMins) {
                    if(!minTimes.count(r.dest) || t + travelTime < minTimes[r.dest]) {
                        minTimes[r.dest] = t + travelTime;
                        q.push({r.dest, t + travelTime});
                    }
                }
            }
        }

        for(auto& kv : minTimes) {
            if(kv.first != src) {
                cout << Color::GREEN << " [FOUND] " << Color::RESET << setw(15) << left << cities[kv.first].name 
                     << " -> ETA: " << Color::YELLOW << fixed << setprecision(1) << kv.second << " mins" << Color::RESET << "\n";
            }
        }
    }

    void aStarSearch(User* u, string a, string b, bool isEmergency) {
        if(!nameIndex.count(a) || !nameIndex.count(b)) { UI::printError("Routing vectors corrupted."); return; }
        int src = nameIndex[a], dest = nameIndex[b];
        
        UI::printSection("A* HEURISTIC PATHFINDER MODULE");
        UI::simulateProgressBar("Calculating Optimal Graph Weights");
        
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
        unordered_map<int, double> gScore, fScore;
        unordered_map<int, int> parent;
        
        for(auto& kv : cities) { gScore[kv.first] = 99999999.0; fScore[kv.first] = 99999999.0; }
        
        gScore[src] = 0;
        fScore[src] = heuristic(cities[src].coords, cities[dest].coords);
        pq.push({fScore[src], src});

        double wMulti = weatherRef->getTrafficMultiplier();

        while(!pq.empty()) {
            int curr = pq.top().second;
            double f = pq.top().first;
            pq.pop();
                        
            if(curr == dest) {
                cout << "\n" << Color::MAGENTA << "=== COMPILED ROUTE VECTOR (" << (isEmergency?"EMERGENCY OVERRIDE MODE":"Standard Constraints") << ") ===" << Color::RESET << "\n";
                cout << "-> Weather Status: " << weatherRef->getWeatherColor() << weatherRef->getWeatherString() << Color::RESET << " (Severity Mult: x" << wMulti << ")\n\n";
                                
                vector<int> path;
                int current = dest;
                while(current != src) { path.push_back(current); current = parent[current]; }
                path.push_back(src);
                reverse(path.begin(), path.end());
                                
                double totalToll = 0;
                double totalTime = gScore[dest];

                cout << Color::CYAN << "  ";
                for(size_t i=0; i<path.size(); i++) {
                    cout << cities[path[i]].name;
                    if(i < path.size()-1) {
                         for(auto& r : adjList[path[i]]) {
                             if(r.dest == path[i+1]) { 
                                 totalToll += r.tollCost; 
                                 if(r.isClosed && isEmergency) cout << Color::RED << " ==[BARRICADE_BYPASS]==> " << Color::CYAN;
                                 else cout << Color::CYAN << " ==> ";
                                 break; 
                             }
                         }
                    }
                }
                cout << Color::RESET << "\n\n";
                                
                cout << ">> Target ETA Profile:   " << Color::YELLOW << fixed << setprecision(1) << totalTime << " minutes" << Color::RESET << ".\n";
                cout << ">> Total Graph Tolls:    " << Color::RED << "$" << totalToll << Color::RESET << "\n\n";

                if (u && !isEmergency && u->walletBalance >= totalToll) {
                    cout << Color::BOLD << "Commence navigation & debit tolls? [y/n]: " << Color::RESET;
                    char c; cin >> c;
                    if(c=='y') {
                        u->walletBalance -= totalToll;
                        totalTollRevenue += totalToll;
                        UI::printSuccess("Transaction Completed. Safe Traversal.");
                        Logger::getInstance()->log("ROUTING", u->username + " traversed " + a + "->" + b);
                    }
                } else if (u && !isEmergency) {
                    UI::printError("Wallet funds insufficient for route economy constraints.");
                }
                return;
            }
            
            if(f > fScore[curr]) continue;
            
            for(auto& edge : adjList[curr]) {
                if(edge.isClosed && !isEmergency) continue;
                double tentative_g = gScore[curr] + edge.getEffectiveWeight(isEmergency, wMulti);
                if(tentative_g < gScore[edge.dest]) {
                    parent[edge.dest] = curr;
                    gScore[edge.dest] = tentative_g;
                    fScore[edge.dest] = tentative_g + heuristic(cities[edge.dest].coords, cities[dest].coords);
                    pq.push({fScore[edge.dest], edge.dest});
                }
            }
        }
        UI::printError("Pathfinding engine isolated. No viable traversal vectors found.");
    }

    void showNetwork() {
        UI::printSection("LIVE MATRIX TOPOLOGY MODULE");
        vector<int> widths = {15, 18, 12, 15, 10};
        UI::printRow({"Origin Node", "Target Vector", "Congestion", "Speed Limit", "Blockade"}, widths);
        cout << string(80, '-') << "\n";
        
        for(auto& kv : cities) {
            for(auto& r : adjList[kv.first]) {
                string srcStr = kv.second.name;
                string destStr = cities[r.dest].name;
                
                string cCol = r.currentTrafficLevel > 75 ? Color::RED : (r.currentTrafficLevel > 40 ? Color::YELLOW : Color::GREEN);
                string trfStr = cCol + to_string(r.currentTrafficLevel) + "%" + Color::RESET;
                
                string spdStr = to_string(r.speedLimit) + " km/h";
                string lckStr = r.isClosed ? (Color::RED + "ACTIVE" + Color::RESET) : "CLEAR";
                
                // Print with ANSI colors accounted for via direct spacing rather than UI format to preserve col widths.
                cout << "| " << setw(15) << left << srcStr 
                     << " | " << setw(18) << left << destStr 
                     << " | " << trfStr << string(12 - to_string(r.currentTrafficLevel).length() - 1, ' ') 
                     << " | " << setw(15) << left << spdStr 
                     << " | " << lckStr << string(r.isClosed ? 4 : 5, ' ') << " |\n";
            }
        }
    }

    void viewTolls() {
        UI::printSection("ECONOMIC METRICS (TOLLS)");
        cout << Color::GREEN << ">> Gross Highway Toll Income: $" << fixed << setprecision(2) << totalTollRevenue << Color::RESET << "\n";
    }
        
    int getCityId(string name) {
        return nameIndex.count(name) ? nameIndex[name] : -1;
    }
};

// ==========================================
// MASTER APPLICATION CONTROLLER
// ==========================================
class TrafficApp {
private:
    AuthManager auth;
    WeatherManager weather;
    TrafficEngine network;
    ParkingSystem parking;
    PoliceDepartment police;
    bool systemRunning;
    thread simThread;

    void backgroundSimulation() {
        while(systemRunning) {
            this_thread::sleep_for(chrono::seconds(10));
            network.simulateLiveTraffic();
        }
    }

public:
    TrafficApp() : network(&weather), systemRunning(true) {
        simThread = thread(&TrafficApp::backgroundSimulation, this);
    }

    ~TrafficApp() {
        systemRunning = false;
        if(simThread.joinable()) simThread.join();
    }

    void run() {
        while(true) {
            if(auth.login()) mainLoop();
            else cout << "Login Failed. Terminal resetting...\n";
            
            cout << "\n" << Color::MAGENTA << ">> System Shutdown Command? [y/n]: " << Color::RESET;
            char c; cin >> c;
            if(c == 'y') break;
        }
    }
    
    void mainLoop() {
        while(true) {
            User* u = auth.getCurrentUser();
            if(!u) break;

            UI::printHeader();
            cout << "  Current System Weather: " << weather.getWeatherColor() << Color::BOLD << weather.getWeatherString() << Color::RESET << "\n";
            cout << "  Operator Terminal: " << Color::CYAN << u->username << Color::RESET << " | Access Level: " << Color::YELLOW << (int)u->role << Color::RESET << " | Funds: " << Color::GREEN << "$" << fixed << setprecision(2) << u->walletBalance << Color::RESET << "\n";
            
            UI::printSection("NEXUS CORE FUNCTIONALITY");
            cout << "  [" << Color::CYAN << "1" << Color::RESET << "] View Active City Network Tracker\n";
            cout << "  [" << Color::CYAN << "2" << Color::RESET << "] Calculate Fastest Route (A* Heuristic Search)\n";
            cout << "  [" << Color::CYAN << "3" << Color::RESET << "] Parking Grid Management\n";
            cout << "  [" << Color::CYAN << "4" << Color::RESET << "] Radar Scan (BFS Max Distance / Time Reach)\n";
            cout << "  [" << Color::CYAN << "5" << Color::RESET << "] View Personal Driving Record\n";
            
            if(u->role == UserRole::POLICE || u->role == UserRole::ADMIN) {
                UI::printSection("LAW ENFORCEMENT PROTOCOLS");
                cout << "  [" << Color::RED << "6" << Color::RESET << "] Report Incident & Blockade Highway\n";
                cout << "  [" << Color::GREEN << "7" << Color::RESET << "] Resolve Incident & Retract Blockade\n";
                cout << "  [" << Color::YELLOW << "8" << Color::RESET << "] Issue Driver Infraction/Citation\n";
            }
            if(u->role == UserRole::ADMIN || u->role == UserRole::ANALYST) {
                UI::printSection("ADMINISTRATIVE & FINANCIAL SUITE");
                cout << "  [" << Color::MAGENTA << "9" << Color::RESET << "] Process Minimal Infrastructure Spanning Tree (Prim's)\n";
                cout << "  [" << Color::MAGENTA << "10" << Color::RESET << "] Retrieve Global Revenue Audits\n";
                cout << "  [" << Color::MAGENTA << "11" << Color::RESET << "] Spawn New Graph Node (City)\n";
            }
            
            cout << "\n  [" << Color::BLUE << "0" << Color::RESET << "] Disconnect Session Operator\n";
            
            cout << "\n" << Color::BOLD << ">> INPUT DISPATCH: " << Color::RESET;
            int choice; cin >> choice;

            if(choice == 0) { UI::printWarning("Disconnecting feed..."); auth.logout(); break; }
            else if(choice == 1) { UI::clearScreen(); network.showNetwork(); }
            else if(choice == 2) {
                string src, dst;
                cout << Color::BOLD << "Origin City Name: " << Color::RESET; cin >> src;
                cout << Color::BOLD << "Dest Target Name: " << Color::RESET; cin >> dst;
                bool isEmergency = false;
                if(!u->vehicles.empty() && u->vehicles[0].isEmergency) {
                    cout << Color::RED << "Deploy Emergency Siren Mode? (Bypass Blockades) [y/n]: " << Color::RESET; 
                    char e; cin >> e; isEmergency = (e=='y');
                }
                network.aStarSearch(u, src, dst, isEmergency);
            }
            else if(choice == 3) {
                UI::clearScreen();
                parking.showAvailability();
                cout << "\nSecure a parking zone? [y/n]: "; char y; cin >> y;
                if(y=='y') parking.reserve(u);
            }
            else if(choice == 4) {
                string src; double t;
                cout << Color::BOLD << "Origin City Node: " << Color::RESET; cin >> src;
                cout << Color::BOLD << "Max Travel Time budget (minutes): " << Color::RESET; cin >> t;
                network.findReachableRange(src, t, false);
            }
            else if(choice == 5) {
                UI::clearScreen();
                police.viewTickets(u);
            }
            else if(choice == 6 && (u->role == UserRole::POLICE || u->role == UserRole::ADMIN)) {
                string src, dst;
                cout << Color::RED << "Incident Origin City: " << Color::RESET; cin >> src;
                cout << Color::RED << "Incident Dest City: " << Color::RESET; cin >> dst;
                network.blockRoad(src, dst);
            }
            else if(choice == 7 && (u->role == UserRole::POLICE || u->role == UserRole::ADMIN)) {
                string src, dst;
                cout << Color::GREEN << "Resolution Origin City: " << Color::RESET; cin >> src;
                cout << Color::GREEN << "Resolution Dest City: " << Color::RESET; cin >> dst;
                network.openRoad(src, dst);
            }
            else if(choice == 8 && (u->role == UserRole::POLICE || u->role == UserRole::ADMIN)) {
                string target, reason; double fine;
                cout << Color::YELLOW << "Target Username: " << Color::RESET; cin >> target;
                cout << Color::YELLOW << "Fine Amount: $" << Color::RESET; cin >> fine;
                cout << Color::YELLOW << "Violation Code: " << Color::RESET; cin >> reason;
                police.issueTicket(auth, target, fine, reason);
            }
            else if(choice == 9 && (u->role == UserRole::ADMIN || u->role == UserRole::ANALYST)) {
                UI::clearScreen();
                network.findMinimumSpanningTree();
            }
            else if(choice == 10 && (u->role == UserRole::ADMIN || u->role == UserRole::ANALYST)) {
                UI::clearScreen();
                network.viewTolls();
                parking.viewRevenue();
            }
            else if(choice == 11 && u->role == UserRole::ADMIN) {
                string name; int pop; double lat, lon;
                UI::printSection("TOPOLOGICAL INJECTION PROTOCOL");
                cout << "New City Name: "; cin >> name;
                cout << "Population: "; cin >> pop;
                cout << "Latitude: "; cin >> lat;
                cout << "Longitude: "; cin >> lon;
                                
                int newId = 1000 + rand() % 1000;
                network.addCity(newId, name, pop, Coordinate(lat, lon));
                UI::printSuccess("Topological Node Generated. Run Network Tracker to verify injection.");
            }
            
            cout << "\n" << Color::CYAN << "Press [ENTER] to return to hub..." << Color::RESET;
            cin.ignore(); cin.get();
        }
    }
};

int main() {
    Logger::getInstance()->log("INFO", "Nexus Traffic Control Node v9.0 Initialized.");
    TrafficApp runtime;
    runtime.run();
    return 0;
}
