#ifndef STATION_H
#define STATION_H

#include <string>
#include <fstream>
#include <sstream>

class User;  // Forward declaration

class Station {
protected:
    std::string stationID;
    std::string stationType;
    std::string name;
    double latitude;
    double longitude;
    double powerRating;
    int status;  // 0=Available, 1=Occupied, 2=Faulty, 3=Maintenance
    int maxSlots;
    int occupiedSlots;

public:
    Station();
    Station(std::string id, std::string type, std::string n, 
            double lat, double lon, double power, int slots);
    
    virtual ~Station();

    // Getters
    std::string getStationID() const;
    std::string getStationType() const;
    std::string getName() const;
    double getLatitude() const;
    double getLongitude() const;
    double getPowerRating() const;
    int getStatus() const;
    int getMaxSlots() const;
    int getOccupiedSlots() const;
    int getAvailableSlots() const;

    // Setters
    void setStatus(int s);
    void setOccupiedSlots(int slots);
    void incrementOccupiedSlots();
    void decrementOccupiedSlots();

    // Pure virtual functions
    virtual double calculatePricePerMin(User* user) const = 0;

    // Virtual functions
    virtual void displayInfo() const;
    virtual std::string getDetails() const;

    // File I/O
    virtual void saveToFile(std::ofstream& file) const;
    virtual void loadFromFile(const std::string& line);

    bool isAvailable() const;
    double getDistance(double lat, double lon) const;
};

#endif
