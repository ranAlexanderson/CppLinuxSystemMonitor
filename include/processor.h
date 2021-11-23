#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long lastTotalJiffiesUsed = 0;
  long lastTotalJiffiesInIdle = 0;
};

#endif