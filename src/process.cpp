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
    string line;
    string placeholder;
    string spid = to_string(pid_);
    long ut, st, cut, cst, stt, tot_t, upt;
    std::ifstream stream_cputil(LinuxParser::kProcDirectory + spid + LinuxParser::kStatFilename);
    if(stream_cputil.is_open()){
        std::getline(stream_cputil, line);
        std::istringstream linestream (line);
        for (int i=1; i<=22; i++){
            linestream >> placeholder;
            if(i==14){
                ut = stol(placeholder);
            }
            else if(i==15){
                st = stol(placeholder);
            }
            else if(i==16){
                cut = stol(placeholder);
            }
            else if(i==17){
                cst = stol(placeholder);
            }
            else if(i==22){
                stt = stol(placeholder);
            }
        }
    }
    tot_t = ut + st + cut + cst;
    upt = LinuxParser::UpTime();
    float seconds = upt - stt/sysconf(_SC_CLK_TCK);
    float cpu_usage = ((tot_t/sysconf(_SC_CLK_TCK))/seconds)*100;
    return cpu_usage;
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
    return LinuxParser::UpTime(pid_);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }