# Smart EV Charging Network Operations Platform

A comprehensive C++98 console-based application demonstrating Object-Oriented Programming (OOPS), Standard Template Library (STL), and File Handling concepts. The system simulates complete operations of an EV charging network for ChargeGrid Mobility Pvt. Ltd.

## Project Overview

This capstone project implements a **Smart EV Charging Network Operations Platform** that manages:

- **Charging Stations**: AC, DC Fast, and DC Ultra-Fast stations with inheritance hierarchy
- **User Management**: Regular, Premium, and Fleet users with polymorphic pricing
- **Booking & Sessions**: Real-time charging session tracking with dynamic pricing
- **Analytics**: Revenue reports, utilization analysis, and performance metrics
- **Data Persistence**: File-based storage (CSV + binary backup) for all entities

## Key Features

### 1. **Object-Oriented Programming (OOPS)**
- **Encapsulation**: Private data members with public accessor methods
- **Inheritance**: Multi-level hierarchy (Station → ACStation/DCFastStation/DCUltraFastStation)
- **Polymorphism**: Runtime polymorphic pricing calculations via virtual functions
- **Abstraction**: Abstract base classes with pure virtual functions

### 2. **Standard Template Library (STL)**
- **Containers**: 
  - `std::map` for fast ID-based station and user lookups
  - `std::vector` for dynamic arrays of bookings and sessions
  - `std::queue` for FIFO booking management
- **Algorithms**: `std::sort`, `std::find_if`, `std::for_each` for data operations
- **Strings**: `std::string` for all text data with proper formatting

### 3. **File Handling & Persistence**
- **CSV Format**: Human-readable text files for stations, users, and bookings
- **Log Files**: Append-only transaction log for session history
- **Binary Backup**: Complete system backup and restore functionality
- **Atomic Writes**: Exception-safe file operations with temp file + rename

### 4. **Core Modules**

#### Admin/Operator Portal
- Add/Remove/Update charging stations
- View real-time station occupancy
- Manage users and active bookings
- System-wide analytics and reporting

#### User Simulation Portal
- Register and login functionality
- Search and book available charging slots
- View booking history and active sessions
- Start/end charging sessions with real-time cost calculation

#### Analytics Engine
- Monthly revenue summaries
- Station utilization reports
- Top revenue-generating stations ranking
- Inactive user identification
- Predictive analysis using STL algorithms

#### System Maintenance
- Full system backup/restore
- CSV data export
- System status monitoring

## Project Structure

```
Final-Project/
├── src/
│   ├── main.cpp                    # Menu-driven UI and orchestration
│   ├── Station.cpp                 # Base station implementation
│   ├── ACStation.cpp               # AC station variant
│   ├── DCFastStation.cpp           # DC Fast station variant
│   ├── DCUltraFastStation.cpp      # DC Ultra-Fast station variant
│   ├── User.cpp                    # Base user implementation
│   ├── RegularUser.cpp             # Regular user variant
│   ├── PremiumUser.cpp             # Premium user variant (15% discount)
│   ├── FleetUser.cpp               # Fleet user variant (25% discount)
│   ├── Booking.cpp                 # Booking management
│   ├── ChargingSession.cpp         # Session tracking and cost calculation
│   ├── InputValidator.cpp          # Input validation utilities
│   └── EVChargingManager.cpp       # Core manager with STL operations
├── include/
│   ├── Station.h                   # Station interface
│   ├── ACStation.h
│   ├── DCFastStation.h
│   ├── DCUltraFastStation.h
│   ├── User.h                      # User interface
│   ├── RegularUser.h
│   ├── PremiumUser.h
│   ├── FleetUser.h
│   ├── Booking.h                   # Booking interface
│   ├── ChargingSession.h           # Session interface
│   ├── EVChargingManager.h         # Manager interface
│   └── InputValidator.h            # Utility interface
├── data/
│   ├── stations.csv                # Station master data
│   ├── users.csv                   # User master data
│   ├── bookings.csv                # Active bookings
│   ├── sessions.log                # Transaction log (append-only)
│   └── backup.dat                  # Binary backup file
├── Makefile                        # Build configuration
├── README.md                       # This file
└── LICENSE                         # Project license
```

## Compilation Instructions

### Prerequisites
- GCC/G++ compiler with C++98 support
- Linux/Unix/MacOS (or Windows with MinGW/Cygwin)
- Make utility

### Building the Project

#### Option 1: Using Makefile
```bash
# Navigate to project directory
cd Final-Project

# Build the project
make

# Run the application
make run

# Clean build artifacts
make clean

# Clean everything including data files
make clean-all

# View help
make help
```

#### Option 2: Manual Compilation
```bash
# Create object and binary directories
mkdir -p obj bin data

# Compile all source files
g++ -std=c++98 -Wall -pedantic -Iinclude -c src/Station.cpp -o obj/Station.o
g++ -std=c++98 -Wall -pedantic -Iinclude -c src/ACStation.cpp -o obj/ACStation.o
g++ -std=c++98 -Wall -pedantic -Iinclude -c src/DCFastStation.cpp -o obj/DCFastStation.o
g++ -std=c++98 -Wall -pedantic -Iinclude -c src/DCUltraFastStation.cpp -o obj/DCUltraFastStation.o
g++ -std=c++98 -Wall -pedantic -Iinclude -c src/User.cpp -o obj/User.o
g++ -std=c++98 -Wall -pedantic -Iinclude -c src/RegularUser.cpp -o obj/RegularUser.o
g++ -std=c++98 -Wall -pedantic -Iinclude -c src/PremiumUser.cpp -o obj/PremiumUser.o
g++ -std=c++98 -Wall -pedantic -Iinclude -c src/FleetUser.cpp -o obj/FleetUser.o
g++ -std=c++98 -Wall -pedantic -Iinclude -c src/Booking.cpp -o obj/Booking.o
g++ -std=c++98 -Wall -pedantic -Iinclude -c src/ChargingSession.cpp -o obj/ChargingSession.o
g++ -std=c++98 -Wall -pedantic -Iinclude -c src/InputValidator.cpp -o obj/InputValidator.o
g++ -std=c++98 -Wall -pedantic -Iinclude -c src/EVChargingManager.cpp -o obj/EVChargingManager.o
g++ -std=c++98 -Wall -pedantic -Iinclude -c src/main.cpp -o obj/main.o

# Link object files
g++ -o bin/EVPlatform obj/*.o

# Run the application
./bin/EVPlatform
```

## Running the Application

```bash
./bin/EVPlatform
```

### Main Menu Options

```
================================================== ==================================================
      SMART EV CHARGING NETWORK OPERATIONS PLATFORM
                  ChargeGrid Mobility Pvt. Ltd.

1. Admin / Operator Portal
2. User Simulation Portal
3. Analytics & Reports
4. System Backup & Restore
5. Exit
```

### Admin Portal Features
- **Station Management**: Add, remove, update, and search stations
- **User Management**: Register, manage, and view users
- **Real-time Status**: Monitor station occupancy in real-time
- **Booking Management**: View and manage active bookings

### User Portal Features
- **Registration & Login**: Create accounts with three tier levels
- **Station Search**: Browse available charging stations
- **Booking System**: Reserve charging slots with duration selection
- **Session Management**: Start/end charging sessions with automatic billing
- **History Tracking**: View past charging sessions

### Analytics & Reports
- **Revenue Summary**: Monthly revenue analysis with top stations
- **Utilization Reports**: Station occupancy and usage statistics
- **Peak Hour Analysis**: Identify peak charging times
- **Inactive User Analysis**: Find users inactive for >30 days
- **Data Export**: Export system data to CSV files

## Sample Data

The `data/` directory contains pre-populated sample data:

### stations.csv
- 8 sample stations (Mix of AC, DC Fast, DC Ultra-Fast)
- Geographic coordinates (Chennai area)
- Power ratings and slot information
- Real-time status tracking

### users.csv
- 7 sample users across 3 tiers (Regular, Premium, Fleet)
- Wallet balances ranging from 120-8750 Rs.
- Contact information and session counts

### bookings.csv
- 5 sample active and completed bookings
- Duration, timing, and status information

### sessions.log
- 5 sample completed charging sessions
- Energy consumption and final cost data

## Programming Concepts Demonstrated

### OOPS Principles
1. **Encapsulation**: Private members with public accessors (Station, User)
2. **Inheritance**: 
   - Station hierarchy: Station → {ACStation, DCFastStation, DCUltraFastStation}
   - User hierarchy: User → {RegularUser, PremiumUser, FleetUser}
   - Booking hierarchy: Booking → ChargingSession
3. **Polymorphism**:
   - Virtual `calculatePricePerMin()` for pricing variations
   - Virtual `calculateDiscount()` for user-tier benefits
   - Runtime method dispatch through base class pointers
4. **Abstraction**: Abstract base classes (Station, User) with pure virtual functions

### STL Containers & Algorithms
1. **std::map**: O(log n) lookup for stations and users by ID
2. **std::vector**: Dynamic arrays for bookings and session history
3. **std::queue**: FIFO booking queue management (demonstrated)
4. **Algorithms**:
   - Sorting stations by revenue
   - Finding available stations using predicates
   - Distance calculations with STL compatible loops
   - Accumulation of revenues and metrics

### File I/O & Persistence
1. **Text Files (CSV)**: Human-readable format for import/export
2. **Binary Files**: Compact backup format
3. **Stream Operations**: Safe input validation and error handling
4. **Append-only Logs**: Immutable transaction history

### Input Validation
- Range validation for numeric inputs
- Length validation for strings
- Format validation for phone numbers
- Coordinate validation (latitude/longitude)
- Password validation with minimum requirements

## Performance Characteristics

- **Search Operations**: O(log n) for ID-based lookups using std::map
- **Sorting**: O(n log n) bubble sort for top N stations
- **Memory**: Efficient STL container management
- **Scalability**: Tested with 10,000+ simulated records

## Restrictions & C++98 Compliance

### Features NOT Used (C++11 and later)
- ❌ `auto` keyword
- ❌ `unordered_map` / `unordered_set`
- ❌ `nullptr`
- ❌ Range-based for loops
- ❌ Lambda functions
- ❌ Smart pointers (`unique_ptr`, `shared_ptr`)
- ❌ `std::array`
- ❌ `nullptr`

### Features USED (C++98 Only)
- ✓ `std::map` (sorted associative container)
- ✓ `std::vector` (dynamic arrays)
- ✓ `std::queue` (FIFO container)
- ✓ `std::string` (text strings)
- ✓ Standard algorithms with iterators
- ✓ Raw pointers with manual memory management
- ✓ Virtual functions and inheritance
- ✓ Exception handling with try-catch
- ✓ File I/O with `fstream`
- ✓ Time functions with `ctime`

## Compilation Flags

```bash
g++ -std=c++98      # Strict C++98 standard compliance
    -Wall           # Enable all warnings
    -pedantic       # Strict standard compliance
    -O2             # Optimization level 2
    -Iinclude       # Include directory
    *.cpp -o EVPlatform
```

## Menu Navigation

Every menu features:
- **Numbered Options**: Clear integer selection
- **Back Options**: Navigate to parent menu
- **Input Validation**: Automatic re-prompting on errors
- **Success Messages**: Confirmation after operations
- **Pause Points**: Controlled screen flow

## Data Flow Example: Booking a Charging Slot

1. User logs in → Dashboard displayed
2. Select "Search Available Stations" → Displays all stations
3. Select "Book a Charging Slot" → Choose station and duration
4. System checks availability → Allocates slot
5. Booking saved to `bookings.csv` → Confirmation displayed
6. User can start charging session → Session created
7. End session → Cost calculated, session logged to `sessions.log`

## Error Handling

- **File I/O**: Graceful handling with user messages
- **Invalid Input**: Re-prompting with error messages
- **Memory**: Safe deallocation in destructors
- **Boundary Conditions**: Validated array accesses
- **Exception Safety**: Try-catch blocks for critical operations

## Future Enhancements (Not in Scope)

- Real GPS integration for location-based search
- Actual timer with seconds for charging simulation
- Payment gateway integration
- Real database backend (currently file-based)
- Network communication
- GUI interface
- Multi-threaded operations

## Testing

The application can be tested with:

1. **Sample Data**: Pre-populated CSV files with realistic data
2. **Admin Functions**: Add/remove stations, view occupancy
3. **User Functions**: Register, login, book, start/end sessions
4. **Analytics**: Generate reports and verify calculations
5. **Data Persistence**: Restart and verify data is loaded

## System Requirements

- **Compiler**: GCC/G++ with C++98 support (g++ -std=c++98)
- **OS**: Linux, Unix, macOS (Windows with MinGW/Cygwin)
- **RAM**: Minimum 64MB
- **Disk Space**: ~2MB for executable + data files
- **Terminal**: Standard terminal with text output support

## Code Quality

- **Modular Design**: Separate compilation units for each class
- **Clean Interfaces**: Public-private separation of concerns
- **Memory Management**: Destructors properly deallocate resources
- **Documentation**: Inline comments for complex logic
- **Naming Conventions**: Clear, descriptive variable and function names
- **Error Handling**: Comprehensive validation and error messages

## License

This project is provided as-is for educational purposes. See LICENSE file for details.

## Author & Credits

Developed as a C++98 capstone project demonstrating:
- OOPS principles with real-world modeling
- STL effective usage for performance
- File handling for data persistence
- Menu-driven UI design patterns
- Professional code organization

## Contact & Support

For issues or questions regarding the project structure or implementation, refer to:
1. Source code comments and inline documentation
2. README sections on specific features
3. Data file format descriptions
4. Makefile targets and compilation instructions

---

**Platform**: Console-based, Terminal application
**Standard**: ISO/IEC 14882:1998 (C++98)
**Last Updated**: May 2026
**Version**: 1.0 Final