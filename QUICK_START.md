# Smart EV Charging Platform - Quick Start Guide

## 🚀 Getting Started in 60 Seconds

### Step 1: Build the Project
```bash
cd Final-Project
make
```

### Step 2: Run the Application
```bash
make run
# or directly
./bin/EVPlatform
```

### Step 3: Explore the Menu
When you see the main menu, choose an option:
```
1. Admin / Operator Portal     ← Manage stations and users
2. User Simulation Portal       ← Test user features
3. Analytics & Reports          ← View reports and analytics
4. System Backup & Restore      ← Backup and restore data
5. Exit                          ← Quit application
```

---

## 📋 Key Features Quick Guide

### For Admins

**Add a Station:**
1. Main Menu → 1 (Admin) → 1 (Station Mgmt) → 1 (Add)
2. Enter Station ID, Name, Location, Type
3. Station added automatically to system

**View All Stations:**
1. Main Menu → 1 (Admin) → 3 (Real-time Status)
2. See all stations with occupancy info

**Manage Users:**
1. Main Menu → 1 (Admin) → 2 (User Mgmt)
2. Add, remove, search, or view users

### For Users

**Register:**
1. Main Menu → 2 (User Portal) → 1 (Register)
2. Enter details and choose tier (Regular/Premium/Fleet)
3. Auto-login and dashboard opens

**Book a Slot:**
1. Dashboard → 2 (Book a Charging Slot)
2. Select station from list
3. Choose duration (15-480 minutes)
4. Booking confirmed

**Start Charging:**
1. Dashboard → 4 (Start Charging Session)
2. Select booking from your list
3. Session starts with timer

**End Session:**
1. Dashboard → 5 (End Charging Session)
2. Cost automatically calculated
3. Session logged to history

### For Analytics

**View Revenue Report:**
1. Main Menu → 3 (Analytics) → 1 (Revenue Summary)
2. See total revenue and top stations

**View Utilization:**
1. Main Menu → 3 (Analytics) → 2 (Utilization Report)
2. See percentage occupancy per station

---

## 🎮 Sample Scenarios

### Scenario 1: Admin Setup (5 minutes)
```
1. Start app → 1 (Admin Portal) → 1 (Station Mgmt) → 1 (Add)
2. Add Station ID: ST099, Name: Test Station, Type: AC
3. Navigate: 1 → 3 → 4 (View All Stations)
4. Your new station appears in list
```

### Scenario 2: User Booking (10 minutes)
```
1. Start app → 2 (User Portal) → 1 (Register)
2. Enter: User ID: U099, Name: Test User, Contact: 9876543210
3. Choose: 1 (Premium)
4. Auto-login → Dashboard opens
5. Select: 1 (Search) → See all stations
6. Select: 2 (Book) → Pick ST001 → Duration: 60 min
7. Booking confirmed
8. Select: 4 (Start) → Session active
9. Select: 5 (End) → Cost calculated
```

### Scenario 3: Analytics (5 minutes)
```
1. Start app → 3 (Analytics & Reports)
2. Select: 1 (Revenue Summary) → See total revenue
3. Back → 2 (Utilization) → See occupancy %
4. Back → 5 (System Analytics) → See all metrics
```

---

## 📁 File Locations

```
Final-Project/
├── bin/EVPlatform           ← Run this executable
├── src/                     ← Source code (.cpp files)
├── include/                 ← Headers (.h files)
├── data/
│   ├── stations.csv         ← Station data
│   ├── users.csv            ← User data
│   ├── bookings.csv         ← Booking data
│   ├── sessions.log         ← Session history
│   └── backup.dat           ← Backup file
├── Makefile                 ← Build configuration
├── README.md                ← Full documentation
├── FEATURES.md              ← Feature list
└── QUICK_START.md           ← This file
```

---

## 🔧 Build Commands Reference

```bash
make              # Build the project
make run          # Build and run
make clean        # Remove build artifacts (keep data)
make clean-all    # Remove everything including data files
make rebuild      # Clean and rebuild
make help         # Show all commands
```

---

## 💡 Tips & Tricks

### Default Credentials
- Default password for all users: **1234**
- Test User IDs: U001-U007 (pre-populated)
- Test Station IDs: ST001-ST008 (pre-populated)

### Wallet Balances (Pre-populated)
- Regular: Rs. 450-890
- Premium: Rs. 1240-2150
- Fleet: Rs. 3420-8750

### Tier Benefits
- **Regular**: No discount
- **Premium**: 15% discount + 50 free minutes/month
- **Fleet**: 25% discount + bulk benefits

### Pricing Per Minute
- AC Station: Rs. 0.5/min (after discount)
- DC Fast: Rs. 1.2/min (after discount)
- DC Ultra: Rs. 2.0/min (after discount)

### Example Booking Cost
```
Station: DC Fast (Rs. 1.2/min)
User: Premium (15% discount)
Duration: 60 minutes
Cost: 1.2 × 60 × (1 - 0.15) = Rs. 61.20
```

---

## 📊 Sample Data Reference

### Pre-populated Stations (8 total)
| ID | Name | Type | Power | Location |
|----|------|------|-------|----------|
| ST001 | Anna Nagar AC Hub | AC | 7.2 kW | 13.0843, 80.2102 |
| ST002 | Guindy DC Fast | DCFast | 25 kW | 13.0067, 80.2206 |
| ST003 | OMR Ultra Fast | DCUltra | 60 kW | 12.9220, 80.1500 |

### Pre-populated Users (7 total)
| ID | Name | Tier | Balance |
|----|------|------|---------|
| U001 | Ramesh Kumar | Regular | Rs. 450.75 |
| U002 | Priya Sharma | Premium | Rs. 1240.50 |
| U003 | Suresh Reddy | Fleet | Rs. 8750.00 |

---

## 🐛 Troubleshooting

### Problem: "Command not found: make"
**Solution**: Install build-essential
```bash
apt-get update && apt-get install -y build-essential
```

### Problem: "g++: command not found"
**Solution**: Install g++ compiler
```bash
apt-get install -y g++
```

### Problem: Data files not loading
**Solution**: Ensure data/ directory exists
```bash
mkdir -p data
```

### Problem: Permission denied for executable
**Solution**: Add execute permission
```bash
chmod +x bin/EVPlatform
```

---

## 📝 Input Validation

The system validates all inputs:

### Phone Number
- Must be exactly 10 digits
- Numbers only (no special characters)

### Station Location
- Latitude: -90 to +90
- Longitude: -180 to +180

### Booking Duration
- Minimum: 15 minutes
- Maximum: 480 minutes (8 hours)

### User ID & Password
- User ID: Any unique string
- Password: Minimum 4 characters

---

## 🎯 Common Workflows

### Workflow 1: Add Station and User
```
1. make run
2. Menu: 1 → 1 → 1 (Add Station)
3. Menu: Back → 2 → 1 (Add User)
4. Menu: Back → 3 → 5 (View System Status)
```

### Workflow 2: Complete Booking Flow
```
1. make run
2. Menu: 2 → 1 (Register User)
3. Menu: 2 (Book Slot)
4. Menu: 4 (Start Session)
5. Menu: 5 (End Session)
6. Menu: 6 (View History)
7. Menu: 7 (Logout)
```

### Workflow 3: Generate Reports
```
1. make run
2. Menu: 3 → 1 (Revenue Summary)
3. Menu: Back → 2 (Utilization)
4. Menu: Back → 5 (System Analytics)
5. Menu: Back → 6 (Export Reports)
```

---

## 📱 Menu Navigation Tips

- **Every menu has a "Back" option** - Use it to go back
- **Press Enter after invalid input** - System re-prompts
- **All operations auto-save** - No need to manually save
- **Type exact values** - ID must match existing record
- **Session pauses** - "Press Enter to continue" gives you time to read

---

## 🚀 Performance Tips

- System handles 10,000+ records efficiently
- Search by ID is fastest (O(log n))
- First run loads sample data automatically
- Data persists between runs
- Reports generate instantly for sample data

---

## 🔒 Data Safety

- **Automatic Backups**: All operations auto-save
- **Backup Feature**: Manual backup available (Menu → 4 → 1)
- **Transaction Log**: All sessions logged to sessions.log
- **CSV Export**: Can export data anytime (Menu → 3 → 6)

---

## 📚 For More Information

- **Full Documentation**: See README.md
- **Feature List**: See FEATURES.md
- **Source Code**: Check src/ and include/ directories
- **Build Details**: See Makefile

---

## ✅ Quick Verification Checklist

After starting the app:
- [ ] Main menu displays correctly
- [ ] Can register a new user
- [ ] Can book a charging slot
- [ ] Can start a charging session
- [ ] Can view analytics reports
- [ ] Can backup and restore data
- [ ] All menu options work
- [ ] Data persists after exit/restart

**Everything working? Great! You're all set! 🎉**

---

**Last Updated**: May 2026
**Version**: 1.0
**Platform**: Linux/Unix/MacOS (console)
