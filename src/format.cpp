#include <string>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long totalSeconds) {
    int hours = totalSeconds / 3600; 
    int minutes = (totalSeconds / 60) % 60;
    int seconds = totalSeconds % 60;
    return std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
    }