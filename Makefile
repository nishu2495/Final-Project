# Makefile for Smart EV Charging Network Operations Platform
# C++98 Compliant Build Configuration

CXX = g++
CXXFLAGS = -std=c++98 -Wall -pedantic -O2
LDFLAGS = 

# Source and header directories (flat root layout)
SRC_DIR = .
INC_DIR = .
OBJ_DIR = obj
BIN_DIR = bin

# Target executable
TARGET = EVPlatform

# Source files (all root .cpp files)
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Include paths
CXXFLAGS += -I$(INC_DIR)

# Default target
all: create_dirs $(BIN_DIR)/$(TARGET)

# Create necessary directories
create_dirs:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p data

# Linking
$(BIN_DIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Build successful! Executable: $@"

# Compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the application
run: all
	@./$(BIN_DIR)/$(TARGET)

# Clean build artifacts
clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Clean complete!"

# Clean all including data files
clean-all: clean
	@rm -rf data/*.csv data/*.log data/*.dat
	@echo "All files cleaned!"

# Rebuild
rebuild: clean all

# Help target
help:
	@echo "Smart EV Charging Network Operations Platform - Build Guide"
	@echo "============================================================"
	@echo "make              - Build the project"
	@echo "make run          - Build and run the application"
	@echo "make clean        - Remove build artifacts"
	@echo "make rebuild      - Clean and build again"
	@echo "make clean-all    - Remove all generated files including data"
	@echo "make help         - Display this help message"

.PHONY: all create_dirs run clean clean-all rebuild help
