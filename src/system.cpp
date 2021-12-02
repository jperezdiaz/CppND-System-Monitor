#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include <chrono>
#include <thread>
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/
System::System()
    : osName_{LinuxParser::OperatingSystem()},
      kernelName_{LinuxParser::Kernel()} {

      };

Processor& System::Cpu() {
  cpu_.update();
  return cpu_;
}

vector<Process>& System::Processes() {
  processes_.clear();
  std::vector<int> pids = LinuxParser::Pids();
  for (int& pid : pids) {
    auto process = Process(pid);
    process.Ram();
    processes_.push_back(process);
  }
  std::sort(processes_.begin(), processes_.end());
  return processes_;
}

std::string System::Kernel() { return kernelName_; }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return osName_; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long System::UpTime() { return LinuxParser::UpTime(); }
