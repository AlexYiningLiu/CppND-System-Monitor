#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include <unistd.h>

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
  float memTotal;
  float memFree;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  string line, key, value; 
  if(stream.is_open()){
    while(std::getline(stream, line)){
      // so that we don't count the : as a word 
      std::replace(line.begin(), line.end(), ":", " ");
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if(key == "MemTotal"){
          // convert to float 
          memTotal = std::stof(value);
        }
        else if(key == "MemFree"){
          memFree = std::stof(value); 
          return ((memTotal - memFree)/(memTotal*1000));
        }
      }
    }
  }
  return 0; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string value1;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value1; 
  }
  // convert to long 
  return std::stol(value1);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line, key, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  vector<string> cpuTimes{}; 
  if (stream.is_open()){
    // since it's the first line 
    std::getline(stream, line);
    std::istringstream linestream(line); 
    linestream >> key >> user >> nice >> system >> idle >>
          iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    cpuTimes = {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice}; 
  }
  return cpuTimes; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line, key, value; 
  int processes; 
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line); 
    while (std::getline(stream, line)){
      std::istringstream linestream(line); 
      while(linestream >> key >> value){
        if(key == "processes"){
          processes = std::stoi(value); 
          return processes; 
        }
      }
    }
  }
  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line, key, value; 
  int processes; 
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line); 
    while (std::getline(stream, line)){
      std::istringstream linestream(line); 
      while(linestream >> key >> value){
        if(key == "procs_running"){
          processes = std::stoi(value); 
          return processes; 
        }
      }
    }
  }
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  // it's the only line 
  string line, command; 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> command;
    return command; 
  }
  return string(); 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line, key, value; 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ":", " ");
      std::istringstream linestream(line); 
      while(linestream >> key >> value){
        if(key == "VmSize"){
          return value; 
        }
      }    
    }
  }
  return string(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, key, value; 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ":", " ");
      std::istringstream linestream(line); 
      while(linestream >> key >> value){
        if(key == "Uid"){
          return value; 
        }
      }    
    }
  }
  return string(); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid); 
  string line, key, value, x; 
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ":", " ");
      std::istringstream linestream(line); 
      while(linestream >> key >> x >> value){
        if(value == uid){
          return key; 
        }
      }    
    }
  }
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  string line, number;
  string time; 
  int index = 0; 
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> number){
      if (index == 21){
        time = number; 
        break; 
      }
      index++; 
    }
    return (UpTime() - std::stol(time) / sysconf(_SC_CLK_TCK)); 
  }
}
