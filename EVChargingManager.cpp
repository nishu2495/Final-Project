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
