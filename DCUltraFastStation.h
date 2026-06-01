#ifndef DCULTRAFASTSTATION_H
#define DCULTRAFASTSTATION_H

#include "Station.h"

class DCUltraFastStation : public Station {
private:
    double ultraPower;
    double baseRatePerMin;

public:
    DCUltraFastStation();
    DCUltraFastStation(std::string id, std::string name, 
                       double lat, double lon, double power, int slots);
    
    ~DCUltraFastStation();

    double getUltraPower() const;
    double calculatePricePerMin(User* user) const;
    void displayInfo() const;
    std::string getDetails() const;
};

#endif
