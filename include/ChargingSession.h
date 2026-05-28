#ifndef CHARGINGSESSION_H
#define CHARGINGSESSION_H

#include "Booking.h"
#include <string>

class ChargingSession : public Booking {
private:
    double energyConsumed;  // in kWh
    double finalCost;
    std::string sessionID;

public:
    ChargingSession();
    ChargingSession(std::string sessID, std::string bookID, 
                    Station* st, User* u, int duration);
    
    ~ChargingSession();

    // Getters
    double getEnergyConsumed() const;
    double getFinalCost() const;
    std::string getSessionID() const;

    // Setters
    void setEnergyConsumed(double energy);
    void setFinalCost(double cost);

    // Override virtual function
    double calculateCost() const;
    void displayInfo() const;

    // File I/O
    void saveToFile(std::ofstream& file) const;
    void loadFromFile(const std::string& line);
};

#endif
