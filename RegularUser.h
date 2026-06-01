#ifndef REGULARUSER_H
#define REGULARUSER_H

#include "User.h"

class RegularUser : public User {
public:
    RegularUser();
    RegularUser(std::string id, std::string name, 
                std::string contact, double balance);
    
    ~RegularUser();

    double calculateDiscount(double basePrice) const;
    void displayInfo() const;
    std::string getUserType() const;
};

#endif
