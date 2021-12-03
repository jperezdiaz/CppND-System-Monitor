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

int Process::Pid() { return pid_; }

float Process::CpuUtilization() {
  return (static_cast<float>(LinuxParser::ActiveJiffies(pid_)) /
          sysconf(_SC_CLK_TCK)) /
         LinuxParser::UpTime(pid_);
}

string Process::Command() { return command_; }

string Process::Ram() {
  ram_ = stol(LinuxParser::Ram(pid_)) / 1000;
  return std::to_string(ram_);
}

string Process::User() { return user_; }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a[[maybe_unused]]) const {
  return a.ram_ < ram_;
}