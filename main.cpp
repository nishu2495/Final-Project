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
void displayWalletTopUpForUser();
void displayWalletTopUpForAdmin();
void displayWalletManagementMenu();
void displayAnalyticsMenu();
void displayMaintenanceMenu();
void initializeSampleData();

void displayMainMenu() {
    int choice;
    while (true) {
        InputValidator::displayCenteredTitle("SMART EV CHARGING NETWORK OPERATIONS PLATFORM");
    InputValidator::displayCenteredBlock("ChargeGrid Mobility Pvt. Ltd.\n\n1. Admin / Operator Portal\n2. User Simulation Portal\n3. Analytics & Reports\n4. System Backup & Restore\n0. Exit\n");
    choice = InputValidator::getValidatedIntInput(0, 4, "Enter choice (0-4): ");
        switch (choice) {
            case 0:
                manager.saveAllData();
                InputValidator::clearScreen();
                InputValidator::displayCenteredTitle("GOODBYE");
                InputValidator::displayCenteredBlock("Thank you for using ChargeGrid Platform!\n");
                exit(0);
                break;
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
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayAdminPortal() {
    int choice;
    while (true) {
        InputValidator::displayCenteredTitle("ADMIN / OPERATOR PORTAL");
        InputValidator::displayCenteredBlock("1. Station Management\n2. User Management\n3. Wallet Management\n4. View Real-time Station Status\n5. Manage Active Bookings\n0. Back to Main Menu\n");
        choice = InputValidator::getValidatedIntInput(0, 5, "Enter choice (0-5): ");
        
        switch (choice) {
            case 0:
                return;
            case 1:
                displayStationManagement();
                break;
            case 2:
                displayUserManagement();
                break;
            case 3:
                displayWalletManagementMenu();
                break;
            case 4:
                manager.displayAllStations();
                InputValidator::pauseForUser();
                break;
            case 5:
                displayActiveBookingsMenu();
                break;
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayStationManagement() {
    int choice;
    while (true) {
        InputValidator::displayCenteredTitle("STATION MANAGEMENT");
        InputValidator::displayCenteredBlock("1. Add New Charging Station\n2. Remove Station\n3. Update Station Status\n4. List All Stations\n5. Search Station by ID\n0. Back to Admin Menu\n");
        choice = InputValidator::getValidatedIntInput(0, 5, "Enter choice (0-5): ");
        
        switch (choice) {
            case 0:
                return;
            case 1: {
                InputValidator::displayCenteredTitle("Add New Station");
                std::string id = InputValidator::boxedInputString("Station ID (e.g., ST001): ");
                std::string name = InputValidator::boxedInputString("Station Name: ");
                double lat = InputValidator::boxedInputDouble("Latitude (deg) (-90 to 90): ", -90.0, 90.0);
                double lon = InputValidator::boxedInputDouble("Longitude (deg) (-180 to 180): ", -180.0, 180.0);

                int type = InputValidator::boxedInputInt("Station Type:\n1. AC\n2. DC Fast\n3. DC Ultra-Fast\n", 1, 3);
                
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
                InputValidator::displayCenteredTitle("Remove Station");
                manager.displayAllStations();
                std::string id = InputValidator::boxedInputString("Enter Station ID to remove (e.g., ST001): ");
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
                InputValidator::displayCenteredTitle("Search Station");
                std::string id = InputValidator::boxedInputString("Enter Station ID (e.g., ST001): ");
                Station* st = manager.findStationByID(id);
                if (st != NULL) {
                    st->displayInfo();
                } else {
                    std::cout << "Station not found!\n";
                }
                InputValidator::pauseForUser();
                break;
            }
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayUserManagement() {
    int choice;
    while (true) {
        InputValidator::displayCenteredTitle("USER MANAGEMENT");
        InputValidator::displayCenteredBlock("1. Add New User (Register)\n2. Remove User\n3. List All Users\n4. Search User by ID\n5. View User Details\n6. Top-up User Wallet\n0. Back to Admin Menu\n");
        choice = InputValidator::getValidatedIntInput(0, 6, "Enter choice (0-6): ");
        
        switch (choice) {
            case 0:
                return;
            case 1: {
                InputValidator::displayCenteredTitle("Register New User");
                std::string id = InputValidator::boxedInputString("User ID (e.g., U001) (0 to cancel): ", true);
                if (id.empty()) {
                    std::cout << "User registration cancelled.\n";
                    InputValidator::pauseForUser();
                    break;
                }
                std::string name = InputValidator::boxedInputString("Full Name (0 to cancel): ", true);
                if (name.empty()) {
                    std::cout << "User registration cancelled.\n";
                    InputValidator::pauseForUser();
                    break;
                }
                std::string contact = InputValidator::getValidatedPhoneInput(true);
                if (contact.empty()) {
                    std::cout << "User registration cancelled.\n";
                    InputValidator::pauseForUser();
                    break;
                }
                
                int tier = InputValidator::boxedInputInt("User Tier:\n1. Regular\n2. Premium\n3. Fleet\n", 1, 3);
                
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
                InputValidator::displayCenteredTitle("Remove User");
                manager.displayAllUsers();
                std::string id = InputValidator::boxedInputString("Enter User ID to remove (e.g., U001): ");
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
                InputValidator::displayCenteredTitle("Search User by ID");
                std::string id = InputValidator::boxedInputString("Enter User ID (e.g., U001): ");
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
                InputValidator::displayCenteredTitle("View User Details");
                manager.displayAllUsers();
                std::string id = InputValidator::boxedInputString("Enter User ID (e.g., U001): ");
                User* user = manager.findUserByID(id);
                if (user != NULL) {
                    user->displayInfo();
                } else {
                    std::cout << "User not found!\n";
                }
                InputValidator::pauseForUser();
                break;
            }
            case 6: {
                displayWalletTopUpForAdmin();
                break;
            }
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayActiveBookingsMenu() {
    int choice;
    while (true) {
        InputValidator::displayCenteredTitle("ACTIVE BOOKINGS");
        InputValidator::displayCenteredBlock("1. View All Active Bookings\n2. Cancel Booking (Admin)\n3. Force End Session\n0. Back to Admin Menu\n");
        choice = InputValidator::getValidatedIntInput(0, 3, "Enter choice (0-3): ");
        
        switch (choice) {
            case 0:
                return;
            case 1:
                manager.displayActiveBookings();
                InputValidator::pauseForUser();
                break;
            case 2: {
                InputValidator::displayCenteredTitle("Cancel Booking");
                manager.displayActiveBookings();
                std::string id = InputValidator::boxedInputString("Enter Booking ID to cancel (e.g., B001): ");
                if (manager.cancelBooking(id)) {
                    std::cout << "Booking cancelled!\n";
                } else {
                    std::cout << "Booking not found!\n";
                }
                InputValidator::pauseForUser();
                break;
            }
            case 3: {
                InputValidator::displayCenteredTitle("End Session");
                manager.displayActiveBookings();
                std::string id = InputValidator::boxedInputString("Enter Booking ID to end (e.g., B001): ");
                if (manager.endChargingSession(id)) {
                    std::cout << "Session ended!\n";
                } else {
                    std::cout << "Session not found!\n";
                }
                InputValidator::pauseForUser();
                break;
            }
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayUserPortal() {
    int choice;
    while (true) {
        InputValidator::displayCenteredTitle("USER SIMULATION PORTAL");
        InputValidator::displayCenteredBlock("1. Register New User\n2. Login\n0. Back to Main Menu\n");
        choice = InputValidator::getValidatedIntInput(0, 2, "Enter choice (0-2): ");
        
        switch (choice) {
            case 0:
                return;
            case 1: {
                InputValidator::displayCenteredTitle("Register New User");
                std::string id = InputValidator::boxedInputString("User ID (e.g., U001) (0 to cancel): ", true);
                if (id.empty()) {
                    std::cout << "User registration cancelled.\n";
                    InputValidator::pauseForUser();
                    break;
                }
                std::string name = InputValidator::boxedInputString("Full Name (0 to cancel): ", true);
                if (name.empty()) {
                    std::cout << "User registration cancelled.\n";
                    InputValidator::pauseForUser();
                    break;
                }
                std::string contact = InputValidator::getValidatedPhoneInput(true);
                if (contact.empty()) {
                    std::cout << "User registration cancelled.\n";
                    InputValidator::pauseForUser();
                    break;
                }
                
                int tier = InputValidator::boxedInputInt("User Tier:\n1. Regular\n2. Premium\n3. Fleet\n", 1, 3);
                
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
                InputValidator::displayCenteredTitle("Login");
                std::string id = InputValidator::boxedInputString("User ID: ");
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
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayUserDashboard() {
    if (loggedInUser == NULL) return;
    
    int choice;
    while (true) {
        InputValidator::displayCenteredTitle("USER DASHBOARD");
        std::ostringstream welcome;
        welcome << "========== WELCOME, " << loggedInUser->getName() << " ==========";
        std::ostringstream status;
        status << "Current Tier: " << loggedInUser->getTier() << "\n";
        status << "Wallet Balance: Rs. " << std::fixed << std::setprecision(2) << loggedInUser->getWalletBalance();
        InputValidator::displayCenteredBlock(welcome.str() + "\n" + status.str() + "\n\n1. Search Available Stations\n2. Book a Charging Slot\n3. View My Bookings\n4. Start Charging Session\n5. End Charging Session\n6. View Charging History\n7. Top-up Wallet\n0. Logout\n");
        
        choice = InputValidator::getValidatedIntInput(0, 7, "Enter choice (0-7): ");
        
        switch (choice) {
            case 0:
                loggedInUser = NULL;
                std::cout << "Logged out successfully!\n";
                InputValidator::pauseForUser();
                return;
            case 1: {
                InputValidator::displayCenteredBlock("Available Charging Stations\n");
                manager.displayAllStations();
                InputValidator::pauseForUser();
                break;
            }
            case 2: {
                InputValidator::displayCenteredTitle("Book a Charging Slot");
                manager.displayAllStations();
                    std::string stationID = InputValidator::boxedInputString("Enter Station ID (e.g., ST001): ");
                int minutes = InputValidator::boxedInputInt("Enter charging duration in minutes (15-480): ", 15, 480);
                
                if (manager.bookSlot(loggedInUser->getUserID(), stationID, minutes)) {
                    std::cout << "Booking confirmed!\n";
                } else {
                    std::cout << "Booking failed!\n";
                }
                InputValidator::pauseForUser();
                break;
            }
            case 3: {
                InputValidator::displayCenteredBlock("My Bookings\n");
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
                InputValidator::displayCenteredTitle("Start Charging Session");
                std::vector<Booking*> bookings = manager.findUserBookings(loggedInUser->getUserID());
                if (bookings.empty()) {
                    std::cout << "No bookings available.\n";
                } else {
                    for (size_t i = 0; i < bookings.size(); ++i) {
                        if (bookings[i]->isBooked()) {
                            std::cout << (i+1) << ". " << bookings[i]->getBookingID() << std::endl;
                        }
                    }
                    std::string id = InputValidator::boxedInputString("Enter Booking ID (e.g., B001): ");
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
                InputValidator::displayCenteredTitle("End Charging Session");
                std::vector<Booking*> bookings = manager.findUserBookings(loggedInUser->getUserID());
                if (bookings.empty()) {
                    InputValidator::displayCenteredBlock("No active sessions.\n");
                } else {
                    for (size_t i = 0; i < bookings.size(); ++i) {
                        if (bookings[i]->isActive()) {
                            std::cout << (i+1) << ". " << bookings[i]->getBookingID() << std::endl;
                        }
                    }
                    std::string id = InputValidator::boxedInputString("Enter Booking ID (e.g., B001): ");
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
                InputValidator::displayCenteredBlock("Charging History\n");
                std::cout << "Total Sessions: " << loggedInUser->getTotalSessions() << std::endl;
                InputValidator::pauseForUser();
                break;
            }
            case 7: {
                displayWalletTopUpForUser();
                break;
            }
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayWalletTopUpForUser() {
    if (loggedInUser == NULL) return;
    InputValidator::displayCenteredTitle("WALLET TOP-UP");
    std::ostringstream prompt;
    prompt << "Current Balance: Rs. " << std::fixed << std::setprecision(2)
           << loggedInUser->getWalletBalance() << "\n";
    prompt << "Enter amount to top-up (0 to cancel):";
    double amount = InputValidator::boxedInputDouble(prompt.str(), 0.0, 1000000.0);
    if (amount == 0.0) {
        std::cout << "Wallet top-up cancelled.\n";
        InputValidator::pauseForUser();
        return;
    }
    if (manager.topUpUserWallet(loggedInUser->getUserID(), amount)) {
        std::cout << "Top-up successful! New balance: Rs. "
                  << std::fixed << std::setprecision(2)
                  << loggedInUser->getWalletBalance() << std::endl;
    } else {
        std::cout << "Top-up failed. Please try again." << std::endl;
    }
    InputValidator::pauseForUser();
}

void displayWalletTopUpForAdmin() {
    InputValidator::displayCenteredTitle("ADMIN WALLET TOP-UP");
    manager.displayAllUsers();
    std::string id = InputValidator::boxedInputString("Enter User ID to top-up (e.g., U001): ");
    User* user = manager.findUserByID(id);
    if (user == NULL) {
        std::cout << "User not found!" << std::endl;
        InputValidator::pauseForUser();
        return;
    }
    std::ostringstream prompt;
    prompt << "Current Balance for " << user->getName() << " (" << user->getUserID() << "): Rs. "
           << std::fixed << std::setprecision(2) << user->getWalletBalance() << "\n";
    prompt << "Enter amount to top-up (0 to cancel):";
    double amount = InputValidator::boxedInputDouble(prompt.str(), 0.0, 1000000.0);
    if (amount == 0.0) {
        std::cout << "Wallet top-up cancelled.\n";
        InputValidator::pauseForUser();
        return;
    }
    if (manager.topUpUserWallet(id, amount)) {
        std::cout << "Top-up successful! New balance: Rs. "
                  << std::fixed << std::setprecision(2)
                  << user->getWalletBalance() << std::endl;
    } else {
        std::cout << "Top-up failed. Please try again." << std::endl;
    }
    InputValidator::pauseForUser();
}

void displayWalletManagementMenu() {
    int choice;
    while (true) {
        InputValidator::displayCenteredTitle("WALLET MANAGEMENT");
        InputValidator::displayCenteredBlock("1. List All Wallet Balances\n2. Top-up User Wallet\n3. Search User Wallet by ID\n0. Back to Admin Portal\n");
        choice = InputValidator::getValidatedIntInput(0, 3, "Enter choice (0-3): ");

        switch (choice) {
            case 0:
                return;
            case 1:
                manager.displayAllUsers();
                InputValidator::pauseForUser();
                break;
            case 2:
                displayWalletTopUpForAdmin();
                break;
            case 3: {
                InputValidator::displayCenteredTitle("SEARCH USER WALLET");
                std::string id = InputValidator::boxedInputString("Enter User ID (e.g., U001): ");
                User* user = manager.findUserByID(id);
                if (user == NULL) {
                    std::cout << "User not found!" << std::endl;
                } else {
                    user->displayInfo();
                }
                InputValidator::pauseForUser();
                break;
            }
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayAnalyticsMenu() {
    int choice;
    while (true) {
        InputValidator::displayCenteredTitle("ANALYTICS & REPORTS");
        InputValidator::displayCenteredBlock("1. Revenue Summary Report\n2. Station Utilization Report\n3. Top 10 Highest Revenue Stations\n4. List Inactive Users\n5. System Analytics Report\n6. Export Report to File\n0. Back to Main Menu\n");
        
        choice = InputValidator::getValidatedIntInput(0, 6, "Enter choice (0-6): ");
        
        switch (choice) {
            case 0:
                return;
            case 1:
                manager.generateRevenueReport();
                InputValidator::pauseForUser();
                break;
            case 2:
                manager.generateUtilizationReport();
                InputValidator::pauseForUser();
                break;
            case 3: {
                InputValidator::displayCenteredBlock("Top 10 Revenue Stations\n");
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
                InputValidator::displayCenteredBlock("Inactive Users (>30 days)\n");
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
                InputValidator::displayCenteredBlock("Export Report\n1. Export Stations\n2. Export Users\n");
                int exp = InputValidator::getValidatedIntInput(1, 2, "Enter choice (1-2): ");
                if (exp == 1) {
                    manager.exportToCSV("stations");
                } else {
                    manager.exportToCSV("users");
                }
                InputValidator::pauseForUser();
                break;
            }
            default:
                std::cout << "Invalid choice!\n";
        }
    }
}

void displayMaintenanceMenu() {
    int choice;
    while (true) {
        InputValidator::displayCenteredTitle("SYSTEM MAINTENANCE");
        InputValidator::displayCenteredBlock("1. Create Full System Backup\n2. Restore System from Backup\n3. Export All Stations to CSV\n4. Export All Users to CSV\n5. System Status\n0. Back to Main Menu\n");
        
        choice = InputValidator::getValidatedIntInput(0, 5, "Enter choice (0-5): ");
        
        switch (choice) {
            case 0:
                return;
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

    // Populate a few completed sessions for analytics/demo purposes
    manager.bookSlot("U001", "ST001", 30);
    manager.bookSlot("U002", "ST002", 45);
    manager.bookSlot("U003", "ST003", 60);

    std::vector<Booking*> bks1 = manager.findUserBookings("U001");
    if (!bks1.empty()) { manager.startChargingSession(bks1[0]->getBookingID()); manager.endChargingSession(bks1[0]->getBookingID()); }
    std::vector<Booking*> bks2 = manager.findUserBookings("U002");
    if (!bks2.empty()) { manager.startChargingSession(bks2[0]->getBookingID()); manager.endChargingSession(bks2[0]->getBookingID()); }
    std::vector<Booking*> bks3 = manager.findUserBookings("U003");
    if (!bks3.empty()) { manager.startChargingSession(bks3[0]->getBookingID()); manager.endChargingSession(bks3[0]->getBookingID()); }
}

int main() {
    std::cout << "Loading Smart EV Charging Network Platform...\n";
    // Ask user for preferred console color theme (CMD/Terminal)
    InputValidator::chooseConsoleColor();
    manager.loadAllData();
    
    // Initialize sample data if no data exists
    if (manager.getTotalStations() == 0) {
        initializeSampleData();
        manager.saveAllData();
    }
    
    displayMainMenu();
    return 0;
}
