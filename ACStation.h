#ifndef ACSTATION_H
#define ACSTATION_H

#include "Station.h"

class ACStation : public Station {
private:
    double baseRatePerMin;

public:
    ACStation();
    ACStation(std::string id, std::string name, 
              double lat, double lon, int slots);
    
    ~ACStation();

    double calculatePricePerMin(User* user) const;
    void displayInfo() const;
    std::string getDetails() const;
};

#endif
