#include "DCFastStation.h"
#include "User.h"
#include <iostream>
#include <sstream>

DCFastStation::DCFastStation() 
    : Station("", "DCFast", "", 0.0, 0.0, 25.0, 0), 
      maxPower(25.0), baseRatePerMin(1.2) {
}

DCFastStation::DCFastStation(std::string id, std::string n, 
                             double lat, double lon, double power, int slots)
    : Station(id, "DCFast", n, lat, lon, power, slots), 
      maxPower(power), baseRatePerMin(1.2) {
}

DCFastStation::~DCFastStation() {
}

double DCFastStation::getMaxPower() const {
    return maxPower;
}

double DCFastStation::calculatePricePerMin(User* user) const {
    if (user == NULL) return baseRatePerMin;
    double discount = user->calculateDiscount(baseRatePerMin);
    return baseRatePerMin - discount;
}

void DCFastStation::displayInfo() const {
    Station::displayInfo();
    std::cout << "Max Power: " << maxPower << " kW" << std::endl;
    std::cout << "Base Rate: Rs. " << baseRatePerMin << " per minute" << std::endl;
}

std::string DCFastStation::getDetails() const {
    std::ostringstream oss;
    oss << Station::getDetails() << " | Rate: Rs." << baseRatePerMin << "/min";
    return oss.str();
}
