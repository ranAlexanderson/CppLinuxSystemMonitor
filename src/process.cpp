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

Process::Process(int pid){
    pid_ = pid;
    user_ = LinuxParser::User(pid_);
    command_ = LinuxParser::Command(pid_);
    ram_ = LinuxParser::Ram(pid_);
    cpuUsage_ = LinuxParser::CpuUtilization(pid_);
}

// TODO: Return this process's ID
int Process::Pid() { return pid_;  }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    return cpuUsage_;
 }

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return  ram_;}

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a ) const { 
    if (this->cpuUsage_ < a.cpuUsage_)
        return true;
    else
        return false;
    }