# Booking Module Explanation

## 1. Purpose

The booking feature in this EV charging platform allows users to reserve a charging slot at a station, track booking status, and transition that reservation into an actual charging session. It manages booking state, pricing estimation, station occupancy, and persistence.

## 2. Key Files

- `Booking.h` / `Booking.cpp`: Defines the `Booking` class and the core booking data model.
- `EVChargingManager.h` / `EVChargingManager.cpp`: Implements booking operations like creating, finding, canceling, starting, and ending bookings.
- `main.cpp`: Contains the user menu flows that invoke booking operations.
- `data/bookings.csv`: Stores booking records for persistence between runs.

## 2.1 Connections to Other Code

- `main.cpp` calls booking operations through `EVChargingManager manager`.
  - `manager.bookSlot(loggedInUser->getUserID(), stationID, minutes)` in the "Book a Charging Slot" flow.
  - `manager.findUserBookings(loggedInUser->getUserID())` in "View My Bookings", "Start Charging Session", and "End Charging Session".
  - `manager.startChargingSession(id)` in the "Start Charging Session" option.
  - `manager.endChargingSession(id)` in the "End Charging Session" option.
  - Admin flows also call `manager.displayActiveBookings()` and `manager.cancelBooking(id)`.
- `EVChargingManager.cpp` contains the functions that connect `Booking` to application logic:
  - `bookSlot()` creates `Booking` objects and reserves station capacity.
  - `findBookingByID()` looks up a booking by the booking ID.
  - `findUserBookings()` returns bookings for a user.
  - `cancelBooking()` cancels a booking and updates station occupancy.
  - `startChargingSession()` updates a booking to active state.
  - `endChargingSession()` closes a booking, computes cost, creates a `ChargingSession`, and removes the booking.
  - `loadBookings()` reads `data/bookings.csv` and rehydrates `Booking` objects on startup.
  - `saveAllData()` / `saveBookings()` persist the current booking state back to `data/bookings.csv`.
- `Booking.cpp` methods such as `displayInfo()`, `isBooked()`, `isActive()`, `getBookingID()`, and `calculateCost()` are used by both `main.cpp` and `EVChargingManager.cpp`.

## 3. Booking Class Overview

### 3.1 Data Members

The `Booking` class stores:

- `bookingID`: unique string ID such as `B0001`.
- `station`: pointer to the reserved `Station`.
- `user`: pointer to the booking `User`.
- `startTime`: `time_t` representing when the booking was created or started.
- `endTime`: `time_t` representing when the session ended.
- `slotDuration`: integer booking duration in minutes.
- `status`: integer status code:
  - `0` = Booked
  - `1` = Active
  - `2` = Completed
  - `3` = Cancelled

### 3.2 Primary Methods

- `calculateCost()`: Uses the associated station and user to compute the booking cost.
- `displayInfo()`: Prints booking details including estimated cost.
- `saveToFile()`: Writes CSV-formatted booking data for persistence.
- `loadFromFile()`: Reads booking data from a saved CSV line.
- `isActive()` and `isBooked()`: Quickly check booking state.
- `getDurationInSeconds()`: Returns `slotDuration` and maps booking minutes to simulation seconds.

## 4. Booking Lifecycle

### 4.1 Slot Reservation

The booking flow starts when a user chooses to book a charging slot in `main.cpp`:

- `manager.bookSlot(userID, stationID, minutes)` is called.
- In `EVChargingManager::bookSlot()`:
  - user and station are validated.
  - station availability is checked with `station->isAvailable()`.
  - a booking ID is generated using `generateID("B")`.
  - a new `Booking` object is created with status `0` (= Booked).
  - the booking is added to `activeBookings`.
  - `station->incrementOccupiedSlots()` is called so the station occupancy reflects the reservation.
  - `saveAllData()` persists updated state to `data/bookings.csv` and other files.

### 4.2 Booking State

A booking starts in state `Booked`.

- `Booked` bookings are visible to users and can be viewed with `findUserBookings()`.
- Users can later start a charging session for a booked reservation.

### 4.3 Starting the Session

When a user starts charging using a booking ID:

- `manager.startChargingSession(bookingID)` is invoked.
- If the booking exists and is not already active:
  - status changes to `1` (= Active).
  - `startTime` is updated to the current time.
  - current data is saved.

### 4.4 Ending the Session

Ending a booking is handled by `EVChargingManager::endChargingSession(bookingID)`: 

- It supports both booked and active bookings.
- The current time is recorded as `endTime`.
- The system computes actual runtime and compares it to the booked duration.
- Billing rules:
  - If the booking was never started (`Booked` state), charge full booked duration.
  - If active and stopped before booked duration, charge full booked duration.
  - Otherwise, bill based on actual elapsed time.
- Cost is calculated by calling the station's `calculatePricePerMin(user)` and multiplying by billed minutes.
- Energy consumed is estimated from station power rating and billed minutes.
- A `ChargingSession` record is created and logged to `data/sessions.log`.
- The user wallet is debited. If funds are insufficient, the user can:
  - top-up the balance,
  - allow a negative balance,
  - or cancel the end action.
- The station occupancy is decremented via `s->decrementOccupiedSlots()`.
- The booking is removed from `activeBookings` and deleted.
- State is persisted again.

### 4.5 Canceling a Booking

Admin or user cancellation uses `EVChargingManager::cancelBooking(bookingID)`:

- booking status is set to `3` (= Cancelled).
- station occupied slots are decremented.
- the booking object is deleted from `activeBookings`.
- data is saved.

## 5. Booking Storage and Persistence

Bookings are persisted in `data/bookings.csv` using the CSV format:

`bookingID,userID,stationID,slotDuration,startTime,endTime,status`

During application startup, `EVChargingManager::loadBookings()` reads the file and reconstructs each booking. It also adjusts `nextBookingNumber` so new booking IDs do not collide with existing ones.

## 6. Booking Querying

Common booking lookup methods:

- `findBookingByID(bookingID)`: returns a pointer to an active booking.
- `findUserBookings(userID)`: returns all active bookings for a user.
- `displayActiveBookings()`: prints every booking currently in memory.

These methods operate on the `activeBookings` vector.

## 7. Booking ID Generation

Booking IDs use a sequential format built by `generateID("B")`:

- `formatSequentialID(prefix, number)` formats IDs like `B0001`.
- The manager increments `nextBookingNumber` until a unique ID is found.
- Existing IDs are also loaded from file to keep the sequence consistent.

## 8. Interview Talking Points

### Architecture
- Booking is a separate entity that composes `Station*` and `User*` pointers.
- It is managed centrally by `EVChargingManager` rather than by station/user classes.
- This preserves single responsibility: `Booking` models reservation data, while `EVChargingManager` orchestrates lifecycle and persistence.

### Business Rules
- Station availability is reserved immediately when a booking is created.
- Booking status tracks where the reservation is in the lifecycle.
- Billing uses a simulation rule where `1 second = 1 minute` to keep runtime testing manageable.
- Early termination of a session still bills the full booked duration.

### Data Flow
1. User picks a station and duration.
2. `bookSlot()` validates user/station and creates a `Booking`.
3. Reservation is stored in memory and saved to CSV.
4. User starts session, changing booking status to Active.
5. User ends session, generating a `ChargingSession` and removing the booking.

### Error Handling
- `bookSlot()` returns `false` if station/user is invalid or station unavailable.
- `startChargingSession()` rejects already-active bookings.
- `endChargingSession()` rejects bookings that are neither booked nor active.
- Insufficient wallet balance triggers a structured flow to resolve payment.

## 9. Example User Flow

1. User logs in.
2. User selects `Book a Charging Slot` from the menu.
3. The system displays stations and requests `stationID` and duration.
4. The system confirms the booking and reserves station capacity.
5. User views bookings via `View My Bookings`.
6. User starts charging with `Start Charging Session`.
7. When charging finishes, user chooses `End Charging Session`.
8. The booking converts into a session record and billing is applied.

## 10. Why This Design Works

- It clearly separates reservation and charging session concepts.
- It supports persistence, recovery, and safe ID generation.
- It keeps booking operations centralized, which simplifies admin actions like cancellation and active booking display.
- The design is flexible enough to extend with custom booking subclasses or advanced scheduling later.

---

This file is intended to help you explain the booking component to an interviewer with code-level clarity and architectural context.
