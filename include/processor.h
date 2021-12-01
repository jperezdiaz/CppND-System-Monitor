#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  void update();
  float Utilization();  // TODO: See src/processor.cpp

 private:
  float utilization_;
};

#endif