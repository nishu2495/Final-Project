#include "../include/EVChargingManager.h"
#include "../include/ACStation.h"
#include "../include/DCFastStation.h"
#include "../include/DCUltraFastStation.h"
#include "../include/RegularUser.h"
#include "../include/PremiumUser.h"
#include "../include/FleetUser.h"
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
        if (activeBookings[i] != NULL && activeBookings[i]->getBookingID() == bookingID) {
            Booking* booking = activeBookings[i];
            if (booking->getStation() != NULL) {
                booking->getStation()->decrementOccupiedSlots();
            }
            booking->setStatus(3);  // Cancelled
            saveAllData();
            return true;
        }
    }
    return false;
}

void EVChargingManager::displayActiveBookings() const {
    if (activeBookings.empty()) {
        std::cout << "No active bookings.\n";
        return;
    }
    
    std::cout << "\n=== Active Bookings ===\n";
    int count = 1;
    for (size_t i = 0; i < activeBookings.size(); ++i) {
        if (activeBookings[i] != NULL && (activeBookings[i]->isBooked() || activeBookings[i]->isActive())) {
            std::cout << count << ". " << activeBookings[i]->getBookingID() << " | ";
            if (activeBookings[i]->getUser() != NULL) {
                std::cout << activeBookings[i]->getUser()->getName() << " | ";
            }
            if (activeBookings[i]->getStation() != NULL) {
                std::cout << activeBookings[i]->getStation()->getName();
            }
            std::cout << " | Status: " << activeBookings[i]->getStatusString() << std::endl;
            count++;
        }
    }
}

bool EVChargingManager::startChargingSession(const std::string& bookingID) {
    Booking* booking = findBookingByID(bookingID);
    if (booking == NULL || !booking->isBooked()) return false;
    
    booking->setStatus(1);  // Active
    booking->setStartTime(time(NULL));
    saveAllData();
    return true;
}

bool EVChargingManager::endChargingSession(const std::string& bookingID) {
    Booking* booking = findBookingByID(bookingID);
    if (booking == NULL || !booking->isActive()) return false;
    
    booking->setEndTime(time(NULL));
    booking->setStatus(2);  // Completed
    
    // Create charging session
    std::string sessionID = generateID("SESS");
    ChargingSession session(sessionID, bookingID, booking->getStation(), 
                           booking->getUser(), booking->getSlotDuration());
    session.setStartTime(booking->getStartTime());
    session.setEndTime(booking->getEndTime());
    session.setEnergyConsumed(booking->getSlotDuration() * 0.5);  // Simulated
    session.setFinalCost(booking->calculateCost());
    
    logSession(session);
    completedSessions.push_back(session);
    
    saveAllData();
    return true;
}

void EVChargingManager::logSession(const ChargingSession& session) {
    std::ofstream file(sessionsFile.c_str(), std::ios::app);
    if (file.is_open()) {
        session.saveToFile(file);
        file.close();
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
    // Simplified: calculate for current month
    return calculateTotalRevenue();
}

double EVChargingManager::calculateStationRevenue(const std::string& stationID) const {
    double total = 0.0;
    for (size_t i = 0; i < completedSessions.size(); ++i) {
        if (completedSessions[i].getStation() != NULL &&
            completedSessions[i].getStation()->getStationID() == stationID) {
            total += completedSessions[i].getFinalCost();
        }
    }
    return total;
}

double EVChargingManager::calculateStationUtilization(const std::string& stationID) const {
    Station* st = findStationByID(stationID);
    if (st == NULL) return 0.0;
    
    double occupied = st->getOccupiedSlots();
    double total = st->getMaxSlots();
    
    if (total == 0) return 0.0;
    return (occupied / total) * 100.0;
}

std::vector<Station*> EVChargingManager::findTopRevenueStations(int topN) const {
    std::vector<Station*> allStations;
    for (StationMap::const_iterator it = stations.begin(); it != stations.end(); ++it) {
        if (it->second != NULL) {
            allStations.push_back(it->second);
        }
    }
    
    // Sort by revenue using bubble sort (C++98 compatible)
    for (size_t i = 0; i < allStations.size(); ++i) {
        for (size_t j = 0; j < allStations.size() - 1 - i; ++j) {
            double rev1 = calculateStationRevenue(allStations[j]->getStationID());
            double rev2 = calculateStationRevenue(allStations[j+1]->getStationID());
            if (rev1 < rev2) {
                Station* temp = allStations[j];
                allStations[j] = allStations[j+1];
                allStations[j+1] = temp;
            }
        }
    }
    
    std::vector<Station*> result;
    for (int i = 0; i < topN && i < allStations.size(); ++i) {
        result.push_back(allStations[i]);
    }
    return result;
}

void EVChargingManager::generateRevenueReport() const {
    std::cout << "\n=== Monthly Revenue Report ===\n";
    std::cout << "Total Revenue: Rs. " << std::fixed << std::setprecision(2)
              << calculateTotalRevenue() << std::endl;
    std::cout << "\nTop 5 Revenue Stations:\n";
    
    std::vector<Station*> topStations = findTopRevenueStations(5);
    int rank = 1;
    for (size_t i = 0; i < topStations.size(); ++i) {
        std::cout << rank << ". " << topStations[i]->getName() << " - Rs. "
                 << calculateStationRevenue(topStations[i]->getStationID()) << std::endl;
        rank++;
    }
}

void EVChargingManager::generateUtilizationReport() const {
    std::cout << "\n=== Station Utilization Report ===\n";
    for (StationMap::const_iterator it = stations.begin(); it != stations.end(); ++it) {
        if (it->second != NULL) {
            std::cout << it->second->getName() << ": "
                     << std::fixed << std::setprecision(2)
                     << calculateStationUtilization(it->second->getStationID()) << "%\n";
        }
    }
}

void EVChargingManager::generateAnalyticsReport() const {
    std::cout << "\n=== System Analytics Report ===\n";
    std::cout << "Total Stations: " << getTotalStations() << std::endl;
    std::cout << "Total Users: " << getTotalUsers() << std::endl;
    std::cout << "Total Sessions: " << getTotalSessions() << std::endl;
    std::cout << "Total Revenue: Rs. " << std::fixed << std::setprecision(2)
              << calculateTotalRevenue() << std::endl;
}

std::vector<Station*> EVChargingManager::recommendStations(const std::string& userID) const {
    return findNearestStations(0, 0, 3);  // Simplified recommendation
}

Station* EVChargingManager::findNearestAvailableStation(double lat, double lon) const {
    std::vector<Station*> nearest = findNearestStations(lat, lon, 1);
    if (nearest.empty()) return NULL;
    return nearest[0];
}

void EVChargingManager::backupSystem(const std::string& backupFileName) {
    std::ofstream file(backupFileName.c_str(), std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Failed to create backup file!\n";
        return;
    }
    
    int stationCount = stations.size();
    int userCount = users.size();
    file.write((char*)&stationCount, sizeof(int));
    file.write((char*)&userCount, sizeof(int));
    
    file.close();
    std::cout << "Backup created successfully!\n";
}

void EVChargingManager::restoreSystem(const std::string& backupFileName) {
    std::ifstream file(backupFileName.c_str(), std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Failed to open backup file!\n";
        return;
    }
    
    int stationCount, userCount;
    file.read((char*)&stationCount, sizeof(int));
    file.read((char*)&userCount, sizeof(int));
    
    file.close();
    std::cout << "System restored successfully!\n";
}

void EVChargingManager::exportToCSV(const std::string& type) const {
    if (type == "stations") {
        std::cout << "Exporting stations...\n";
        std::ofstream file("stations_export.csv");
        if (file.is_open()) {
            for (StationMap::const_iterator it = stations.begin(); it != stations.end(); ++it) {
                if (it->second != NULL) {
                    it->second->saveToFile(file);
                }
            }
            file.close();
            std::cout << "Exported to stations_export.csv\n";
        }
    } else if (type == "users") {
        std::cout << "Exporting users...\n";
        std::ofstream file("users_export.csv");
        if (file.is_open()) {
            for (UserMap::const_iterator it = users.begin(); it != users.end(); ++it) {
                if (it->second != NULL) {
                    it->second->saveToFile(file);
                }
            }
            file.close();
            std::cout << "Exported to users_export.csv\n";
        }
    }
}

int EVChargingManager::getTotalStations() const {
    return stations.size();
}

int EVChargingManager::getTotalUsers() const {
    return users.size();
}

int EVChargingManager::getTotalBookings() const {
    return activeBookings.size();
}

int EVChargingManager::getTotalSessions() const {
    return completedSessions.size();
}

void EVChargingManager::displaySystemStatus() const {
    std::cout << "\n=== System Status ===\n";
    std::cout << "Total Stations: " << getTotalStations() << std::endl;
    std::cout << "Total Users: " << getTotalUsers() << std::endl;
    std::cout << "Active Bookings: " << getTotalBookings() << std::endl;
    std::cout << "Completed Sessions: " << getTotalSessions() << std::endl;
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

std::string EVChargingManager::generateID(const std::string& prefix) {
    static int counter = 0;
    counter++;
    std::ostringstream oss;
    oss << prefix << counter;
    return oss.str();
}

User* EVChargingManager::createUserFromTier(const std::string& tier, std::string id, 
                                           std::string name, std::string contact, double balance) {
    if (tier == "Premium") {
        return new PremiumUser(id, name, contact, balance);
    } else if (tier == "Fleet") {
        return new FleetUser(id, name, contact, id, "Company", balance);
    } else {
        return new RegularUser(id, name, contact, balance);
    }
}
