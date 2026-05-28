#include "../include/InputValidator.h"
#include <limits>
#include <cstdlib>

int InputValidator::getValidatedIntInput(int minVal, int maxVal) {
    int input;
    while (true) {
        std::cout << "Enter choice (" << minVal << "-" << maxVal << "): ";
        std::cin >> input;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        
        if (input < minVal || input > maxVal) {
            std::cout << "Input out of range! Please enter between " 
                     << minVal << " and " << maxVal << ".\n";
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return input;
    }
}

double InputValidator::getValidatedDoubleInput() {
    double input;
    while (true) {
        std::cout << "Enter value: ";
        std::cin >> input;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a valid number.\n";
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return input;
    }
}

double InputValidator::getValidatedDoubleInput(double minVal, double maxVal) {
    double input;
    while (true) {
        std::cout << "Enter value (" << minVal << "-" << maxVal << "): ";
        std::cin >> input;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        
        if (input < minVal || input > maxVal) {
            std::cout << "Input out of range! Please enter between " 
                     << minVal << " and " << maxVal << ".\n";
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return input;
    }
}

std::string InputValidator::getValidatedStringInput(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        if (input.empty()) {
            std::cout << "Input cannot be empty! Please try again.\n";
            continue;
        }
        
        return input;
    }
}

std::string InputValidator::getValidatedStringInput(const std::string& prompt, 
                                                     int minLen, int maxLen) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        if (input.empty() || input.length() < minLen) {
            std::cout << "Input too short! Minimum " << minLen << " characters.\n";
            continue;
        }
        
        if (input.length() > maxLen) {
            std::cout << "Input too long! Maximum " << maxLen << " characters.\n";
            continue;
        }
        
        return input;
    }
}

std::string InputValidator::getValidatedPhoneInput() {
    std::string input;
    while (true) {
        std::cout << "Enter phone number (10 digits): ";
        std::getline(std::cin, input);
        
        if (input.length() != 10) {
            std::cout << "Invalid phone number! Must be 10 digits.\n";
            continue;
        }
        
        bool allDigits = true;
        for (size_t i = 0; i < input.length(); ++i) {
            if (input[i] < '0' || input[i] > '9') {
                allDigits = false;
                break;
            }
        }
        
        if (!allDigits) {
            std::cout << "Invalid phone number! Must contain only digits.\n";
            continue;
        }
        
        return input;
    }
}

std::string InputValidator::getValidatedPasswordInput(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        if (input.empty() || input.length() < 4) {
            std::cout << "Password too short! Minimum 4 characters.\n";
            continue;
        }
        
        return input;
    }
}

double InputValidator::getValidatedLatitude() {
    return getValidatedDoubleInput(-90.0, 90.0);
}

double InputValidator::getValidatedLongitude() {
    return getValidatedDoubleInput(-180.0, 180.0);
}

void InputValidator::pauseForUser() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void InputValidator::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void InputValidator::displaySeparator(const std::string& title) {
    if (title.empty()) {
        std::cout << "\n=================================================="
                  << "==================================================\n";
    } else {
        std::cout << "\n========== " << title << " ==========\n";
    }
}
