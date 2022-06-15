#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

void Process::Setid(int id){
this->pid = id;
}

// DONE: Return this process's ID
int Process::Pid() const { return this->pid; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { return LinuxParser::ProcessCPU(Pid()); }

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// DONE: Return this process's memory utilization
string Process::Ram() const  { return LinuxParser::Ram(Pid()); }

// DONE: Return the user (name) that generated this process
string Process::User() {return LinuxParser::User(Pid()); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime() - LinuxParser::UpTime(Pid());}

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return std::stoi(this->Ram()) > std::stoi((a).Ram());
}
