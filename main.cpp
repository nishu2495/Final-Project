// Copy of src/main.cpp: full implementation
#include "EVChargingManager.h"
#include "InputValidator.h"
#include "Station.h"
#include "User.h"
#include "ACStation.h"
#include "DCFastStation.h"
#include "DCUltraFastStation.h"
#include "RegularUser.h"
#include "PremiumUser.h"
#include "FleetUser.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>

EVChargingManager manager;
User* loggedInUser = NULL;

void displayMainMenu();
void displayAdminPortal();
void displayStationManagement();
void displayUserManagement();
void displayActiveBookingsMenu();
void displayUserPortal();
void displayUserDashboard();
void displayAnalyticsMenu();
void displayMaintenanceMenu();
void initializeSampleData();

void displayMainMenu() {
    int choice;
    while (true) {
        InputValidator::clearScreen();
        InputValidator::displaySeparator("SMART EV CHARGING NETWORK OPERATIONS PLATFORM");
        std::cout << "\n       ChargeGrid Mobility Pvt. Ltd.\n\n";
        std::cout << "1. Admin / Operator Portal\n";
        std::cout << "2. User Simulation Portal\n";
        std::cout << "3. Analytics & Reports\n";
        std::cout << "4. System Backup & Restore\n";
        std::cout << "5. Exit\n";
        
        choice = InputValidator::getValidatedIntInput(1, 5);
        
        switch (choice) {
            case 1:
                displayAdminPortal();
                break;
            case 2:
                displayUserPortal();
                break;
            case 3:
                displayAnalyticsMenu();
                break;
            case 4:
                displayMaintenanceMenu();
                break;
            case 5:
                manager.saveAllData();
                std::cout << "\nThank you for using ChargeGrid Platform!\n";
                exit(0);
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayAdminPortal() {
    int choice;
    while (true) {
        InputValidator::clearScreen();
        InputValidator::displaySeparator("ADMIN / OPERATOR PORTAL");
        std::cout << "\n1. Station Management\n";
        std::cout << "2. User Management\n";
        std::cout << "3. View Real-time Station Status\n";
        std::cout << "4. Manage Active Bookings\n";
        std::cout << "5. Back to Main Menu\n";
        
        choice = InputValidator::getValidatedIntInput(1, 5);
        
        switch (choice) {
            case 1:
                displayStationManagement();
                break;
            case 2:
                displayUserManagement();
                break;
            case 3:
                manager.displayAllStations();
                InputValidator::pauseForUser();
                break;
            case 4:
                displayActiveBookingsMenu();
                break;
            case 5:
                return;
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayStationManagement() {
    int choice;
    while (true) {
        InputValidator::clearScreen();
        InputValidator::displaySeparator("STATION MANAGEMENT");
        std::cout << "\n1. Add New Charging Station\n";
        std::cout << "2. Remove Station\n";
        std::cout << "3. Update Station Status\n";
        std::cout << "4. List All Stations\n";
        std::cout << "5. Search Station by ID\n";
        std::cout << "6. Back to Admin Menu\n";
        
        choice = InputValidator::getValidatedIntInput(1, 6);
        
        switch (choice) {
            case 1: {
                std::cout << "\nAdd New Station\n";
                std::string id = InputValidator::getValidatedStringInput("Station ID (e.g., ST001): ");
                std::string name = InputValidator::getValidatedStringInput("Station Name: ");
                double lat = InputValidator::getValidatedLatitude();
                double lon = InputValidator::getValidatedLongitude();
                
                std::cout << "\nStation Type:\n1. AC\n2. DC Fast\n3. DC Ultra-Fast\n";
                int type = InputValidator::getValidatedIntInput(1, 3);
                
                Station* station = NULL;
                if (type == 1) {
                    station = new ACStation(id, name, lat, lon, 4);
                } else if (type == 2) {
                    station = new DCFastStation(id, name, lat, lon, 25.0, 2);
                } else {
                    station = new DCUltraFastStation(id, name, lat, lon, 60.0, 3);
                }
                
                if (manager.addStation(station)) {
                    std::cout << "Station added successfully!\n";
                } else {
                    std::cout << "Failed to add station!\n";
                    delete station;
                }
                InputValidator::pauseForUser();
                break;
            }
            case 2: {
                std::cout << "\nRemove Station\n";
                manager.displayAllStations();
                std::string id = InputValidator::getValidatedStringInput("\nEnter Station ID to remove (e.g., ST001): ");
                if (manager.removeStation(id)) {
                    std::cout << "Station removed!\n";
                } else {
                    std::cout << "Station not found!\n";
                }
                InputValidator::pauseForUser();
                break;
            }
            case 3:
                manager.updateStationStatus();
                InputValidator::pauseForUser();
                break;
            case 4:
                manager.displayAllStations();
                InputValidator::pauseForUser();
                break;
            case 5: {
                std::cout << "\nSearch Station by ID\n";
                std::string id = InputValidator::getValidatedStringInput("Enter Station ID (e.g., ST001): ");
                Station* st = manager.findStationByID(id);
                if (st != NULL) {
                    st->displayInfo();
                } else {
                    std::cout << "Station not found!\n";
                }
                InputValidator::pauseForUser();
                break;
            }
            case 6:
                return;
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayUserManagement() {
    int choice;
    while (true) {
        InputValidator::clearScreen();
        InputValidator::displaySeparator("USER MANAGEMENT");
        std::cout << "\n1. Add New User (Register)\n";
        std::cout << "2. Remove User\n";
        std::cout << "3. List All Users\n";
        std::cout << "4. Search User by ID\n";
        std::cout << "5. View User Details\n";
        std::cout << "6. Back to Admin Menu\n";
        
        choice = InputValidator::getValidatedIntInput(1, 6);
        
        switch (choice) {
            case 1: {
                std::cout << "\nRegister New User\n";
                std::string id = InputValidator::getValidatedStringInput("User ID (e.g., U001): ");
                std::string name = InputValidator::getValidatedStringInput("Full Name: ");
                std::string contact = InputValidator::getValidatedPhoneInput();
                
                std::cout << "\nUser Tier:\n1. Regular\n2. Premium\n3. Fleet\n";
                int tier = InputValidator::getValidatedIntInput(1, 3);
                
                User* user = NULL;
                if (tier == 1) {
                    user = new RegularUser(id, name, contact, 1000.0);
                } else if (tier == 2) {
                    user = new PremiumUser(id, name, contact, 2000.0);
                } else {
                    user = new FleetUser(id, name, contact, id, "Company Ltd.", 5000.0);
                }
                
                if (manager.addUser(user)) {
                    std::cout << "User registered successfully!\n";
                } else {
                    std::cout << "Failed to register user!\n";
                    delete user;
                }
                InputValidator::pauseForUser();
                break;
            }
            case 2: {
                std::cout << "\nRemove User\n";
                manager.displayAllUsers();
                std::string id = InputValidator::getValidatedStringInput("\nEnter User ID to remove (e.g., U001): ");
                if (manager.removeUser(id)) {
                    std::cout << "User removed!\n";
                } else {
                    std::cout << "User not found!\n";
                }
                InputValidator::pauseForUser();
                break;
            }
            case 3:
                manager.displayAllUsers();
                InputValidator::pauseForUser();
                break;
            case 4: {
                std::cout << "\nSearch User by ID\n";
                std::string id = InputValidator::getValidatedStringInput("Enter User ID (e.g., U001): ");
                User* user = manager.findUserByID(id);
                if (user != NULL) {
                    user->displayInfo();
                } else {
                    std::cout << "User not found!\n";
                }
                InputValidator::pauseForUser();
                break;
            }
            case 5: {
                std::cout << "\nView User Details\n";
                manager.displayAllUsers();
                std::string id = InputValidator::getValidatedStringInput("\nEnter User ID (e.g., U001): ");
                User* user = manager.findUserByID(id);
                if (user != NULL) {
                    user->displayInfo();
                } else {
                    std::cout << "User not found!\n";
                }
                InputValidator::pauseForUser();
                break;
            }
            case 6:
                return;
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayActiveBookingsMenu() {
    int choice;
    while (true) {
        InputValidator::clearScreen();
        InputValidator::displaySeparator("ACTIVE BOOKINGS");
        std::cout << "\n1. View All Active Bookings\n";
        std::cout << "2. Cancel Booking (Admin)\n";
        std::cout << "3. Force End Session\n";
        std::cout << "4. Back to Admin Menu\n";
        
        choice = InputValidator::getValidatedIntInput(1, 4);
        
        switch (choice) {
            case 1:
                manager.displayActiveBookings();
                InputValidator::pauseForUser();
                break;
            case 2: {
                std::cout << "\nCancel Booking\n";
                manager.displayActiveBookings();
                std::string id = InputValidator::getValidatedStringInput("\nEnter Booking ID to cancel (e.g., B001): ");
                if (manager.cancelBooking(id)) {
                    std::cout << "Booking cancelled!\n";
                } else {
                    std::cout << "Booking not found!\n";
                }
                InputValidator::pauseForUser();
                break;
            }
            case 3: {
                std::cout << "\nEnd Session\n";
                manager.displayActiveBookings();
                std::string id = InputValidator::getValidatedStringInput("\nEnter Booking ID to end (e.g., B001): ");
                if (manager.endChargingSession(id)) {
                    std::cout << "Session ended!\n";
                } else {
                    std::cout << "Session not found!\n";
                }
                InputValidator::pauseForUser();
                break;
            }
            case 4:
                return;
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayUserPortal() {
    int choice;
    while (true) {
        InputValidator::clearScreen();
        InputValidator::displaySeparator("USER SIMULATION PORTAL");
        std::cout << "\n1. Register New User\n";
        std::cout << "2. Login\n";
        std::cout << "3. Back to Main Menu\n";
        
        choice = InputValidator::getValidatedIntInput(1, 3);
        
        switch (choice) {
            case 1: {
                std::cout << "\nRegister New User\n";
                std::string id = InputValidator::getValidatedStringInput("User ID (e.g., U001): ");
                std::string name = InputValidator::getValidatedStringInput("Full Name: ");
                std::string contact = InputValidator::getValidatedPhoneInput();
                
                std::cout << "\nUser Tier:\n1. Regular\n2. Premium\n3. Fleet\n";
                int tier = InputValidator::getValidatedIntInput(1, 3);
                
                User* user = NULL;
                if (tier == 1) {
                    user = new RegularUser(id, name, contact, 1000.0);
                } else if (tier == 2) {
                    user = new PremiumUser(id, name, contact, 2000.0);
                } else {
                    user = new FleetUser(id, name, contact, id, "Company Ltd.", 5000.0);
                }
                
                if (manager.addUser(user)) {
                    std::cout << "User registered successfully!\n";
                    loggedInUser = user;
                    displayUserDashboard();
                } else {
                    std::cout << "Failed to register user!\n";
                    delete user;
                }
                InputValidator::pauseForUser();
                break;
            }
            case 2: {
                std::cout << "\nLogin\n";
                std::string id = InputValidator::getValidatedStringInput("User ID: ");
                User* user = manager.findUserByID(id);
                if (user != NULL) {
                    std::string pwd = InputValidator::getValidatedPasswordInput("Password: ");
                    if (user->validatePassword(pwd)) {
                        loggedInUser = user;
                        std::cout << "Login successful!\n";
                        InputValidator::pauseForUser();
                        displayUserDashboard();
                    } else {
                        std::cout << "Invalid password!\n";
                        InputValidator::pauseForUser();
                    }
                } else {
                    std::cout << "User not found!\n";
                    InputValidator::pauseForUser();
                }
                break;
            }
            case 3:
                return;
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayUserDashboard() {
    if (loggedInUser == NULL) return;
    
    int choice;
    while (true) {
        InputValidator::clearScreen();
        std::cout << "\n========== WELCOME, " << loggedInUser->getName() << " ==========" << std::endl;
        std::cout << "Current Tier: " << loggedInUser->getTier() << std::endl;
        std::cout << "Wallet Balance: Rs. " << std::fixed << std::setprecision(2) 
                 << loggedInUser->getWalletBalance() << std::endl << std::endl;
        
        std::cout << "1. Search Available Stations\n";
        std::cout << "2. Book a Charging Slot\n";
        std::cout << "3. View My Bookings\n";
        std::cout << "4. Start Charging Session\n";
        std::cout << "5. End Charging Session\n";
        std::cout << "6. View Charging History\n";
        std::cout << "7. Logout\n";
        
        choice = InputValidator::getValidatedIntInput(1, 7);
        
        switch (choice) {
            case 1: {
                std::cout << "\nAvailable Charging Stations\n";
                manager.displayAllStations();
                InputValidator::pauseForUser();
                break;
            }
            case 2: {
                std::cout << "\nBook a Charging Slot\n";
                manager.displayAllStations();
                    std::string stationID = InputValidator::getValidatedStringInput("\nEnter Station ID (e.g., ST001): ");
                int minutes = InputValidator::getValidatedIntInput(15, 480);
                
                if (manager.bookSlot(loggedInUser->getUserID(), stationID, minutes)) {
                    std::cout << "Booking confirmed!\n";
                } else {
                    std::cout << "Booking failed!\n";
                }
                InputValidator::pauseForUser();
                break;
            }
            case 3: {
                std::cout << "\nMy Bookings\n";
                std::vector<Booking*> bookings = manager.findUserBookings(loggedInUser->getUserID());
                if (bookings.empty()) {
                    std::cout << "No bookings found.\n";
                } else {
                    for (size_t i = 0; i < bookings.size(); ++i) {
                        bookings[i]->displayInfo();
                    }
                }
                InputValidator::pauseForUser();
                break;
            }
            case 4: {
                std::cout << "\nStart Charging Session\n";
                std::vector<Booking*> bookings = manager.findUserBookings(loggedInUser->getUserID());
                if (bookings.empty()) {
                    std::cout << "No bookings available.\n";
                } else {
                    for (size_t i = 0; i < bookings.size(); ++i) {
                        if (bookings[i]->isBooked()) {
                            std::cout << (i+1) << ". " << bookings[i]->getBookingID() << std::endl;
                        }
                    }
                    std::string id = InputValidator::getValidatedStringInput("Enter Booking ID (e.g., B001): ");
                    if (manager.startChargingSession(id)) {
                        std::cout << "Charging session started!\n";
                    } else {
                        std::cout << "Failed to start session!\n";
                    }
                }
                InputValidator::pauseForUser();
                break;
            }
            case 5: {
                std::cout << "\nEnd Charging Session\n";
                std::vector<Booking*> bookings = manager.findUserBookings(loggedInUser->getUserID());
                if (bookings.empty()) {
                    std::cout << "No active sessions.\n";
                } else {
                    for (size_t i = 0; i < bookings.size(); ++i) {
                        if (bookings[i]->isActive()) {
                            std::cout << (i+1) << ". " << bookings[i]->getBookingID() << std::endl;
                        }
                    }
                    std::string id = InputValidator::getValidatedStringInput("Enter Booking ID (e.g., B001): ");
                    if (manager.endChargingSession(id)) {
                        std::cout << "Charging session ended!\n";
                    } else {
                        std::cout << "Failed to end session!\n";
                    }
                }
                InputValidator::pauseForUser();
                break;
            }
            case 6: {
                std::cout << "\nCharging History\n";
                std::cout << "Total Sessions: " << loggedInUser->getTotalSessions() << std::endl;
                InputValidator::pauseForUser();
                break;
            }
            case 7:
                loggedInUser = NULL;
                std::cout << "Logged out successfully!\n";
                InputValidator::pauseForUser();
                return;
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayAnalyticsMenu() {
    int choice;
    while (true) {
        InputValidator::clearScreen();
        InputValidator::displaySeparator("ANALYTICS & REPORTS");
        std::cout << "\n1. Revenue Summary Report\n";
        std::cout << "2. Station Utilization Report\n";
        std::cout << "3. Top 10 Highest Revenue Stations\n";
        std::cout << "4. List Inactive Users\n";
        std::cout << "5. System Analytics Report\n";
        std::cout << "6. Export Report to File\n";
        std::cout << "7. Back to Main Menu\n";
        
        choice = InputValidator::getValidatedIntInput(1, 7);
        
        switch (choice) {
            case 1:
                manager.generateRevenueReport();
                InputValidator::pauseForUser();
                break;
            case 2:
                manager.generateUtilizationReport();
                InputValidator::pauseForUser();
                break;
            case 3: {
                std::cout << "\nTop 10 Revenue Stations\n";
                std::vector<Station*> topStations = manager.findTopRevenueStations(10);
                int rank = 1;
                for (size_t i = 0; i < topStations.size(); ++i) {
                    std::cout << rank << ". " << topStations[i]->getName() << std::endl;
                    rank++;
                }
                InputValidator::pauseForUser();
                break;
            }
            case 4: {
                std::cout << "\nInactive Users (>30 days)\n";
                std::vector<User*> inactiveUsers = manager.findInactiveUsers(30);
                for (size_t i = 0; i < inactiveUsers.size(); ++i) {
                    std::cout << inactiveUsers[i]->getName() << std::endl;
                }
                InputValidator::pauseForUser();
                break;
            }
            case 5:
                manager.generateAnalyticsReport();
                InputValidator::pauseForUser();
                break;
            case 6: {
                std::cout << "\nExport Report\n";
                std::cout << "1. Export Stations\n2. Export Users\n";
                int exp = InputValidator::getValidatedIntInput(1, 2);
                if (exp == 1) {
                    manager.exportToCSV("stations");
                } else {
                    manager.exportToCSV("users");
                }
                InputValidator::pauseForUser();
                break;
            }
            case 7:
                return;
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayMaintenanceMenu() {
    int choice;
    while (true) {
        InputValidator::clearScreen();
        InputValidator::displaySeparator("SYSTEM MAINTENANCE");
        std::cout << "\n1. Create Full System Backup\n";
        std::cout << "2. Restore System from Backup\n";
        std::cout << "3. Export All Stations to CSV\n";
        std::cout << "4. Export All Users to CSV\n";
        std::cout << "5. System Status\n";
        std::cout << "6. Back to Main Menu\n";
        
        choice = InputValidator::getValidatedIntInput(1, 6);
        
        switch (choice) {
            case 1:
                manager.backupSystem("data/backup.dat");
                InputValidator::pauseForUser();
                break;
            case 2:
                manager.restoreSystem("data/backup.dat");
                InputValidator::pauseForUser();
                break;
            case 3:
                manager.exportToCSV("stations");
                InputValidator::pauseForUser();
                break;
            case 4:
                manager.exportToCSV("users");
                InputValidator::pauseForUser();
                break;
            case 5:
                manager.displaySystemStatus();
                InputValidator::pauseForUser();
                break;
            case 6:
                return;
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void initializeSampleData() {
    // Create sample stations
    manager.addStation(new ACStation("ST001", "Anna Nagar AC Hub", 13.0843, 80.2102, 4));
    manager.addStation(new DCFastStation("ST002", "Guindy DC Fast Charger", 13.0067, 80.2206, 25.0, 2));
    manager.addStation(new DCUltraFastStation("ST003", "OMR Ultra Fast Station", 12.9220, 80.1500, 60.0, 3));
    manager.addStation(new ACStation("ST004", "Koyambedu AC Station", 13.0690, 80.1947, 6));
    manager.addStation(new DCFastStation("ST005", "Velachery DC Fast", 12.9750, 80.2210, 25.0, 2));
    
    // Create sample users
    manager.addUser(new RegularUser("U001", "Ramesh Kumar", "9876543210", 450.75));
    manager.addUser(new PremiumUser("U002", "Priya Sharma", "9123456789", 1240.50));
    manager.addUser(new FleetUser("U003", "Suresh Reddy", "9988776655", "FLEET001", "ChargeGrid Fleet", 8750.00));
    manager.addUser(new RegularUser("U004", "Anjali Menon", "8765432109", 120.00));
    manager.addUser(new PremiumUser("U005", "Karthik Rajan", "9345678901", 2150.25));
    
    std::cout << "Sample data initialized!\n";
}

int main() {
    std::cout << "Loading Smart EV Charging Network Platform...\n";
    manager.loadAllData();
    
    // Initialize sample data if no data exists
    if (manager.getTotalStations() == 0) {
        initializeSampleData();
        manager.saveAllData();
    }
    
    displayMainMenu();
    return 0;
}
