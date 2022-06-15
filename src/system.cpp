#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// DONE: Return the system's CPU
Processor& System::Cpu() {

    return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
   
     vector<int> pidlist{LinuxParser::Pids()};

     processes_ = {};

     for(auto i: pidlist){
            Process Proc;
            Proc.Setid(i);
            processes_.emplace_back(Proc);
	}

    std::sort(processes_.begin(),processes_.end(),[](const Process& a, const Process& b){return a<b;}); 

    return processes_; }

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long System::UpTime() { return LinuxParser::UpTime(); }
