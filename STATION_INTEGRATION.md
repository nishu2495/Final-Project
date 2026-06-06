# Station Module Integration Guide

This document explains how the station-related code is organized, how it is integrated across the application, and what each major line/block does.

## 1. Purpose of the Station Module

The station module models different charging station types and exposes station state and pricing logic to the rest of the application. Stations are used by:
- the `EVChargingManager` for loading, saving, and managing station records,
- the booking system to reserve charging slots,
- the charging session logic to calculate costs and manage occupancy,
- the UI in `main.cpp` for admin station management.

## 2. Station Base Class (`Station.h` / `Station.cpp`)

### `Station.h`

`Station` is the abstract base class for all station types.

Members:
- `stationID`: unique station identifier.
- `stationType`: type name such as "AC", "DCFast", or "DCUltra".
- `name`: human-readable station name.
- `latitude` / `longitude`: geographic coordinates.
- `powerRating`: power rating in kW.
- `status`: integer representing station state:
  - `0` = Available
  - `1` = Occupied
  - `2` = Faulty
  - `3` = Maintenance
- `maxSlots`: total slots at the station.
- `occupiedSlots`: how many slots are currently taken.

Constructors / destructor:
- `Station()`: default initializes values.
- `Station(std::string id, std::string type, std::string n, double lat, double lon, double power, int slots)`: initializes a station record with real data.
- `virtual ~Station()`: virtual destructor so subclass objects can be deleted polymorphically.

Getters:
- `getStationID()` returns `stationID`.
- `getStationType()` returns `stationType`.
- `getName()` returns `name`.
- `getLatitude()`, `getLongitude()` return coordinates.
- `getPowerRating()` returns `powerRating`.
- `getStatus()` returns `status`.
- `getMaxSlots()` returns `maxSlots`.
- `getOccupiedSlots()` returns `occupiedSlots`.
- `getAvailableSlots()` returns `maxSlots - occupiedSlots`.

Setters and occupancy helpers:
- `setStatus(int s)`: sets the station state if `s` is between 0 and 3.
- `setOccupiedSlots(int slots)`: sets occupied slots if the value is valid.
- `incrementOccupiedSlots()`: increases occupancy if there is still room.
- `decrementOccupiedSlots()`: decreases occupancy if above zero.

Polymorphism:
- `virtual double calculatePricePerMin(User* user) const = 0;`
  - pure virtual method that forces subclasses to implement pricing logic.
- `virtual void displayInfo() const;`
  - prints the shared station details.
- `virtual std::string getDetails() const;`
  - returns a one-line station summary.

File operations:
- `virtual void saveToFile(std::ofstream& file) const;`
  - serializes station fields to CSV.
- `virtual void loadFromFile(const std::string& line);`
  - parses station values from a CSV line.

Utility:
- `bool isAvailable() const;`
  - returns `true` only if `status == 0` and `occupiedSlots < maxSlots`.
- `double getDistance(double lat, double lon) const;`
  - returns Euclidean distance between the station and a coordinate.

### `Station.cpp`

Constructor implementations:
- `Station::Station()` initializes default values with zeros and empty strings.
- `Station::Station(...)` stores arguments and sets `status = 0` and `occupiedSlots = 0`.

Getters and setters are straightforward, returning or updating the stored fields.

Availability and distance:
- `isAvailable()` checks both status and slot capacity.
- `getDistance()` computes the Euclidean distance between station coordinates and a given point.

Display and details:
- `displayInfo()` prints station metadata and status using `std::cout`.
- `getDetails()` builds a short string summarizing station ID, type, power, and available slots.

File serialization:
- `saveToFile()` writes a CSV row in this order:
  `stationID,stationType,name,latitude,longitude,powerRating,status,maxSlots`
- `loadFromFile()` reads the same values from a CSV line and resets `occupiedSlots = 0`.

## 3. Station Subclasses

### `ACStation` (`ACStation.h` / `ACStation.cpp`)

`ACStation` is a concrete subclass representing regular AC chargers.

Members:
- `baseRatePerMin`: default per-minute price.

Constructors:
- default constructor sets type to "AC", power to `7.2`, and rate to `0.5`.
- `ACStation(std::string id, std::string n, double lat, double lon, int slots)` calls base `Station(id, "AC", ...)` with `7.2 kW`.

Overrides:
- `calculatePricePerMin(User* user) const`
  - returns `baseRatePerMin - user->calculateDiscount(baseRatePerMin)`.
  - if `user == NULL`, returns `baseRatePerMin`.
- `displayInfo() const`
  - calls `Station::displayInfo()` then prints the AC base rate.
- `getDetails() const`
  - adds `Rate: Rs.<baseRatePerMin>/min` to the station summary.

### `DCFastStation` (`DCFastStation.h` / `DCFastStation.cpp`)

`DCFastStation` models fast DC charging stations.

Members:
- `maxPower`: stores the maximum supported power.
- `baseRatePerMin`: base price per minute set to `1.2`.

Constructors:
- default constructor sets type to "DCFast", default `power=25.0`, `slots=0`.
- parameterized constructor calls `Station(id, "DCFast", ...)` with a supplied `power`.

Overrides:
- `getMaxPower()` returns `maxPower`.
- `calculatePricePerMin(User* user) const` uses the same discount logic as AC.
- `displayInfo()` prints station info plus max power and rate.
- `getDetails()` adds the rate to the summary.

### `DCUltraFastStation` (`DCUltraFastStation.h` / `DCUltraFastStation.cpp`)

`DCUltraFastStation` models ultra-fast DC chargers.

Members:
- `ultraPower`: stores the fast charger power value.
- `baseRatePerMin`: base rate set to `2.0`.

Constructors:
- default constructor sets type to "DCUltra", default `power=60.0`, `slots=0`.
- parameterized constructor uses the supplied power value.

Overrides:
- `getUltraPower()` returns `ultraPower`.
- `calculatePricePerMin(User* user) const` uses discount logic.
- `displayInfo()` prints ultra power and rate.
- `getDetails()` adds the rate to the summary.

## 4. EVChargingManager Integration

`EVChargingManager` is the central application class that loads station objects, stores them, and routes station operations.

### `EVChargingManager.h` station APIs

Key methods used by the UI and booking flow:
- `void loadStations();`
- `bool addStation(Station* station);`
- `bool removeStation(const std::string& stationID);`
- `Station* findStationByID(const std::string& stationID) const;`
- `std::vector<Station*> findAvailableStations(const std::string& type) const;`
- `std::vector<Station*> findNearestStations(double lat, double lon, int count) const;`
- `void displayAllStations() const;`
- `void updateStationStatus();`

### Loading stations from file: `EVChargingManager::loadStations()`

This method reads `data/stations.csv`.

What it does:
- opens the file `stationsFile`.
- for each non-empty line, it parses `stationID` and `stationType`.
- based on `stationType`, it creates the correct subclass:
  - `AC` → `new ACStation()`
  - `DCFast` → `new DCFastStation()`
  - `DCUltra` → `new DCUltraFastStation()`
- calls `station->loadFromFile(line)` to fill the object.
- stores the pointer in the `stations` map keyed by `stationID`.

Why this matters:
- this is the point where polymorphism enters the system.
- the manager only stores `Station*`, but the actual object can be any concrete station type.
- later calls to `calculatePricePerMin()` or `displayInfo()` use the subclass implementation.

### Saving stations: `EVChargingManager::saveAllData()`

This method writes all station records back to CSV when the app exits or when data is saved.

It iterates over the `stations` map and calls `station->saveToFile(stFile);` for each station.

### Adding a station: `EVChargingManager::addStation(Station* station)`

What it does:
- returns `false` if the pointer is `NULL`.
- gets `stationID` from the object.
- if the ID already exists in the `stations` map, it rejects the addition.
- otherwise stores the new station object in the map.

This is the integration point used by the admin UI when the user creates a new station.

### Removing a station: `EVChargingManager::removeStation(const std::string& stationID)`

What it does:
- looks up the ID in the `stations` map.
- if found, erases the entry and returns `true`.
- if not found, returns `false`.

### Finding a station: `EVChargingManager::findStationByID(...)`

Returns a pointer to the stored `Station` object or `NULL` if not found.

### Displaying stations: `EVChargingManager::displayAllStations()`

Prints all stations by iterating the map and calling `displayInfo()` on each object.

### Updating station status: `EVChargingManager::updateStationStatus()`

What it does:
- shows the list of stations.
- reads station ID from admin input.
- looks up the station and allows changing its status via input.

This is the admin maintenance hook for station health and availability.

## 5. Booking and Session Integration

Station objects are also used during booking and charging.

### Booking: `EVChargingManager::bookSlot(...)`

This method links a user and a station.

Line-by-line behavior:
- `User* user = findUserByID(userID);`
  - fetches the user making the booking.
- `Station* station = findStationByID(stationID);`
  - fetches the station to book.
- `if (user == NULL || station == NULL) return false;`
  - booking fails if either is missing.
- `if (!station->isAvailable()) return false;`
  - station must be available with at least one free slot.
- `Booking* booking = new Booking(bookingID, station, user, minutes);`
  - creates a booking record that stores the `Station*`.
- `station->incrementOccupiedSlots();`
  - marks the station slot as occupied.
- `activeBookings.push_back(booking);`
  - saves the active booking.
- `saveAllData();`
  - persists the updated station state and booking list.

Why this matters:
- station availability directly governs booking success.
- the station object becomes part of the booking so session code can later access it.

### Charging session cost calculation

In charging session logic, a line like:
- `double pricePerMin = b->getStation()->calculatePricePerMin(b->getUser());`

means:
- the session asks the `Station` object for pricing,
- the actual rate depends on the concrete station type,
- the station can apply user discounts through `User`.

This is a core integration point where station type affects billing.

### Occupancy update on session end

When a booking or session ends, the code calls:
- `station->decrementOccupiedSlots();`

so the station becomes available for future bookings.

## 6. Application Integration in `main.cpp`

### Admin station management menu

The station menu is implemented in `displayStationManagement()`.

Important integration points:
- Add station:
  - reads station fields from user input.
  - chooses concrete type based on admin choice.
  - constructs `new ACStation(...)`, `new DCFastStation(...)`, or `new DCUltraFastStation(...)`.
  - calls `manager.addStation(station)`.
- Remove station:
  - calls `manager.removeStation(id)`.
- Update status:
  - calls `manager.updateStationStatus()`.
- List stations:
  - calls `manager.displayAllStations()`.
- Search station:
  - calls `manager.findStationByID(id)` and then `st->displayInfo()`.

### User booking integration

In the user portal, booking uses:
- `manager.bookSlot(loggedInUser->getUserID(), stationID, minutes)`

This connects station availability to the user-facing booking workflow.

### Sample data initialization

The function `initializeSampleData()` creates and registers station objects directly:
- `manager.addStation(new ACStation("ST001", ...));`
- `manager.addStation(new DCFastStation("ST002", ...));`
- `manager.addStation(new DCUltraFastStation("ST003", ...));`

This demonstrates how the main program constructs stations and stores them in the manager.

## 7. Important integration flow summary

1. At start, `main()` calls `manager.loadAllData()`.
2. `EVChargingManager::loadStations()` reads `data/stations.csv` and creates concrete station objects.
3. The manager stores stations in `std::map<std::string, Station*> stations;`.
4. UI functions in `main.cpp` call manager methods for station creation, deletion, display, and update.
5. Booking functions call `station->isAvailable()` and `station->incrementOccupiedSlots()`.
6. Charging sessions call `station->calculatePricePerMin(user)` and `station->decrementOccupiedSlots()`.
7. When saving, `saveAllData()` serializes each station via `station->saveToFile()`.

## 8. How to explain it in an interview

Use this structure:
- Start from the base class: `Station` stores common fields and declares a pure virtual pricing method.
- Explain subclass specialization: `ACStation`, `DCFastStation`, `DCUltraFastStation` each implement pricing and add station-specific data.
- Describe the manager as the integration layer that loads station data from CSV and stores station objects in a map.
- Show how the UI uses the manager to add/remove stations and update their status.
- Point out the booking/session flow where the station object is used to verify availability, reserve slots, and compute cost.

## 9. File references

- `Station.h`
- `Station.cpp`
- `ACStation.h`
- `ACStation.cpp`
- `DCFastStation.h`
- `DCFastStation.cpp`
- `DCUltraFastStation.h`
- `DCUltraFastStation.cpp`
- `EVChargingManager.h`
- `EVChargingManager.cpp`
- `main.cpp`

---

If you want, I can also add a second document with a line-by-line mapping of `main.cpp` and `EVChargingManager.cpp` using actual line numbers for every statement.

## 10. Exact source line references

The following references point to the station-related code locations in this repository.

### Station class definitions
- `Station.h`: class `Station` definition begins at line 10.
- `Station.h`: `incrementOccupiedSlots()` and `decrementOccupiedSlots()` declarations are at lines 44-45.
- `Station.cpp`: `Station::incrementOccupiedSlots()` starts at line 73.
- `Station.cpp`: `Station::decrementOccupiedSlots()` starts at line 79.

### Station subclasses
- `ACStation.h`: `class ACStation` at line 6.
- `ACStation.h`: `calculatePricePerMin(User* user) const` declaration at line 17.
- `ACStation.cpp`: `ACStation::calculatePricePerMin(User* user) const` starts at line 18.
- `DCFastStation.h`: `class DCFastStation` at line 6.
- `DCFastStation.h`: `calculatePricePerMin(User* user) const` declaration at line 19.
- `DCFastStation.cpp`: `DCFastStation::calculatePricePerMin(User* user) const` starts at line 24.
- `DCUltraFastStation.h`: `class DCUltraFastStation` at line 6.
- `DCUltraFastStation.h`: `calculatePricePerMin(User* user) const` declaration at line 19.
- `DCUltraFastStation.cpp`: `DCUltraFastStation::calculatePricePerMin(User* user) const` starts at line 24.

### EVChargingManager station integration
- `EVChargingManager.h`: station operations are declared at lines 40-52 and `bookSlot()` at line 62.
- `EVChargingManager.cpp`: `loadStations()` begins at line 78.
- `EVChargingManager.cpp`: station type object creation occurs at lines 100-104.
- `EVChargingManager.cpp`: `addStation(Station* station)` begins at line 397.
- `EVChargingManager.cpp`: `removeStation(const std::string& stationID)` begins at line 413.
- `EVChargingManager.cpp`: `findStationByID(const std::string& stationID) const` begins at line 423.
- `EVChargingManager.cpp`: `displayAllStations() const` begins at line 468.
- `EVChargingManager.cpp`: `updateStationStatus()` begins at line 484.
- `EVChargingManager.cpp`: `bookSlot(...)` begins at line 581.
- `EVChargingManager.cpp`: `Station* station = findStationByID(stationID);` at line 583.
- `EVChargingManager.cpp`: `station->incrementOccupiedSlots();` at line 593.
- `EVChargingManager.cpp`: `s->decrementOccupiedSlots();` at line 624.

### Booking and pricing integration
- `Booking.cpp`: `double pricePerMin = station->calculatePricePerMin(user);` is at line 91.

### Main application and UI integration
- `main.cpp`: `displayStationManagement()` begins at line 97.
- `main.cpp`: station creation for admin input happens at lines 118-122.
- `main.cpp`: `manager.addStation(station)` is at line 125.
- `main.cpp`: `manager.displayAllStations()` in station menu is at line 136.
- `main.cpp`: `manager.removeStation(id)` is at line 138.
- `main.cpp`: `manager.updateStationStatus()` is at line 147.
- `main.cpp`: `manager.findStationByID(id)` is at line 157.
- `main.cpp`: `manager.displayAllStations()` in user portal is at line 421.
- `main.cpp`: `manager.bookSlot(loggedInUser->getUserID(), stationID, minutes)` is at line 431.
- `main.cpp`: `initializeSampleData()` begins at line 695.
- `main.cpp`: sample station creation lines are 697-701.
- `main.cpp`: sample booking calls are at lines 713-715.
- `main.cpp`: `initializeSampleData()` is called at line 733, and `manager.saveAllData()` at line 734.

These exact references make it easier to point to the code during your interview and explain the integration precisely.