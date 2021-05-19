#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  // TODO: Declare any necessary private members
 private:
  long int previdle;
  long int previowait;
  long int prevuser;
  long int prevnice;
  long int prevsystem;
  long int previrq;
  long int prevsoftirq;
  long int prevsteal;

  bool init_{true}; 

};

#endif