#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include <ctime>
#include <fstream>
#include <sstream>

class Station;
class User;

class Booking {
protected:
    std::string bookingID;
    Station* station;
    User* user;
    time_t startTime;
    time_t endTime;
    int slotDuration;  // in minutes
    int status;  // 0=Booked, 1=Active, 2=Completed, 3=Cancelled

public:
    Booking();
    Booking(std::string id, Station* st, User* u, int duration);
    
    virtual ~Booking();

    // Getters
    std::string getBookingID() const;
    Station* getStation() const;
    User* getUser() const;
    time_t getStartTime() const;
    time_t getEndTime() const;
    int getSlotDuration() const;
    int getStatus() const;
    std::string getStatusString() const;

    // Setters
    void setStatus(int s);
    void setStartTime(time_t t);
    void setEndTime(time_t t);

    // Virtual functions
    virtual double calculateCost() const;
    virtual void displayInfo() const;

    // File I/O
    virtual void saveToFile(std::ofstream& file) const;
    virtual void loadFromFile(const std::string& line);

    bool isActive() const;
    bool isBooked() const;
    long getDurationInSeconds() const;
};

#endif
