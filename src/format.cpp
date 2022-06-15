#include <string>
#include <iomanip>
#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    int hours;
    int mins;
    int sec;

    hours = seconds / 3600;
    seconds = seconds % 3600;
    mins = seconds / 60;
    sec =  seconds % 60;



    string s_hours, s_mins, s_sec, result;
    s_hours = std::to_string(hours);
    s_mins = std::to_string(mins);
    s_sec = std::to_string(sec);

    // added the below lines as per the recommendation from the first reviewer (alternate implementation that was suggested) as the first implementation seemed to give an 'std::length_error' as per the second reviewer.  
    std::stringstream ss{};

    ss << std::setw(2) << std::setfill('0') << hours
        << std::setw(1) << ":"
        << std::setw(2) << std::setfill('0') << mins
        << std::setw(1) << ":"
        << std::setw(2) << std::setfill('0') << sec;  


    return ss.str(); }
