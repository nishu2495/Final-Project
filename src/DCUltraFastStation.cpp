#include "../include/DCUltraFastStation.h"
#include "../include/User.h"
#include <iostream>
#include <sstream>

DCUltraFastStation::DCUltraFastStation() 
    : Station("", "DCUltra", "", 0.0, 0.0, 60.0, 0), 
      ultraPower(60.0), baseRatePerMin(2.0) {
}

DCUltraFastStation::DCUltraFastStation(std::string id, std::string n, 
                                       double lat, double lon, double power, int slots)
    : Station(id, "DCUltra", n, lat, lon, power, slots), 
      ultraPower(power), baseRatePerMin(2.0) {
}

DCUltraFastStation::~DCUltraFastStation() {
}

double DCUltraFastStation::getUltraPower() const {
    return ultraPower;
}

double DCUltraFastStation::calculatePricePerMin(User* user) const {
    if (user == NULL) return baseRatePerMin;
    double discount = user->calculateDiscount(baseRatePerMin);
    return baseRatePerMin - discount;
}

void DCUltraFastStation::displayInfo() const {
    Station::displayInfo();
    std::cout << "Ultra Power: " << ultraPower << " kW" << std::endl;
    std::cout << "Base Rate: Rs. " << baseRatePerMin << " per minute" << std::endl;
}

std::string DCUltraFastStation::getDetails() const {
    std::ostringstream oss;
    oss << Station::getDetails() << " | Rate: Rs." << baseRatePerMin << "/min";
    return oss.str();
}
