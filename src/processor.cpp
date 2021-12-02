#include "processor.h"
#include <iostream>
#include <string>
#include <vector>
#include "linux_parser.h"
void Processor::update() {
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
  utilization_ = static_cast<float>(totalTime - idleTime) / totalTime;
}

float Processor::Utilization() { return utilization_; }