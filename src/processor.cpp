#include "processor.h"
#include "linux_parser.h"
#include <vector>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<long> utildata = LinuxParser::CpuUtilization();
    float cpu_util = 0.0;
    long prevTotal = 0; 
    long prevIdle = 0;
    if(prevTotal!=0){
        long total = utildata[0];
        long idle = utildata[1];
        long difftotal = total - prevTotal;
        long diffidle = idle - prevIdle;
        cpu_util = (difftotal-diffidle)/difftotal;
    }

    prevTotal = utildata[0];
    prevIdle = utildata[1];
    return cpu_util;
}