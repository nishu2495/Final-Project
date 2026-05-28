#ifndef PREMIUMUSER_H
#define PREMIUMUSER_H

#include "User.h"

class PremiumUser : public User {
private:
    int freeMinutes;

public:
    PremiumUser();
    PremiumUser(std::string id, std::string name, 
                std::string contact, double balance);
    
    ~PremiumUser();

    int getFreeMinutes() const;
    void setFreeMinutes(int minutes);
    double calculateDiscount(double basePrice) const;
    void displayInfo() const;
    std::string getUserType() const;
};

#endif
