#include "InputValidator.h"
#include <limits>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <vector>

// ANSI color codes
static const std::string ANSI_RESET = "\033[0m";
static const std::string ANSI_RED = "\033[31m";
static const std::string ANSI_GREEN = "\033[32m";
static const std::string ANSI_YELLOW = "\033[33m";
static const std::string ANSI_BLUE = "\033[34m";
static const std::string ANSI_MAGENTA = "\033[35m";
static const std::string ANSI_CYAN = "\033[36m";

static std::string _currentColor = "";

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
        std::ostringstream rangePrompt;
        rangePrompt << std::fixed << std::setprecision(2) << minVal << "-" << maxVal;
        std::cout << "Enter value (" << rangePrompt.str() << "): ";
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
    return getValidatedStringInput(prompt, false);
}

std::string InputValidator::getValidatedStringInput(const std::string& prompt, bool allowCancel) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        if (allowCancel && input == "0") {
            return std::string("");
        }
        
        if (input.empty()) {
            std::cout << "Input cannot be empty! Please try again.\n";
            continue;
        }
        
        return input;
    }
}

std::string InputValidator::getValidatedStringInput(const std::string& prompt, 
                                                     int minLen, int maxLen) {
    return getValidatedStringInput(prompt, minLen, maxLen, false);
}

std::string InputValidator::getValidatedStringInput(const std::string& prompt, 
                                                     int minLen, int maxLen, bool allowCancel) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        if (allowCancel && input == "0") {
            return std::string("");
        }
        
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
    return getValidatedPhoneInput(false);
}

std::string InputValidator::getValidatedPhoneInput(bool allowCancel) {
    std::string input;
    while (true) {
        std::cout << "Enter phone number (10 digits): ";
        std::getline(std::cin, input);
        
        if (allowCancel && input == "0") {
            return std::string("");
        }
        
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
    std::string prefix = _currentColor.empty() ? "" : _currentColor;
    std::string suffix = _currentColor.empty() ? "" : ANSI_RESET;

    if (title.empty()) {
        std::cout << prefix << "\n=================================================="
                  << "==================================================\n" << suffix;
    } else {
        std::cout << prefix << "\n========== " << title << " ==========" << std::endl << suffix;
    }
}

void InputValidator::chooseConsoleColor() {
    std::cout << "\nConsole color theme: Magenta\n";
    _currentColor = ANSI_MAGENTA;
}

void InputValidator::setConsoleColorByChoice(int choice) {
    _currentColor = ANSI_MAGENTA;
}

void InputValidator::resetConsoleColor() {
    _currentColor.clear();
    std::cout << ANSI_RESET;
}

std::string InputValidator::currentColorCode() {
    return _currentColor;
}

void InputValidator::displayCenteredTitle(const std::string& title) {
    clearScreen();
    std::string prefix = _currentColor.empty() ? "" : _currentColor;
    std::string suffix = _currentColor.empty() ? "" : ANSI_RESET;
    const int width = 80;
    int pad = (int)((width - (int)title.length()) / 2);
    if (pad < 0) pad = 0;

    std::cout << "\n" << std::string(width, '=') << std::endl;
    for (int i = 0; i < pad; ++i) std::cout << ' ';
    std::cout << prefix << title << suffix << std::endl;
    std::cout << std::string(width, '=') << std::endl << std::endl;
}

void InputValidator::displayCenteredBlock(const std::string& text) {
    std::string prefix = _currentColor.empty() ? "" : _currentColor;
    std::string suffix = _currentColor.empty() ? "" : ANSI_RESET;
    const int termWidth = 80;
    std::vector<std::string> lines;
    std::string line;

    for (size_t i = 0; i <= text.length(); ++i) {
        if (i == text.length() || text[i] == '\n') {
            lines.push_back(line);
            line.clear();
        } else {
            line += text[i];
        }
    }

    size_t maxLineLength = 0;
    for (size_t i = 0; i < lines.size(); ++i) {
        if (lines[i].length() > maxLineLength) {
            maxLineLength = lines[i].length();
        }
    }

    const int boxWidth = (int)std::min<size_t>(maxLineLength + 4, termWidth - 10);
    int pad = (termWidth - boxWidth) / 2;
    if (pad < 0) pad = 0;
    std::string left(pad, ' ');
    std::string border = "+" + std::string(boxWidth - 2, '-') + "+";

    std::cout << left << prefix << border << suffix << std::endl;
    for (size_t i = 0; i < lines.size(); ++i) {
        std::string content = lines[i];
        if (content.length() > (size_t)boxWidth - 4) {
            content = content.substr(0, boxWidth - 4);
        }
        std::cout << left << prefix << "| " << content;
        std::cout << std::string(boxWidth - 4 - content.length(), ' ') << " |" << suffix << std::endl;
    }
    std::cout << left << prefix << border << suffix << std::endl << std::endl;
}

int InputValidator::getValidatedIntInput(int minVal, int maxVal, const std::string& prompt) {
    int input;
    std::string prefix = _currentColor.empty() ? "" : _currentColor;
    std::string suffix = _currentColor.empty() ? "" : ANSI_RESET;
    const int width = 80;

    while (true) {
        int pad = (int)((width - (int)prompt.length()) / 2);
        if (pad < 0) pad = 0;
        for (int i = 0; i < pad; ++i) std::cout << ' ';
        std::cout << prefix << prompt << suffix;
        std::cin >> input;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nInvalid input! Please enter a number.\n";
            continue;
        }
        
        if (input < minVal || input > maxVal) {
            std::cout << "\nInput out of range! Please enter between " 
                     << minVal << " and " << maxVal << ".\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return input;
    }
}

static void printBoxed(const std::string& content) {
    const int boxW = 60;
    const int termW = 80;
    int pad = (termW - boxW) / 2;
    if (pad < 0) pad = 0;
    std::string left(pad, ' ');
    std::string border = "+" + std::string(boxW - 2, '-') + "+";
    std::cout << "\n" << left << border << std::endl;
    int innerW = boxW - 4; // space for | and padding

    std::vector<std::string> lines;
    std::string currentLine;
    for (size_t i = 0; i < content.length(); ++i) {
        char c = content[i];
        if (c == '\n') {
            lines.push_back(currentLine);
            currentLine.clear();
        } else {
            currentLine += c;
        }
    }
    lines.push_back(currentLine);

    for (size_t i = 0; i < lines.size(); ++i) {
        const std::string& rawLine = lines[i];
        size_t pos = 0;
        while (pos < rawLine.length()) {
            std::string part = rawLine.substr(pos, innerW);
            std::ostringstream oss;
            oss << "| " << part;
            std::string out = oss.str();
            if ((int)out.length() < boxW - 1) {
                out += std::string((boxW - 1) - out.length(), ' ');
            }
            out += "|";
            std::cout << left << (_currentColor.empty() ? std::string("") : _currentColor) << out << (_currentColor.empty() ? std::string("") : ANSI_RESET) << std::endl;
            pos += innerW;
        }
        if (rawLine.empty()) {
            std::string emptyLine = "| " + std::string(innerW, ' ') + " |";
            std::cout << left << (_currentColor.empty() ? std::string("") : _currentColor) << emptyLine << (_currentColor.empty() ? std::string("") : ANSI_RESET) << std::endl;
        }
    }
    std::cout << left << border << std::endl << std::endl;
}

std::string InputValidator::boxedInputString(const std::string& prompt) {
    return boxedInputString(prompt, false);
}

std::string InputValidator::boxedInputString(const std::string& prompt, bool allowCancel) {
    printBoxed(prompt);
    std::string pad( ((80-60)/2) + 2, ' ');
    return getValidatedStringInput(pad + "> " + prompt, allowCancel);
}

int InputValidator::boxedInputInt(const std::string& prompt, int minVal, int maxVal) {
    printBoxed(prompt);
    std::string pad( ((80-60)/2) + 2, ' ');
    std::cout << pad << "> ";
    return getValidatedIntInput(minVal, maxVal);
}

double InputValidator::boxedInputDouble(const std::string& prompt, double minVal, double maxVal) {
    printBoxed(prompt);
    std::string pad( ((80-60)/2) + 2, ' ');
    std::cout << pad << "> ";
    return getValidatedDoubleInput(minVal, maxVal);
}
