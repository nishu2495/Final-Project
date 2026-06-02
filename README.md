# Smart EV Charging Network Operations Platform

A C++98 console application that simulates the operations of an EV charging network for ChargeGrid Mobility Pvt. Ltd. The project demonstrates OOP principles, STL usage, file handling, and a menu-driven charging management workflow.

## Project Overview

This project implements a **Smart EV Charging Network Operations Platform** with support for:

- **Charging Stations**: AC, DC Fast, and DC Ultra-Fast stations
- **User Management**: Regular, Premium, and Fleet users with different pricing rules
- **Booking & Charging Sessions**: Reserve slots, start and end sessions, and calculate costs
- **Analytics**: Revenue reports, utilization insights, and activity tracking
- **Persistence**: CSV-based storage, logs, and backup/restore support

## Key Features

### Object-Oriented Design
- Encapsulation with private data members and accessors
- Inheritance across station and user type hierarchies
- Polymorphic pricing via virtual methods
- Abstract base classes for shared interfaces

### Standard Template Library
- `std::map` for fast ID lookups
- `std::vector` for managing bookings and sessions
- `std::queue` for booking flows
- `std::sort`, `std::find_if`, and other algorithms for reports and searches

### File Handling & Persistence
- CSV-based storage for station, user, and booking data
- Append-only session logs for transaction history
- Backup and restore capabilities
- Automated data directory creation when building

## Repository Structure

```
Final-Project/
├── main.cpp
├── Station.cpp
├── ACStation.cpp
├── DCFastStation.cpp
├── DCUltraFastStation.cpp
├── User.cpp
├── RegularUser.cpp
├── PremiumUser.cpp
├── FleetUser.cpp
├── Booking.cpp
├── ChargingSession.cpp
├── EVChargingManager.cpp
├── InputValidator.cpp
├── Station.h
├── ACStation.h
├── DCFastStation.h
├── DCUltraFastStation.h
├── User.h
├── RegularUser.h
├── PremiumUser.h
├── FleetUser.h
├── Booking.h
├── ChargingSession.h
├── EVChargingManager.h
├── InputValidator.h
├── Makefile
├── build.bat
├── data/
│   ├── bookings.csv
│   ├── stations.csv
│   ├── users.csv
│   ├── users.csv.bak
│   ├── users.csv.bak2
├── bin/
├── obj/
├── README_DEV_CPP.md
├── LICENSE
├── FEATURES.md
├── PROJECT_SUMMARY.md
├── QUICK_START.md
└── README.md
```

> Note: The `src/` and `include/` directories are present in the repository but are empty in this version of the project.

## Build Instructions

### Prerequisites
- GNU `g++` with C++98 support
- `make` utility on Linux/macOS
- MinGW if building on Windows with `build.bat`

### Build with Makefile
```bash
cd /workspaces/Final-Project
make
```

### Run the app
```bash
make run
```

### Clean up
```bash
make clean
make clean-all
make rebuild
make help
```

### Build on Windows with MinGW
```bash
cd /workspaces/Final-Project
build.bat
```

## Running the Application

After building, run:

```bash
./bin/EVPlatform
```

The program starts a console menu with the following major portals:

- Admin / Operator Portal
- User Simulation Portal
- Analytics & Reports
- System Backup & Restore

## Main Functionality

- Add, update, and manage charging stations
- Register, manage, and view users across tiers
- Book charging slots and manage active sessions
- Track and bill charging sessions automatically
- Generate revenue and utilization reports
- Export data and perform backups

## Notes

- Ensure the working directory is the repository root so the `data/` folder is accessible.
- The application uses relative paths to load and save CSV data.
- Existing sample data is included in `data/` for initial testing.
