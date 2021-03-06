#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

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
  string os, version, kernel;
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
        int pid = std::stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  string MemTotal;
  string MemFree;
  std::ifstream streammeminfo(kProcDirectory + kMeminfoFilename);
  if(streammeminfo.is_open()){
    while(std::getline(streammeminfo, line)){
      //std::replace(line.begin(), line.end(), ' ', ' ');
      //std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "MemTotal:"){
          MemTotal = value;
        }
        else if (key == "MemFree:"){
          MemFree = value;
        }
      }
    }
  }
  return ((stof(MemTotal)-stof(MemFree)))/stof(MemTotal);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
   string uptime;
   string line;
   std::ifstream streamuptime(kProcDirectory + kUptimeFilename);
   if(streamuptime.is_open()){
     std::getline(streamuptime, line);
     std::istringstream linestream(line);
     linestream >> uptime;
   }
   return std::stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long sum = 0;
  string line;
  vector<string> utildata;
  string cpu_name, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream stream_jiffy(kProcDirectory + kStatFilename);
  if(stream_jiffy.is_open()){
    while(std::getline(stream_jiffy, line)){
      std::istringstream linestream(line);
      linestream >> cpu_name >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
      if(cpu_name == "cpu"){
        utildata = {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
      }
    }
  }
  for(int i = 0;i < 10;i++){
    sum += std::stol(utildata[i]);
  }
  return sum;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string spid = to_string(pid);
  long val = 0;
  string data;
  std::ifstream stream_upt(kProcDirectory + spid + kStatFilename);
  if(stream_upt.is_open()){
    std::getline(stream_upt, line);
    std::istringstream linestream(line);
    for(int count = 1; count<=22 ; count++){
      linestream >> data;
      if(count==14 || count==15 || count==16 || count==17){
        val += stol(data);
      }
    }
  }
  return val;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return (LinuxParser::Jiffies()-LinuxParser::IdleJiffies());
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line;
  vector<string> utildata;
  string cpu_name, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream stream_idlejiffy(kProcDirectory + kStatFilename);
  if(stream_idlejiffy.is_open()){
    while(std::getline(stream_idlejiffy, line)){
      std::istringstream linestream(line);
      linestream >> cpu_name >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
      if(cpu_name == "cpu"){
        utildata = {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
      }
    }
  }
  return (stol(utildata[3])+stol(utildata[4])); 
}

// TODO: Read and return CPU utilization
std::vector<long> LinuxParser::CpuUtilization() { 
  vector<long> utildata;
  string line;
  long idle_time = LinuxParser::IdleJiffies();
  long total_time = LinuxParser::Jiffies();
  utildata.push_back(total_time);
  utildata.push_back(idle_time);
  return utildata;

  //string cpu_name, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  //std::ifstream streamcpu(kProcDirectory + kStatFilename);
  //if(streamcpu.is_open()){
    //while(std::getline(streamcpu, line)){
      //std::istringstream linestream(line);
      //linestream >> cpu_name;
      //if (cpu_name == "cpu"){
        //linestream >> cpu_name >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
        //utildata = {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
        //return utildata;
      //}
      //else continue;
    //}
  //}
  //return utildata;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string totalno;
  string line;
  string word;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> word >> totalno;
      if (word == "processes"){
        return std::stoi(totalno);
      }
      else continue;
    }
  }
  return std::stoi(totalno);
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string running_no;
  string line;
  string word;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> word >> running_no;
      if (word == "procs_running"){
        return std::stoi(running_no);
      }
      else continue;
    }
  }
  return std::stoi(running_no);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  string spid = to_string(pid);
  std::ifstream stream_cmd(kProcDirectory + spid + kCmdlineFilename);
  if(stream_cmd.is_open()){
    std::getline(stream_cmd, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string first_word;
  string spid = to_string(pid);
  string ram;
  int rami;
  std::ifstream stream_ram(kProcDirectory + spid + kStatusFilename);
  if(stream_ram.is_open()){
    while(std::getline(stream_ram, line)){
      std::istringstream linestream(line);
      linestream >> first_word >> ram;
      if(first_word == "VmSize:"){
        rami = stoi(ram)/1024;;
      }
      else continue;
    }
  }
  return to_string(rami);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string first_word;
  string uid;
  string spid = to_string(pid);
  std::ifstream stream_uid(kProcDirectory + spid + kStatusFilename);
  if(stream_uid.is_open()){
    while(std::getline(stream_uid, line)){
      std::istringstream linestream(line);
      linestream >> first_word >> uid;
      if(first_word == "Uid:"){
        return uid;
      }
      else continue;
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line;
  string suid = LinuxParser::Uid(pid);
  //string find_this = ":x:" + suid;
  string user;
  string x;
  string uid;
  std::ifstream stream_user(kPasswordPath);
  if(stream_user.is_open()){
    while(std::getline(stream_user, line)){
      std::replace(line.begin(),line.end(),':',' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> uid;
      if(uid==suid){
        return user;
      }
      else continue;
      //if(line.find(find_this)!=string::npos){
        //std::replace(line.begin(),line.end(),":"," ");
        //std::istringstream linestream(line);
        //linestream >> user;
      //}
      //else continue;
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  string spid = to_string(pid);
  string data;
  std::ifstream stream_upt(kProcDirectory + spid + kStatFilename);
  if(stream_upt.is_open()){
    std::getline(stream_upt, line);
    std::istringstream linestream(line);
    for(int count = 0; count<22 ; count++){
      linestream >> data;
    }
  }  
  return (stol(data)/sysconf(_SC_CLK_TCK));
}