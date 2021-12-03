#include "format.h"
#include <math.h>
#include <iomanip>
#include <string>
using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int hours = floor((seconds) / (3600.0));
  int minutes = floor((seconds - hours * 3600) / 60.0);
  int seconds_ = (seconds - hours * 3600 - minutes * 60);
  std::stringstream ss{};

  ss << std::setw(2) << std::setfill('0') << hours      // HH
     << std::setw(1) << ":"                             // :
     << std::setw(2) << std::setfill('0') << minutes    // MM
     << std::setw(1) << ":"                             // :
     << std::setw(2) << std::setfill('0') << seconds_;  // SS

  return ss.str();
}