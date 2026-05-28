#include "../include/User.h"
#include <iostream>
#include <iomanip>

User::User() 
    : userID(""), name(""), contact(""), tier("Regular"), 
      walletBalance(0.0), totalSessions(0), password("") {
}

User::User(std::string id, std::string n, std::string c, 
           std::string t, double balance)
    : userID(id), name(n), contact(c), tier(t), 
      walletBalance(balance), totalSessions(0), password("1234") {
}

User::~User() {
}

std::string User::getUserID() const {
    return userID;
}

std::string User::getName() const {
    return name;
}

std::string User::getContact() const {
    return contact;
}

std::string User::getTier() const {
    return tier;
}

double User::getWalletBalance() const {
    return walletBalance;
}

int User::getTotalSessions() const {
    return totalSessions;
}

std::string User::getPassword() const {
    return password;
}

void User::setWalletBalance(double balance) {
    if (balance >= 0) {
        walletBalance = balance;
    }
}

void User::setPassword(const std::string& pwd) {
    password = pwd;
}

void User::addSession() {
    totalSessions++;
}

void User::addBalance(double amount) {
    if (amount > 0) {
        walletBalance += amount;
    }
}

void User::deductBalance(double amount) {
    if (amount > 0 && walletBalance >= amount) {
        walletBalance -= amount;
    }
}

double User::calculateDiscount(double basePrice) const {
    return 0.0;  // No discount for base User
}

void User::displayInfo() const {
    std::cout << "\nUser ID: " << userID << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Contact: " << contact << std::endl;
    std::cout << "Tier: " << tier << std::endl;
    std::cout << "Wallet Balance: Rs. " << std::fixed << std::setprecision(2) 
              << walletBalance << std::endl;
    std::cout << "Total Sessions: " << totalSessions << std::endl;
}

void User::saveToFile(std::ofstream& file) const {
    file << userID << "," << name << "," << contact << "," 
         << tier << "," << std::fixed << std::setprecision(2)
         << walletBalance << "," << totalSessions << std::endl;
}

void User::loadFromFile(const std::string& line) {
    std::istringstream iss(line);
    char comma;
    iss >> userID >> comma >> name >> comma >> contact >> comma
        >> tier >> comma >> walletBalance >> comma >> totalSessions;
}

bool User::validatePassword(const std::string& pwd) const {
    return password == pwd;
}
