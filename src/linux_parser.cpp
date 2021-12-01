
#include "linux_parser.h"
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "format.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string memTotal, memFree;
  string key;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    for (int i = 0; i < 2; i++) {
      std::getline(stream, line);
      line.erase(line.end() - 3);
      std::replace(line.begin(), line.end(), ':', ' ');

      std::istringstream linestream(line);

      linestream >> key;
      if (key == "MemTotal") {
        linestream >> memTotal;
      } else if (key == "MemFree") {
        linestream >> memFree;
      } else {
        // throw exception?
      }
    }
  }
  return 1 - stof(memFree) / stof(memTotal);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string upTime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime;
  }
  return stol(upTime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  std::vector<std::string> cpuFields = LinuxParser::CpuUtilization();
  long user = stol(cpuFields.at(LinuxParser::CPUStates::kUser_));
  long nice = stol(cpuFields.at(LinuxParser::CPUStates::kNice_));
  long system = stol(cpuFields.at(LinuxParser::CPUStates::kSystem_));
  long idle = stol(cpuFields.at(LinuxParser::CPUStates::kIdle_));
  long iowait = stol(cpuFields.at(LinuxParser::CPUStates::kIOwait_));
  long irq = stol(cpuFields.at(LinuxParser::CPUStates::kIRQ_));
  long softirq = stol(cpuFields.at(LinuxParser::CPUStates::kSoftIRQ_));
  long steal = stol(cpuFields.at(LinuxParser::CPUStates::kSteal_));

  long idleTime = idle + iowait;
  long nonIdleTime = user + nice + system + irq + softirq + steal;

  long totalTime = idleTime + nonIdleTime;
  return totalTime;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::vector<std::string> cpuFields = LinuxParser::CpuUtilization();
  long user = stol(cpuFields.at(LinuxParser::CPUStates::kUser_));
  long nice = stol(cpuFields.at(LinuxParser::CPUStates::kNice_));
  long system = stol(cpuFields.at(LinuxParser::CPUStates::kSystem_));
  long irq = stol(cpuFields.at(LinuxParser::CPUStates::kIRQ_));
  long softirq = stol(cpuFields.at(LinuxParser::CPUStates::kSoftIRQ_));
  long steal = stol(cpuFields.at(LinuxParser::CPUStates::kSteal_));

  long nonIdleTime = user + nice + system + irq + softirq + steal;

  return nonIdleTime;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::vector<std::string> cpuFields = LinuxParser::CpuUtilization();
  long idle = stol(cpuFields.at(LinuxParser::CPUStates::kIdle_));
  long iowait = stol(cpuFields.at(LinuxParser::CPUStates::kIOwait_));

  long idleTime = idle + iowait;
  return idleTime;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string cpuName, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  guest_nice;
  vector<string> cpuFields;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpuName >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;

    cpuFields.push_back(user);
    cpuFields.push_back(nice);
    cpuFields.push_back(system);
    cpuFields.push_back(idle);
    cpuFields.push_back(iowait);
    cpuFields.push_back(irq);
    cpuFields.push_back(softirq);
    cpuFields.push_back(steal);
    cpuFields.push_back(guest);
    cpuFields.push_back(guest_nice);
  }

  return cpuFields;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string totalProcesses;
  string key;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (key != "processes") {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "processes") {
        linestream >> totalProcesses;
        break;
      }
    }
  }
  return stoi(totalProcesses);
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string runningProcesses;
  string key;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (key != "procs_running") {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "procs_running") {
        linestream >> runningProcesses;
        break;
      }
    }
  }
  return stoi(runningProcesses);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key, value;
  string filename = kProcDirectory + std::to_string(pid) + kStatusFilename;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    while (key != "VmSize" && std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');

      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize") {
        return value;
      }
    }
  }
  return "0";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key, value;
  string filename = kProcDirectory + std::to_string(pid) + kStatusFilename;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    while (key != "Uid") {
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), ':', ' ');

      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid") {
        return value;
      }
    }
  }
  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);

  string line, user, placeholder, currentUid;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (currentUid != uid) {
      std::getline(stream, line);
      std::replace(line.begin(), line.end(), ':', ' ');

      std::istringstream linestream(line);
      linestream >> user >> placeholder >> currentUid;
      if (currentUid == uid) {
        return user;
      }
    }
  }
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  string value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    for (int i = 0; i < 22; i++) {
      linestream >> value;
    }
  }
  return static_cast<float>(stol(value)) / sysconf(_SC_CLK_TCK);
}
