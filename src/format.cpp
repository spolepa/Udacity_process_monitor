#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(const long seconds) { 
    int secs, mins, hrs;
    hrs = seconds/3600;
    secs = seconds%3600;
    mins = secs/60;;
    secs = secs%60;
    string str = std::to_string(hrs) + ":" + std::to_string(mins) + ":" + std::to_string(secs);
    return str;
}