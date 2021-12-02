#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_{pid} {
  uid_ = LinuxParser::Uid(pid);
  user_ = LinuxParser::User(pid);
  command_ = LinuxParser::Command(pid);
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  return 100 * (static_cast<float>(LinuxParser::ActiveJiffies(pid_)) /
                sysconf(_SC_CLK_TCK)) /
         LinuxParser::UpTime(pid_);
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() {
  ram_ = stol(LinuxParser::Ram(pid_)) / 1000;
  return std::to_string(ram_);
}

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const {
  return a.ram_ < ram_;
}