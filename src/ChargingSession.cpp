#include "../include/ChargingSession.h"
#include "../include/Station.h"
#include "../include/User.h"
#include <iostream>
#include <iomanip>

ChargingSession::ChargingSession() 
    : Booking(), energyConsumed(0.0), finalCost(0.0), sessionID("") {
}

ChargingSession::ChargingSession(std::string sessID, std::string bookID, 
                                 Station* st, User* u, int duration)
    : Booking(bookID, st, u, duration), 
      energyConsumed(0.0), finalCost(0.0), sessionID(sessID) {
}

ChargingSession::~ChargingSession() {
}

double ChargingSession::getEnergyConsumed() const {
    return energyConsumed;
}

double ChargingSession::getFinalCost() const {
    return finalCost;
}

std::string ChargingSession::getSessionID() const {
    return sessionID;
}

void ChargingSession::setEnergyConsumed(double energy) {
    if (energy >= 0) {
        energyConsumed = energy;
    }
}

void ChargingSession::setFinalCost(double cost) {
    if (cost >= 0) {
        finalCost = cost;
    }
}

double ChargingSession::calculateCost() const {
    return Booking::calculateCost();
}

void ChargingSession::displayInfo() const {
    std::cout << "\n=== Charging Session Details ===" << std::endl;
    std::cout << "Session ID: " << sessionID << std::endl;
    Booking::displayInfo();
    std::cout << "Energy Consumed: " << std::fixed << std::setprecision(2) 
              << energyConsumed << " kWh" << std::endl;
    std::cout << "Final Cost: Rs. " << finalCost << std::endl;
}

void ChargingSession::saveToFile(std::ofstream& file) const {
    if (station != NULL && user != NULL) {
        file << sessionID << "," << bookingID << "," << user->getUserID() << ","
             << station->getStationID() << "," << startTime << "," << endTime << ","
             << std::fixed << std::setprecision(2) << energyConsumed << ","
             << finalCost << "," << user->getTier() << std::endl;
    }
}

void ChargingSession::loadFromFile(const std::string& line) {
    std::istringstream iss(line);
    char comma;
    std::string userID, stationID, tier;
    iss >> sessionID >> comma >> bookingID >> comma >> userID >> comma 
        >> stationID >> comma >> startTime >> comma >> endTime >> comma
        >> energyConsumed >> comma >> finalCost >> comma >> tier;
}
