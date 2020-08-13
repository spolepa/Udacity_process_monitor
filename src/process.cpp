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
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    auto hz = sysconf(_SC_CLK_TCK);
    long process_active = LinuxParser::ActiveJiffies(pid_)/hz;
    long uptime = LinuxParser::UpTime();
    float cpu_util = float(process_active)/float(uptime);
    return cpu_util;
}

// TODO: Return the command that generated this process
string Process::Command() { 
    return (LinuxParser::Command(pid_));
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return (LinuxParser::Ram(pid_));
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(pid_);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return (LinuxParser::UpTime()-LinuxParser::UpTime(pid_));
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }