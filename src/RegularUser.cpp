#include "../include/RegularUser.h"
#include <iostream>

RegularUser::RegularUser() 
    : User("", "", "", "Regular", 0.0) {
}

RegularUser::RegularUser(std::string id, std::string n, 
                         std::string c, double balance)
    : User(id, n, c, "Regular", balance) {
}

RegularUser::~RegularUser() {
}

double RegularUser::calculateDiscount(double basePrice) const {
    return 0.0;  // No discount for regular users
}

void RegularUser::displayInfo() const {
    User::displayInfo();
    std::cout << "User Type: Regular (No special benefits)" << std::endl;
}

std::string RegularUser::getUserType() const {
    return "Regular";
}
