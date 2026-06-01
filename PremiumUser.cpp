#include "PremiumUser.h"
#include <iostream>
#include <iomanip>

PremiumUser::PremiumUser() 
    : User("", "", "", "Premium", 0.0), freeMinutes(50) {
}

PremiumUser::PremiumUser(std::string id, std::string n, 
                         std::string c, double balance)
    : User(id, n, c, "Premium", balance), freeMinutes(50) {
}

PremiumUser::~PremiumUser() {
}

int PremiumUser::getFreeMinutes() const {
    return freeMinutes;
}

void PremiumUser::setFreeMinutes(int minutes) {
    if (minutes >= 0) {
        freeMinutes = minutes;
    }
}

double PremiumUser::calculateDiscount(double basePrice) const {
    return basePrice * 0.15;  // 15% discount for premium users
}

void PremiumUser::displayInfo() const {
    User::displayInfo();
    std::cout << "User Type: Premium" << std::endl;
    std::cout << "Free Charging Minutes: " << freeMinutes << " minutes" << std::endl;
    std::cout << "Discount: 15%" << std::endl;
}

std::string PremiumUser::getUserType() const {
    return "Premium";
}
