#include "ACStation.h"
#include "User.h"
#include <iostream>
#include <sstream>

ACStation::ACStation() 
    : Station("", "AC", "", 0.0, 0.0, 7.2, 0), baseRatePerMin(0.5) {
}

ACStation::ACStation(std::string id, std::string n, 
                     double lat, double lon, int slots)
    : Station(id, "AC", n, lat, lon, 7.2, slots), baseRatePerMin(0.5) {
}

ACStation::~ACStation() {
}

double ACStation::calculatePricePerMin(User* user) const {
    if (user == NULL) return baseRatePerMin;
    double discount = user->calculateDiscount(baseRatePerMin);
    return baseRatePerMin - discount;
}

void ACStation::displayInfo() const {
    Station::displayInfo();
    std::cout << "Base Rate: Rs. " << baseRatePerMin << " per minute" << std::endl;
}

std::string ACStation::getDetails() const {
    std::ostringstream oss;
    oss << Station::getDetails() << " | Rate: Rs." << baseRatePerMin << "/min";
    return oss.str();
}
