#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  int totalmemory;
  int freememory;

  std::ifstream filestream{kProcDirectory + kMeminfoFilename};
  
  if(filestream.is_open()){
    for(int i =0; i < 2; i++){
      std::getline(filestream, line);
      std::istringstream linestream(line);
      while(linestream >> key >> value){
	if(key == filterMemTotalString){
	  totalmemory = (std::stoi(value,nullptr));
	}
	if(key == filterMemFreeString ){
	  freememory = (std::stoi(value,nullptr));
	}
      }
    }
    return 1.0 - (static_cast<float>(freememory)/(totalmemory));
  }

  throw std::runtime_error("invalid memory utilization");
}

// DONE -  Read and return the system uptime
long int LinuxParser::UpTime() {
	std::string uptime, idletime, line;
	std::ifstream filestream{kProcDirectory + kUptimeFilename};
	long result;

	if(filestream.is_open()){
		std::getline(filestream,line);
		std::istringstream linestream(line);
		linestream >> uptime;
	}
	result = (std::stol(uptime));

	return result;

}


// DONE: Read and return CPU utilization
// used https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux to calculat cpu utilization 
float LinuxParser::CpuUtilization() {
   
    string  user, nice, system, idle, iowait, irq, softirq, steal, guest, guestnice;
    string line;
    string key;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestnice;
    }

  int Idle, NonIdle, Total;
  Idle = std::stoi(idle,nullptr) + std::stoi(iowait,nullptr);
  NonIdle = std::stoi(user,nullptr) + std::stoi(nice,nullptr) + std::stoi(system,nullptr) + std::stoi(irq,nullptr) +std::stoi(softirq,nullptr) + std::stoi(steal,nullptr);
  Total = Idle + NonIdle;
  return (static_cast<float>(NonIdle)/Total);

}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {

  string line;
  string key;
  string value;

  std::ifstream filestream{kProcDirectory + kStatFilename};
  
  if(filestream.is_open()){
      while(std::getline(filestream,line)){
          std::istringstream linestream(line);
          while(linestream >> key >> value){
              if(key == filterProcesses){
                  return std::stoi(value,nullptr);
              }
          }
      }
  }
throw std::runtime_error("invalid number of total processes");

}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {

  string line;
  string key, value;
  int processes;

  std::ifstream filestream{kProcDirectory + kStatFilename};
  
  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
	if(key == filterRunningProcesses){
	  processes  = (std::stoi(value,nullptr));
	}
      }
    }
  }
  return processes;}


// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
string pid_s {to_string(pid)};
string line, result;

std::ifstream filestream{kProcDirectory + pid_s + kCmdlineFilename};
if(filestream.is_open()){
	std::getline(filestream, line);
	std::istringstream linestream(line);
	linestream >> result; 
}
return result;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(const int pid) {
string pid_s {to_string(pid)};
string line, key, result, value;
int kilobyte{0}, megabyte;

std::ifstream filestream{kProcDirectory + pid_s + kStatusFilename};

  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
          // used VMRSS instead of VmSize as per the recommendation of the first reviewer
          // the underlying reason is that VmRSS gives exact physical memory instead of all the virutal memory as VmSize does
	if(key == filterProcMem){
	  kilobyte  = (std::stoi(value,nullptr));
	}
      }
    }
}
if(kilobyte == 0){
return "0";
} else{
megabyte = kilobyte/1024;
return to_string(megabyte);
}
}
// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {

string pid_s {to_string(pid)};
string line, key, result, value;

std::ifstream filestream{kProcDirectory + pid_s + kStatusFilename};

  if(filestream.is_open()){
    while(std::getline(filestream,line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
	if(key == filterUID){
	  return value;
	}
      }
    }
}
throw std::runtime_error("invalid pid");
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {

    string user, uid1, uid2, line;
    std::ifstream filestream{kPasswordPath};
    if(filestream.is_open()){
        while(std::getline(filestream, line)){
            std::replace(line.begin(),line.end(), ':', ' ');
            std::istringstream linestream(line);
            while(linestream >> user >> uid1 >>  uid2){
                if(uid2 == (LinuxParser::Uid(pid))){
                    return user;
                }
            }
        }
    }
throw std::runtime_error("invalid pid");

}

// DONE: Read and return the uptime of a process
long int LinuxParser::UpTime(int pid) {

string pid_s {to_string(pid)};
string line, key, result, value;

std::ifstream filestream{kProcDirectory + pid_s + kStatFilename};

string comm, state;
string  pid_stat, ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt, majflt, cmajflt, utime, stime, cutime, cstime, priority, nice, num_threads, itrealvalue, starttime;

  if(filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream >> pid_stat >> comm >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads >> itrealvalue >> starttime;
  }


 long int uptime {std::stol(starttime,nullptr)/sysconf(_SC_CLK_TCK)};

    return uptime;}

// used https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599 to determine CPU calc for processes
float LinuxParser::ProcessCPU(int pid)
{

string pid_s {to_string(pid)};
string line, key, result, value;

std::ifstream filestream{kProcDirectory + pid_s + kStatFilename};


string comm, state;
string pid_stat, ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt, majflt, cmajflt, utime, stime, cutime, cstime, priority, nice, num_threads, itrealvalue, starttime;

  if(filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream >> pid_stat >> comm >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads >> itrealvalue >> starttime;
  }




  int proc_time_total =  std::stoi(utime,nullptr) + std::stoi(stime,nullptr) + std::stoi(cutime,nullptr) + std::stoi(cstime,nullptr);
  int proc_time_start = LinuxParser::UpTime() - (std::stoi(starttime,nullptr)/sysconf(_SC_CLK_TCK));
  float proc_cpu = (static_cast<float>(proc_time_total)/sysconf(_SC_CLK_TCK))/proc_time_start;

    return proc_cpu;
}
