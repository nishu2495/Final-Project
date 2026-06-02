#ifndef EVCHARGINGMANAGER_H
#define EVCHARGINGMANAGER_H

#include <string>
#include <map>
#include <vector>
#include <queue>
#include "Station.h"
#include "User.h"
#include "Booking.h"
#include "ChargingSession.h"

typedef std::map<std::string, Station*> StationMap;
typedef std::map<std::string, User*> UserMap;
typedef std::vector<Booking*> BookingVector;
typedef std::vector<ChargingSession> SessionVector;

class EVChargingManager {
private:
    StationMap stations;
    UserMap users;
    BookingVector activeBookings;
    SessionVector completedSessions;
    std::queue<Booking*> bookingQueue;
    
    std::string dataPath;
    std::string stationsFile;
    std::string usersFile;
    std::string bookingsFile;
    std::string sessionsFile;
    std::string backupFile;

public:
    EVChargingManager();
    ~EVChargingManager();

    // Data management
    void loadAllData();
    void saveAllData();
    void loadStations();
    void loadUsers();
    void loadBookings();
    void loadSessions();

    // Station operations
    bool addStation(Station* station);
    bool removeStation(const std::string& stationID);
    Station* findStationByID(const std::string& stationID) const;
    std::vector<Station*> findAvailableStations(const std::string& type) const;
    std::vector<Station*> findNearestStations(double lat, double lon, int count) const;
    void displayAllStations() const;
    void updateStationStatus();

    // User operations
    bool addUser(User* user);
    bool removeUser(const std::string& userID);
    User* findUserByID(const std::string& userID) const;
    void displayAllUsers() const;
    std::vector<User*> findInactiveUsers(int days) const;

    // Booking operations
    bool bookSlot(const std::string& userID, const std::string& stationID, int minutes);
    Booking* findBookingByID(const std::string& bookingID) const;
    std::vector<Booking*> findUserBookings(const std::string& userID) const;
    bool cancelBooking(const std::string& bookingID);
    void displayActiveBookings() const;
    bool topUpUserWallet(const std::string& userID, double amount);

    // Charging session operations
    bool startChargingSession(const std::string& bookingID);
    bool endChargingSession(const std::string& bookingID);
    void logSession(const ChargingSession& session);

    // Analytics operations
    double calculateTotalRevenue() const;
    double calculateMonthlyRevenue() const;
    double calculateStationRevenue(const std::string& stationID) const;
    double calculateStationUtilization(const std::string& stationID) const;
    std::vector<Station*> findTopRevenueStations(int topN) const;
    void generateRevenueReport() const;
    void generateUtilizationReport() const;
    void generateAnalyticsReport() const;

    // Recommendation engine
    std::vector<Station*> recommendStations(const std::string& userID) const;
    Station* findNearestAvailableStation(double lat, double lon) const;

    // File operations
    void backupSystem(const std::string& backupFileName);
    void restoreSystem(const std::string& backupFileName);
    void exportToCSV(const std::string& type) const;

    // Utility
    int getTotalStations() const;
    int getTotalUsers() const;
    int getTotalBookings() const;
    int getTotalSessions() const;
    void displaySystemStatus() const;
    void clearAllData();

private:
    std::string generateID(const std::string& prefix);
    int nextBookingNumber;
    int nextSessionNumber;
    User* createUserFromTier(const std::string& tier, std::string id, 
                            std::string name, std::string contact, double balance);
    Station* createStationFromType(const std::string& type);
};

#endif
