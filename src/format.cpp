#include "format.h"
#include <math.h>
#include <string>
using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int hours = floor((seconds) / (3600.0));
  int minutes = floor((seconds - hours * 3600) / 60.0);
  int seconds_ = (seconds - hours * 3600 - minutes * 60);
  return std::to_string(hours) + ":" + std::to_string(minutes) + ":" +
         std::to_string(seconds_);
}