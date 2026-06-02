#include "EVChargingManager.h"
#include "ACStation.h"
#include "DCFastStation.h"
#include "DCUltraFastStation.h"
#include "RegularUser.h"
#include "PremiumUser.h"
#include "FleetUser.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <cstdlib>

EVChargingManager::EVChargingManager() 
    : dataPath("data/"), 
      stationsFile("data/stations.csv"),
      usersFile("data/users.csv"),
      bookingsFile("data/bookings.csv"),
      sessionsFile("data/sessions.log"),
      backupFile("data/backup.dat") {
}

EVChargingManager::~EVChargingManager() {
    clearAllData();
}

void EVChargingManager::loadAllData() {
    std::cout << "Loading system data...\n";
    loadStations();
    loadUsers();
    loadBookings();
    loadSessions();
    std::cout << "Data loaded successfully!\n";
}

void EVChargingManager::saveAllData() {
    std::ofstream stFile(stationsFile.c_str());
    if (stFile.is_open()) {
        for (StationMap::iterator it = stations.begin(); it != stations.end(); ++it) {
            if (it->second != NULL) {
                it->second->saveToFile(stFile);
            }
        }
        stFile.close();
    }

    std::ofstream usFile(usersFile.c_str());
    if (usFile.is_open()) {
        for (UserMap::iterator it = users.begin(); it != users.end(); ++it) {
            if (it->second != NULL) {
                it->second->saveToFile(usFile);
            }
        }
        usFile.close();
    }

    std::ofstream bkFile(bookingsFile.c_str());
    if (bkFile.is_open()) {
        for (size_t i = 0; i < activeBookings.size(); ++i) {
            if (activeBookings[i] != NULL) {
                activeBookings[i]->saveToFile(bkFile);
            }
        }
        bkFile.close();
    }

    std::ofstream sesFile(sessionsFile.c_str(), std::ios::app);
    if (sesFile.is_open()) {
        // Sessions are logged individually, not batch saved
        sesFile.close();
    }
}

void EVChargingManager::loadStations() {
    std::ifstream file(stationsFile.c_str());
    if (!file.is_open()) {
        std::cout << "Creating new stations file...\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        // Parse CSV: stationID,stationType,name,latitude,longitude,powerRating,status,maxSlots
        std::istringstream iss(line);
        std::string stationID, stationType;
        char comma;
        
        iss >> stationID >> comma;
        iss >> stationType;
        
        // Determine station type and create appropriate object
        Station* station = NULL;
        if (stationType == "AC") {
            station = new ACStation();
        } else if (stationType == "DCFast") {
            station = new DCFastStation();
        } else if (stationType == "DCUltra") {
            station = new DCUltraFastStation();
        }
        
        if (station != NULL) {
            station->loadFromFile(line);
            stations[station->getStationID()] = station;
        }
    }
    file.close();
}

void EVChargingManager::loadUsers() {
    std::ifstream file(usersFile.c_str());
    if (!file.is_open()) {
        std::cout << "Creating new users file...\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        // Split line by commas
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> parts;
        while (std::getline(iss, token, ',')) {
            parts.push_back(token);
        }

        if (parts.size() < 6) continue; // malformed

        std::string userID = parts[0];
        std::string name = parts[1];
        std::string contact = parts[2];
        std::string tier = parts[3];
        double balance = atof(parts[4].c_str());
        int sessions = atoi(parts[5].c_str());

        User* user = createUserFromTier(tier, userID, name, contact, balance);
        if (user != NULL) {
            // ensure totalSessions is set
            for (int i = 0; i < sessions; ++i) user->addSession();
            users[user->getUserID()] = user;
        }
    }
    file.close();
}

void EVChargingManager::loadBookings() {
    std::ifstream file(bookingsFile.c_str());
    if (!file.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        // Parse CSV: bookingID,userID,stationID,slotDuration,startTime,endTime,status
        std::istringstream iss(line);
        std::string bookingID, userID, stationID;
        char comma;
        int duration, status;
        time_t startTime, endTime;
        
        iss >> bookingID >> comma >> userID >> comma >> stationID >> comma
            >> duration >> comma >> startTime >> comma >> endTime >> comma >> status;
        
        Station* st = findStationByID(stationID);
        User* usr = findUserByID(userID);
        
        if (st != NULL && usr != NULL) {
            Booking* booking = new Booking(bookingID, st, usr, duration);
            booking->setStartTime(startTime);
            booking->setEndTime(endTime);
            booking->setStatus(status);
            activeBookings.push_back(booking);
        }
    }
    file.close();
}

void EVChargingManager::loadSessions() {
    std::ifstream file(sessionsFile.c_str());
    if (!file.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        // Parse: sessionID,bookingID,userID,stationID,startTime,endTime,energyConsumed,finalCost,tier
        ChargingSession session;
        session.loadFromFile(line);
        completedSessions.push_back(session);
    }
    file.close();
}

// Start a charging session for a booking
bool EVChargingManager::startChargingSession(const std::string& bookingID) {
    Booking* b = findBookingByID(bookingID);
    if (b == NULL) return false;
    if (b->isActive()) return false;

    b->setStatus(1); // Active
    b->setStartTime(time(NULL));
    saveAllData();
    return true;
}

// End a charging session (admin/user end or force-stop). If the session was force-stopped
// (actual runtime less than booked duration) the user is charged for the full booking period.
bool EVChargingManager::endChargingSession(const std::string& bookingID) {
    Booking* b = findBookingByID(bookingID);
    if (b == NULL) return false;

    // Allow ending either a booked or active booking (force stop or normal end)
    if (!b->isActive() && !b->isBooked()) return false;

    time_t now = time(NULL);
    b->setEndTime(now);

    long actualSec = b->getEndTime() - b->getStartTime();
    long bookedSec = b->getDurationInSeconds();

    int billedMinutes = 0;
    if (actualSec < bookedSec) {
        // Force-stop or shortened session: charge full booking period
        billedMinutes = b->getSlotDuration();
    } else {
        billedMinutes = static_cast<int>((actualSec + 59) / 60);
    }

    double pricePerMin = b->getStation()->calculatePricePerMin(b->getUser());
    double finalCost = pricePerMin * billedMinutes;
    double energy = b->getStation()->getPowerRating() * (billedMinutes / 60.0);

    // Create a charging session record
    std::string sessID = generateID("S");
    ChargingSession session(sessID, b->getBookingID(), b->getStation(), b->getUser(), b->getSlotDuration());
    session.setStartTime(b->getStartTime());
    session.setEndTime(b->getEndTime());
    session.setEnergyConsumed(energy);
    session.setFinalCost(finalCost);

    // Log and persist
    completedSessions.push_back(session);
    logSession(session);

    // Update user and station state
    User* u = b->getUser();
    Station* s = b->getStation();
    if (u != NULL) {
        u->deductBalance(finalCost);
        u->addSession();
    }
    if (s != NULL) {
        s->decrementOccupiedSlots();
    }

    // Remove booking from active bookings and delete
    for (size_t i = 0; i < activeBookings.size(); ++i) {
        if (activeBookings[i] == b) {
            delete activeBookings[i];
            activeBookings.erase(activeBookings.begin() + i);
            break;
        }
    }

    saveAllData();
    return true;
}

void EVChargingManager::logSession(const ChargingSession& session) {
    std::ofstream sesFile(sessionsFile.c_str(), std::ios::app);
    if (sesFile.is_open()) {
        session.saveToFile(sesFile);
        sesFile.close();
    }
}

std::string EVChargingManager::generateID(const std::string& prefix) {
    std::ostringstream oss;
    oss << prefix << time(NULL) << (rand() % 1000);
    return oss.str();
}

bool EVChargingManager::addStation(Station* station) {
    if (station == NULL) return false;
    
    std::string id = station->getStationID();
    if (stations.find(id) != stations.end()) {
        std::cout << "Station with ID '" << id << "' already exists. Existing record:\n";
        Station* existing = stations[id];
        if (existing != NULL) existing->displayInfo();
        return false;  // Station already exists
    }
    
    stations[id] = station;
    saveAllData();
    return true;
}

bool EVChargingManager::removeStation(const std::string& stationID) {
    StationMap::iterator it = stations.find(stationID);
    if (it == stations.end()) return false;
    
    delete it->second;
    stations.erase(it);
    saveAllData();
    return true;
}

Station* EVChargingManager::findStationByID(const std::string& stationID) const {
    StationMap::const_iterator it = stations.find(stationID);
    if (it == stations.end()) return NULL;
    return it->second;
}

std::vector<Station*> EVChargingManager::findAvailableStations(const std::string& type) const {
    std::vector<Station*> result;
    for (StationMap::const_iterator it = stations.begin(); it != stations.end(); ++it) {
        Station* st = it->second;
        if (st != NULL && st->isAvailable() && st->getStationType() == type) {
            result.push_back(st);
        }
    }
    return result;
}

std::vector<Station*> EVChargingManager::findNearestStations(double lat, double lon, int count) const {
    std::vector<Station*> allStations;
    for (StationMap::const_iterator it = stations.begin(); it != stations.end(); ++it) {
        if (it->second != NULL) {
            allStations.push_back(it->second);
        }
    }
    
    // Simple bubble sort by distance (C++98 compatible)
    for (size_t i = 0; i < allStations.size(); ++i) {
        for (size_t j = 0; j < allStations.size() - 1 - i; ++j) {
            double dist1 = allStations[j]->getDistance(lat, lon);
            double dist2 = allStations[j+1]->getDistance(lat, lon);
            if (dist1 > dist2) {
                Station* temp = allStations[j];
                allStations[j] = allStations[j+1];
                allStations[j+1] = temp;
            }
        }
    }
    
    std::vector<Station*> result;
    for (int i = 0; i < count && i < allStations.size(); ++i) {
        result.push_back(allStations[i]);
    }
    return result;
}

void EVChargingManager::displayAllStations() const {
    if (stations.empty()) {
        std::cout << "No stations available.\n";
        return;
    }
    
    std::cout << "\n=== All Charging Stations ===\n";
    int count = 1;
    for (StationMap::const_iterator it = stations.begin(); it != stations.end(); ++it) {
        if (it->second != NULL) {
            std::cout << count << ". " << it->second->getDetails() << std::endl;
            count++;
        }
    }
}

void EVChargingManager::updateStationStatus() {
    displayAllStations();
    std::cout << "\nEnter Station ID to update: ";
    std::string stationID;
    std::getline(std::cin, stationID);
    
    Station* st = findStationByID(stationID);
    if (st == NULL) {
        std::cout << "Station not found!\n";
        return;
    }
    
    std::cout << "\nStation Status Options:\n";
    std::cout << "0 - Available\n1 - Occupied\n2 - Faulty\n3 - Maintenance\n";
    std::cout << "Enter new status: ";
    int status;
    std::cin >> status;
    std::cin.ignore();
    
    st->setStatus(status);
    saveAllData();
    std::cout << "Status updated!\n";
}

bool EVChargingManager::addUser(User* user) {
    if (user == NULL) return false;
    
    std::string id = user->getUserID();
    if (users.find(id) != users.end()) {
        std::cout << "User with ID '" << id << "' already exists. Existing record:\n";
        User* existing = users[id];
        if (existing != NULL) existing->displayInfo();
        return false;  // User already exists
    }
    
    users[id] = user;
    saveAllData();
    return true;
}

bool EVChargingManager::removeUser(const std::string& userID) {
    UserMap::iterator it = users.find(userID);
    if (it == users.end()) return false;
    
    delete it->second;
    users.erase(it);
    saveAllData();
    return true;
}

User* EVChargingManager::findUserByID(const std::string& userID) const {
    UserMap::const_iterator it = users.find(userID);
    if (it == users.end()) return NULL;
    return it->second;
}

void EVChargingManager::displayAllUsers() const {
    if (users.empty()) {
        std::cout << "No users available.\n";
        return;
    }
    
    std::cout << "\n=== All Users ===\n";
    int count = 1;
    for (UserMap::const_iterator it = users.begin(); it != users.end(); ++it) {
        if (it->second != NULL) {
            std::cout << count << ". " << it->second->getUserID() << " | "
                     << it->second->getName() << " | "
                     << it->second->getTier() << std::endl;
            count++;
        }
    }
}

std::vector<User*> EVChargingManager::findInactiveUsers(int days) const {
    std::vector<User*> result;
    time_t currentTime = time(NULL);
    time_t dayInSeconds = days * 24 * 3600;
    
    // Simplified: check if user has no recent sessions
    for (UserMap::const_iterator it = users.begin(); it != users.end(); ++it) {
        if (it->second != NULL && it->second->getTotalSessions() == 0) {
            result.push_back(it->second);
        }
    }
    return result;
}

bool EVChargingManager::bookSlot(const std::string& userID, const std::string& stationID, int minutes) {
    User* user = findUserByID(userID);
    Station* station = findStationByID(stationID);
    
    if (user == NULL || station == NULL) return false;
    if (!station->isAvailable()) return false;
    
    std::string bookingID = generateID("B");
    Booking* booking = new Booking(bookingID, station, user, minutes);
    booking->setStatus(0);  // Booked
    
    activeBookings.push_back(booking);
    station->incrementOccupiedSlots();
    saveAllData();
    return true;
}

Booking* EVChargingManager::findBookingByID(const std::string& bookingID) const {
    for (size_t i = 0; i < activeBookings.size(); ++i) {
        if (activeBookings[i] != NULL && activeBookings[i]->getBookingID() == bookingID) {
            return activeBookings[i];
        }
    }
    return NULL;
}

std::vector<Booking*> EVChargingManager::findUserBookings(const std::string& userID) const {
    std::vector<Booking*> result;
    for (size_t i = 0; i < activeBookings.size(); ++i) {
        if (activeBookings[i] != NULL && activeBookings[i]->getUser() != NULL &&
            activeBookings[i]->getUser()->getUserID() == userID) {
            result.push_back(activeBookings[i]);
        }
    }
    return result;
}

bool EVChargingManager::cancelBooking(const std::string& bookingID) {
    for (size_t i = 0; i < activeBookings.size(); ++i) {
        Booking* b = activeBookings[i];
        if (b != NULL && b->getBookingID() == bookingID) {
            b->setStatus(3); // Cancelled
            Station* s = b->getStation();
            if (s != NULL) s->decrementOccupiedSlots();
            delete b;
            activeBookings.erase(activeBookings.begin() + i);
            saveAllData();
            return true;
        }
    }
    return false;
}

void EVChargingManager::displayActiveBookings() const {
    if (activeBookings.empty()) {
        std::cout << "No active bookings." << std::endl;
        return;
    }
    std::cout << "\n=== Active Bookings ===" << std::endl;
    for (size_t i = 0; i < activeBookings.size(); ++i) {
        if (activeBookings[i] != NULL) activeBookings[i]->displayInfo();
    }
}

double EVChargingManager::calculateTotalRevenue() const {
    double total = 0.0;
    for (size_t i = 0; i < completedSessions.size(); ++i) {
        total += completedSessions[i].getFinalCost();
    }
    return total;
}

double EVChargingManager::calculateMonthlyRevenue() const {
    // Simplified: return total revenue
    return calculateTotalRevenue();
}

double EVChargingManager::calculateStationRevenue(const std::string& stationID) const {
    double total = 0.0;
    for (size_t i = 0; i < completedSessions.size(); ++i) {
        Station* s = completedSessions[i].getStation();
        if (s != NULL && s->getStationID() == stationID) {
            total += completedSessions[i].getFinalCost();
        }
    }
    return total;
}

double EVChargingManager::calculateStationUtilization(const std::string& stationID) const {
    // Simplified: percent of occupied slots
    Station* s = findStationByID(stationID);
    if (s == NULL) return 0.0;
    int max = s->getMaxSlots();
    if (max == 0) return 0.0;
    return (double)(max - s->getAvailableSlots()) / (double)max * 100.0;
}

std::vector<Station*> EVChargingManager::findTopRevenueStations(int topN) const {
    std::vector<std::pair<double, Station*> > list;
    for (StationMap::const_iterator it = stations.begin(); it != stations.end(); ++it) {
        Station* s = it->second;
        if (s == NULL) continue;
        double rev = calculateStationRevenue(s->getStationID());
        list.push_back(std::make_pair(rev, s));
    }
    // Simple selection sort (descending by revenue) to remain C++98 compatible
    for (size_t i = 0; i < list.size(); ++i) {
        size_t maxIdx = i;
        for (size_t j = i + 1; j < list.size(); ++j) {
            if (list[j].first > list[maxIdx].first) maxIdx = j;
        }
        if (maxIdx != i) {
            std::pair<double, Station*> tmp = list[i];
            list[i] = list[maxIdx];
            list[maxIdx] = tmp;
        }
    }
    std::vector<Station*> res;
    for (int i = 0; i < topN && i < (int)list.size(); ++i) res.push_back(list[i].second);
    return res;
}

void EVChargingManager::generateRevenueReport() const {
    std::cout << "\n=== Revenue Summary ===" << std::endl;
    std::cout << "Total Revenue: Rs. " << std::fixed << std::setprecision(2) << calculateTotalRevenue() << std::endl;
    for (StationMap::const_iterator it = stations.begin(); it != stations.end(); ++it) {
        Station* s = it->second;
        if (s != NULL) {
            std::cout << s->getStationID() << " - " << s->getName() << " : Rs. " << calculateStationRevenue(s->getStationID()) << std::endl;
        }
    }
}

void EVChargingManager::generateUtilizationReport() const {
    std::cout << "\n=== Station Utilization ===" << std::endl;
    for (StationMap::const_iterator it = stations.begin(); it != stations.end(); ++it) {
        Station* s = it->second;
        if (s != NULL) {
            std::cout << s->getStationID() << " - " << s->getName() << " : " << calculateStationUtilization(s->getStationID()) << "%" << std::endl;
        }
    }
}

void EVChargingManager::generateAnalyticsReport() const {
    std::cout << "\n=== System Analytics ===" << std::endl;
    std::cout << "Total Stations: " << getTotalStations() << std::endl;
    std::cout << "Total Users: " << getTotalUsers() << std::endl;
    std::cout << "Active Bookings: " << getTotalBookings() << std::endl;
    std::cout << "Completed Sessions: " << getTotalSessions() << std::endl;
    std::cout << "Total Revenue: Rs. " << calculateTotalRevenue() << std::endl;
}

std::vector<Station*> EVChargingManager::recommendStations(const std::string& userID) const {
    // Very simple: return up to 3 nearest available stations
    User* u = findUserByID(userID);
    if (u == NULL) return std::vector<Station*>();
    return findNearestStations(0.0, 0.0, 3);
}

Station* EVChargingManager::findNearestAvailableStation(double lat, double lon) const {
    std::vector<Station*> nearest = findNearestStations(lat, lon, 1);
    if (nearest.empty()) return NULL;
    return nearest[0];
}

void EVChargingManager::backupSystem(const std::string& backupFileName) {
    // Simple backup: copy data files into a single backup file (append)
    std::ofstream out(backupFileName.c_str(), std::ios::binary);
    if (!out.is_open()) return;
    std::ifstream in;
    in.open(stationsFile.c_str());
    if (in.is_open()) { out << in.rdbuf(); in.close(); }
    in.open(usersFile.c_str());
    if (in.is_open()) { out << in.rdbuf(); in.close(); }
    in.open(bookingsFile.c_str());
    if (in.is_open()) { out << in.rdbuf(); in.close(); }
    out.close();
    std::cout << "Backup created: " << backupFileName << std::endl;
}

void EVChargingManager::restoreSystem(const std::string& backupFileName) {
    // Placeholder: do nothing
    std::cout << "Restore from backup not implemented in demo." << std::endl;
}

void EVChargingManager::exportToCSV(const std::string& type) const {
    if (type == "stations") {
        std::cout << "Stations exported to " << stationsFile << std::endl;
    } else if (type == "users") {
        std::cout << "Users exported to " << usersFile << std::endl;
    }
}

int EVChargingManager::getTotalStations() const { return (int)stations.size(); }
int EVChargingManager::getTotalUsers() const { return (int)users.size(); }
int EVChargingManager::getTotalBookings() const { return (int)activeBookings.size(); }
int EVChargingManager::getTotalSessions() const { return (int)completedSessions.size(); }

void EVChargingManager::displaySystemStatus() const {
    std::cout << "Stations: " << getTotalStations() << ", Users: " << getTotalUsers() << ", Active Bookings: " << getTotalBookings() << ", Sessions: " << getTotalSessions() << std::endl;
}

void EVChargingManager::clearAllData() {
    for (StationMap::iterator it = stations.begin(); it != stations.end(); ++it) {
        delete it->second;
    }
    stations.clear();
    for (UserMap::iterator it = users.begin(); it != users.end(); ++it) {
        delete it->second;
    }
    users.clear();
    for (size_t i = 0; i < activeBookings.size(); ++i) {
        delete activeBookings[i];
    }
    activeBookings.clear();
    completedSessions.clear();
}

User* EVChargingManager::createUserFromTier(const std::string& tier, std::string id, std::string name, std::string contact, double balance) {
    if (tier == "Premium") return new PremiumUser(id, name, contact, balance);
    if (tier == "Fleet") return new FleetUser(id, name, contact, id, "Company", balance);
    return new RegularUser(id, name, contact, balance);
}

Station* EVChargingManager::createStationFromType(const std::string& type) {
    if (type == "AC") return new ACStation();
    if (type == "DCFast") return new DCFastStation();
    if (type == "DCUltra") return new DCUltraFastStation();
    return NULL;
}
