#pragma once
#include <cctype>

bool isStringDigit(const std::string& str) {
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}

std::string getDigitString(int number, int numDigits) {
    std::ostringstream oss;
    oss << std::setw(numDigits) << std::setfill('0') << number;
    return oss.str();
}
