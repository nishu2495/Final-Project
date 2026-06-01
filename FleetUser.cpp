#include "FleetUser.h"
#include <iostream>
#include <iomanip>

FleetUser::FleetUser() 
    : User("", "", "", "Fleet", 0.0), fleetID(""), companyName("") {
}

FleetUser::FleetUser(std::string id, std::string n, 
                     std::string c, std::string fleet, 
                     std::string company, double balance)
    : User(id, n, c, "Fleet", balance), fleetID(fleet), companyName(company) {
}

FleetUser::~FleetUser() {
}

std::string FleetUser::getFleetID() const {
    return fleetID;
}

std::string FleetUser::getCompanyName() const {
    return companyName;
}

double FleetUser::calculateDiscount(double basePrice) const {
    return basePrice * 0.25;  // 25% discount for fleet users
}

void FleetUser::displayInfo() const {
    User::displayInfo();
    std::cout << "User Type: Fleet" << std::endl;
    std::cout << "Fleet ID: " << fleetID << std::endl;
    std::cout << "Company: " << companyName << std::endl;
    std::cout << "Discount: 25%" << std::endl;
}

std::string FleetUser::getUserType() const {
    return "Fleet";
}
