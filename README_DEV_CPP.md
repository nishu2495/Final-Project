Dev-C++ Project Setup Instructions

Goal
- Open and build the project in Dev-C++ (or with MinGW/g++) with a flat workspace where all `.cpp` and `.h` files are in the repository root.

Quick checklist
1. Ensure Dev-C++ (or MinGW) is installed.
2. Open Dev-C++ and create a new Console project.
3. Add the existing `.cpp` files in the repository root to the project.
4. Set compiler options if needed (e.g. `-std=c++11`).
5. Build and run.

Detailed steps (Dev-C++)
1. Launch Dev-C++.
2. File → New → Project → "Console Application" → C++.
3. Set the Project name (e.g. `Final-Project`) and save the project file in the repository root (choose the workspace folder `/path/to/Final-Project`).
4. From the main menu choose Project → "Add to Project..." and select all `.cpp` files in the project root (e.g., `main.cpp`, `Station.cpp`, `Booking.cpp`, etc.).
5. Optional: Project → Project Options → Parameters → Add `-std=c++11` (or `-std=c++98`) under Compiler options if your Dev-C++ toolchain supports it.
6. Save the project. Build: Execute → Compile & Run (or press Ctrl+F10 / F9 depending on your Dev-C++ version).

Notes for Windows / MinGW users (command-line)
- If you prefer compiling with MinGW (g++) from a terminal, run this in the project root:

```bash
# Build an EXE using g++ (MinGW on Windows)
g++ -std=c++11 *.cpp -o EVPlatform.exe
# Or with explicit optimization and warnings:
g++ -std=c++11 -O2 -Wall *.cpp -o EVPlatform.exe
```

- On Linux/macOS use:

```bash
g++ -std=c++11 *.cpp -o EVPlatform
```

Runtime data
- The program expects the `data/` folder (already in the repo) with files like `stations.csv`, `users.csv`, and `bookings.csv`.
- When running from Dev-C++ set the working directory to the project folder so relative paths to `data/` resolve correctly.

Troubleshooting
- Duplicate symbol / linker errors: Ensure Dev-C++ project contains only the files in the repository root (not the now-deleted `src/` or `include/`).
- Missing header includes: All root sources use `#include "Header.h"` — there should be matching header files in the root.
- If your compiler is older and rejects `-std=c++11`, try `-std=c++98` or remove the flag.

Optional: sample Dev-C++ project file
- Different Dev-C++ versions use slightly different `.dev` formats. Instead of creating a potentially incompatible project file, follow the steps above to create a project from within Dev-C++ and add the existing files.

If you want, I can:
- create a `.dev` project file for Dev-C++ (I can generate a conservative sample), or
- add a small `build.bat` (Windows) that calls `g++` using MinGW, or
- add a `Makefile` for Linux/MinGW builds.

Which of the above would you prefer me to add now?
