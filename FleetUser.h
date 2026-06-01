#ifndef FLEETUSER_H
#define FLEETUSER_H

#include "User.h"

class FleetUser : public User {
private:
    std::string fleetID;
    std::string companyName;

public:
    FleetUser();
    FleetUser(std::string id, std::string name, 
              std::string contact, std::string fleet, 
              std::string company, double balance);
    
    ~FleetUser();

    std::string getFleetID() const;
    std::string getCompanyName() const;
    double calculateDiscount(double basePrice) const;
    void displayInfo() const;
    std::string getUserType() const;
};

#endif
