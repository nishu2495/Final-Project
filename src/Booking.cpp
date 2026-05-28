#include "../include/Booking.h"
#include "../include/Station.h"
#include "../include/User.h"
#include <iostream>
#include <iomanip>
#include <ctime>

Booking::Booking() 
    : bookingID(""), station(NULL), user(NULL), 
      startTime(0), endTime(0), slotDuration(0), status(0) {
}

Booking::Booking(std::string id, Station* st, User* u, int duration)
    : bookingID(id), station(st), user(u), 
      startTime(time(NULL)), endTime(0), slotDuration(duration), status(0) {
}

Booking::~Booking() {
}

std::string Booking::getBookingID() const {
    return bookingID;
}

Station* Booking::getStation() const {
    return station;
}

User* Booking::getUser() const {
    return user;
}

time_t Booking::getStartTime() const {
    return startTime;
}

time_t Booking::getEndTime() const {
    return endTime;
}

int Booking::getSlotDuration() const {
    return slotDuration;
}

int Booking::getStatus() const {
    return status;
}

std::string Booking::getStatusString() const {
    switch (status) {
        case 0: return "Booked";
        case 1: return "Active";
        case 2: return "Completed";
        case 3: return "Cancelled";
        default: return "Unknown";
    }
}

void Booking::setStatus(int s) {
    if (s >= 0 && s <= 3) {
        status = s;
    }
}

void Booking::setStartTime(time_t t) {
    startTime = t;
}

void Booking::setEndTime(time_t t) {
    endTime = t;
}

bool Booking::isActive() const {
    return status == 1;
}

bool Booking::isBooked() const {
    return status == 0;
}

long Booking::getDurationInSeconds() const {
    return slotDuration * 60;
}

double Booking::calculateCost() const {
    if (station == NULL || user == NULL) return 0.0;
    double pricePerMin = station->calculatePricePerMin(user);
    return pricePerMin * slotDuration;
}

void Booking::displayInfo() const {
    std::cout << "\nBooking ID: " << bookingID << std::endl;
    if (station != NULL) {
        std::cout << "Station: " << station->getName() << " (" 
                  << station->getStationID() << ")" << std::endl;
    }
    if (user != NULL) {
        std::cout << "User: " << user->getName() << " (" 
                  << user->getUserID() << ")" << std::endl;
    }
    std::cout << "Duration: " << slotDuration << " minutes" << std::endl;
    std::cout << "Status: " << getStatusString() << std::endl;
    std::cout << "Estimated Cost: Rs. " << std::fixed << std::setprecision(2)
              << calculateCost() << std::endl;
}

void Booking::saveToFile(std::ofstream& file) const {
    if (station != NULL && user != NULL) {
        file << bookingID << "," << user->getUserID() << "," 
             << station->getStationID() << "," << slotDuration << ","
             << startTime << "," << endTime << "," << status << std::endl;
    }
}

void Booking::loadFromFile(const std::string& line) {
    std::istringstream iss(line);
    char comma;
    std::string userID, stationID;
    iss >> bookingID >> comma >> userID >> comma >> stationID >> comma
        >> slotDuration >> comma >> startTime >> comma >> endTime >> comma >> status;
}
