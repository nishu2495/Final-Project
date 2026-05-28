#ifndef DCFASTSTATION_H
#define DCFASTSTATION_H

#include "Station.h"

class DCFastStation : public Station {
private:
    double maxPower;
    double baseRatePerMin;

public:
    DCFastStation();
    DCFastStation(std::string id, std::string name, 
                  double lat, double lon, double power, int slots);
    
    ~DCFastStation();

    double getMaxPower() const;
    double calculatePricePerMin(User* user) const;
    void displayInfo() const;
    std::string getDetails() const;
};

#endif
