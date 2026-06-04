#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

#include <string>
#include <iostream>

class InputValidator {
public:
    // Numeric input validation
    static int getValidatedIntInput(int minVal, int maxVal);
    static double getValidatedDoubleInput();
    static double getValidatedDoubleInput(double minVal, double maxVal);

    // String input validation
    static std::string getValidatedStringInput(const std::string& prompt);
    static std::string getValidatedStringInput(const std::string& prompt, bool allowCancel);
    static std::string getValidatedStringInput(const std::string& prompt, 
                                               int minLen, int maxLen);
    static std::string getValidatedStringInput(const std::string& prompt, 
                                               int minLen, int maxLen, bool allowCancel);

    // Contact validation
    static std::string getValidatedPhoneInput();
    static std::string getValidatedPhoneInput(bool allowCancel);

    // Password input
    static std::string getValidatedPasswordInput(const std::string& prompt);

    // Coordinate validation
    static double getValidatedLatitude();
    static double getValidatedLongitude();

    // Pause for user
    static void pauseForUser();

    // Clear screen
    static void clearScreen();

    // Console color utilities
    static void chooseConsoleColor();
    static void displayCenteredTitle(const std::string& title);
    static void displayCenteredBlock(const std::string& text);
    static int getValidatedIntInput(int minVal, int maxVal, const std::string& prompt);
    // Boxed input UI
    static std::string boxedInputString(const std::string& prompt);
    static std::string boxedInputString(const std::string& prompt, bool allowCancel);
    static int boxedInputInt(const std::string& prompt, int minVal, int maxVal);
    static double boxedInputDouble(const std::string& prompt, double minVal, double maxVal);
};

#endif
