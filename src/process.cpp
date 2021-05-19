#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return 0; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
    vector<long int> cpuInfo = LinuxParser::GetProcessCpuInfo(pid_); 
    long int uptime = LinuxParser::UpTime(); 
    long int utime = cpuInfo[0];
    long int stime = cpuInfo[1];
    long int cutime = cpuInfo[2];
    long int cstime = cpuInfo[3];
    long int starttime = cpuInfo[4];
    float total_time = utime + stime + cutime + cstime; 
    float seconds = uptime - (starttime / sysconf(_SC_CLK_TCK)); 
    if (seconds == 0.0){
        return 0.0; 
    }
    else{
        return (100 * (total_time / sysconf(_SC_CLK_TCK) / seconds));
    }  
}

// TODO: Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(pid_); 
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    return LinuxParser::Ram(pid_); 
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(pid_); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(pid_); 
}

// TODO: Overload the "greater than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return a.CpuUtilization() < CpuUtilization(); 
}