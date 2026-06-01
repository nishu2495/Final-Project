#include "Station.h"
#include "User.h"
#include <iostream>
#include <iomanip>
#include <cmath>

Station::Station() 
    : stationID(""), stationType(""), name(""), latitude(0.0), 
      longitude(0.0), powerRating(0.0), status(0), maxSlots(0), occupiedSlots(0) {
}

Station::Station(std::string id, std::string type, std::string n, 
                 double lat, double lon, double power, int slots)
    : stationID(id), stationType(type), name(n), latitude(lat), 
      longitude(lon), powerRating(power), status(0), maxSlots(slots), occupiedSlots(0) {
}

Station::~Station() {
}

std::string Station::getStationID() const {
    return stationID;
}

std::string Station::getStationType() const {
    return stationType;
}

std::string Station::getName() const {
    return name;
}

double Station::getLatitude() const {
    return latitude;
}

double Station::getLongitude() const {
    return longitude;
}

double Station::getPowerRating() const {
    return powerRating;
}

int Station::getStatus() const {
    return status;
}

int Station::getMaxSlots() const {
    return maxSlots;
}

int Station::getOccupiedSlots() const {
    return occupiedSlots;
}

int Station::getAvailableSlots() const {
    return maxSlots - occupiedSlots;
}

void Station::setStatus(int s) {
    if (s >= 0 && s <= 3) {
        status = s;
    }
}

void Station::setOccupiedSlots(int slots) {
    if (slots >= 0 && slots <= maxSlots) {
        occupiedSlots = slots;
    }
}

void Station::incrementOccupiedSlots() {
    if (occupiedSlots < maxSlots) {
        occupiedSlots++;
    }
}

void Station::decrementOccupiedSlots() {
    if (occupiedSlots > 0) {
        occupiedSlots--;
    }
}

bool Station::isAvailable() const {
    return status == 0 && occupiedSlots < maxSlots;
}

double Station::getDistance(double lat, double lon) const {
    // Euclidean distance (simplified)
    double lat_diff = latitude - lat;
    double lon_diff = longitude - lon;
    return std::sqrt(lat_diff * lat_diff + lon_diff * lon_diff);
}

void Station::displayInfo() const {
    std::cout << "\nStation ID: " << stationID << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Type: " << stationType << std::endl;
    std::cout << "Location: (" << std::fixed << std::setprecision(4) 
              << latitude << ", " << longitude << ")" << std::endl;
    std::cout << "Power Rating: " << powerRating << " kW" << std::endl;
    std::cout << "Status: ";
    switch (status) {
        case 0: std::cout << "Available"; break;
        case 1: std::cout << "Occupied"; break;
        case 2: std::cout << "Faulty"; break;
        case 3: std::cout << "Maintenance"; break;
        default: std::cout << "Unknown"; break;
    }
    std::cout << std::endl;
    std::cout << "Occupied Slots: " << occupiedSlots << "/" << maxSlots << std::endl;
}

std::string Station::getDetails() const {
    std::ostringstream oss;
    oss << stationID << " | " << name << " (" << stationType << ") | "
        << "Power: " << powerRating << "kW | "
        << "Available: " << getAvailableSlots() << "/" << maxSlots;
    return oss.str();
}

void Station::saveToFile(std::ofstream& file) const {
    file << stationID << "," << stationType << "," << name << ","
         << std::fixed << std::setprecision(4) << latitude << ","
         << longitude << "," << powerRating << "," << status << ","
         << maxSlots << std::endl;
}

void Station::loadFromFile(const std::string& line) {
    std::istringstream iss(line);
    char comma;
    iss >> stationID >> comma >> stationType >> comma >> name >> comma
        >> latitude >> comma >> longitude >> comma >> powerRating >> comma
        >> status >> comma >> maxSlots;
    occupiedSlots = 0;
}
