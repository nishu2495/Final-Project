#ifndef USER_H
#define USER_H

#include <string>
#include <fstream>
#include <sstream>

class User {
protected:
    std::string userID;
    std::string name;
    std::string contact;
    std::string tier;
    double walletBalance;
    int totalSessions;
    std::string password;

public:
    User();
    User(std::string id, std::string n, std::string c, 
         std::string t, double balance);
    
    virtual ~User();

    // Getters
    std::string getUserID() const;
    std::string getName() const;
    std::string getContact() const;
    std::string getTier() const;
    double getWalletBalance() const;
    int getTotalSessions() const;
    std::string getPassword() const;

    // Setters
    void setWalletBalance(double balance);
    void setPassword(const std::string& pwd);
    void addSession();
    void addBalance(double amount);
    void deductBalance(double amount);
    // Force deduction even if balance insufficient (allows negative balance)
    void forceDeductBalance(double amount);

    // Virtual functions
    virtual double calculateDiscount(double basePrice) const;
    virtual void displayInfo() const;
    virtual std::string getUserType() const = 0;

    // File I/O
    virtual void saveToFile(std::ofstream& file) const;
    virtual void loadFromFile(const std::string& line);

    bool validatePassword(const std::string& pwd) const;
};

#endif
