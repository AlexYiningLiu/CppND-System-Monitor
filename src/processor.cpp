#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> cpuInfo = LinuxParser::CpuUtilization();
  long int user = std::stol(cpuInfo[0]);
  long int nice = std::stol(cpuInfo[1]);
  long int system = std::stol(cpuInfo[2]);
  long int idle = std::stol(cpuInfo[3]);
  long int iowait = std::stol(cpuInfo[4]);
  long int irq = std::stol(cpuInfo[5]);
  long int softirq = std::stol(cpuInfo[6]);
  long int steal = std::stol(cpuInfo[7]);
  // if no previous times 
  if (init_){
    prevuser = user;
    prevnice = nice;
    prevsystem = system;
    previdle = idle;
    previowait = iowait;
    previrq = irq;
    prevsoftirq = softirq;
    prevsteal = steal;  
    init_ = false; 
    return 0.0; 
  }

  float PrevIdle = previdle - previowait;
  float Idle = idle - iowait; 
  float PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal; 
  float NonIdle = user + nice + system + irq + softirq + steal; 
  float PrevTotal = PrevIdle + PrevNonIdle;
  float Total = Idle + NonIdle;

  prevuser = user;
  prevnice = nice;
  prevsystem = system;
  previdle = idle;
  previowait = iowait;
  previrq = irq;
  prevsoftirq = softirq;
  prevsteal = steal;  

  if(Total-PrevTotal == 0.0){
    return 0.0; 
  }
  else{
    return (((Total-PrevTotal) - (Idle-PrevIdle))/(Total-PrevTotal)); 
  }
}