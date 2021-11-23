#include <dirent.h>
#include <unistd.h>
#include <sstream>
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key = "";
  string value;
  float totalMemory = 0;
  float freeMemory = 0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open())
  {
      
    while (key.find("MemTotal") == std::string::npos)
      {
        std::getline(filestream,line);
        std::istringstream linestream(line);
        linestream >> key >>  value;
      }
      try
      {
        totalMemory = std::stof(value);
      }
      catch(const std::exception& e)
      {
        return 0.0f;
      }

      while (key.find("MemFree") == std::string::npos)
      {
        std::getline(filestream,line);
        std::istringstream linestream(line);
        linestream >> key >>  value;
      }

      try
      {
        freeMemory = std::stof(value);
      }
      catch(const std::exception& e)
      {
        return 0.0f;
      }
      return (totalMemory-freeMemory)/totalMemory;
      
  }
  return 0.0f; }

// TODO: Read and return the system uptime
// returns 1 when the value could not be parsed
// returns 2 when file could not be opened
long LinuxParser::UpTime() {
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
      std::getline(filestream,line);
      std::istringstream linestream(line);
      linestream >> value;
      try
      {
        float totalSecs = std::stof(value);
        return long(totalSecs);
      }
      catch(const std::exception& e)
      {
        return 1;
      }        
  }
  return 2;
 }

// TODO: Read and return the number of jiffies for the system
//long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
//long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
//long LinuxParser::ActiveJiffies() {}

// TODO: Read and return the number of idle jiffies for the system
//long LinuxParser::IdleJiffies() {}

// TODO: Read and return CPU utilization. Will be done in Processor.cpp
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "processes") {
          linestream >> value;
          return std::stoi(value);  
        }
      }
    }
  }
  return 0;
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "procs_running") {
          linestream >> value;
          return std::stoi(value);  
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]]> once you define the function
string LinuxParser::Command(int pid) { 
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + "/" + kCmdlineFilename);
  std::string line;
  if (filestream.is_open())
  {
    std::getline(filestream,line);
    return line;  
  }
  return "not found";
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
// returns -1 when value cannot be parsed.
// returns 0 when key is not found.
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + "/" + kStatusFilename);
  
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "VmSize:")
      {
        linestream >> value;
        float ramInKb = 0;
        try
        {
          ramInKb = std::stof(value);
          float ram = ramInKb*kiloByteToMegatByte;
          std::ostringstream out;
          out.precision(2);
          out << std::fixed << ram;
          return out.str();

        }
        catch(const std::exception& e)
        {
          return "-1";
        }
        
      }  
    }
  }
  return "0"; 
}

// TODO: Read and return the user ID associated with a process
// returns an empty string if the file could not be opened*
string LinuxParser::Uid(int pid[[maybe_unused]]) { 
  std::string line;
  std::string valueName;
  std::string uid;
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + "//status");    
  if (filestream.is_open())
    {
      while (std::getline(filestream,line))
      {
        std::istringstream linestream(line);
        linestream >> valueName;
        if (valueName.find("Uid") != std::string::npos)
        {
          linestream >> uid;
          filestream.close();
          return uid;
        }
      }        
    }
  return string(); 
  }



// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  std::string userName;
  std::string line;
  std::string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);    
  std::string subStr = ":" + uid + ":" + uid; 
  std::ifstream passwdFilestream(kPasswordPath);    
  if (passwdFilestream.is_open())
  {
    while (std::getline(passwdFilestream,line))
    {
      if (line.find(subStr) != std::string::npos)
      {
        std::replace(line.begin(),line.end(), ':' , ' ');
        std::istringstream linestream(line);
        std::string user;
        linestream >> user;
        passwdFilestream.close();
        return user;

      }
    }        
  } 
  else
    return string("Val missing"); 
  return string("File Error"); 
  }

// TODO: Read and return the uptime of a process
// returns the Uptime in seconds.
// When the file could not be opened the method returns 0
// When the value could not be parsed from the file the method returns -1

long LinuxParser::UpTime(int pid) 
{ 
  std::string line;
  std::string value;
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + "//stat");    
  if (filestream.is_open())
    {
        std::getline(filestream,line);
        std::istringstream linestream(line);
        int i = 0;
        while (i<22 && linestream >> value)
            {
                i++;
            }
        filestream.close();
        try
        {
          long int ticks = stol(value);
          long seconds = ticks/sysconf(_SC_CLK_TCK);  
          return (UpTime() - seconds);  
        }
        catch(const std::exception& e)
        {
          return -1;
        }
        
        
    }
  return 0; 
  }

float LinuxParser::CpuUtilization(int pid)
{ 
  std::string line;
  std::string value;
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + "//stat");    
  if (filestream.is_open())
    {
        std::getline(filestream,line);
        std::istringstream linestream(line);
        int i = 1;
        long uTime = 0;
        long sTime = 0;
        long ticks = 0;
        while (i<=22 && linestream >> value)
            {
              try
              {
                if (i == 14)
                  uTime = std::stol(value);
                if (i == 15)
                  sTime = std::stol(value); 
                if (i == 22)
                  ticks = std::stol(value); 
              }
              catch(const std::exception& e)
              {
                return 0.0f;
              }
              i++;
            }
        filestream.close();
        long processTotalSeconds = ticks/sysconf(_SC_CLK_TCK);  
        return (float)(100 * ((float)(uTime+sTime)/sysconf(_SC_CLK_TCK))/(UpTime()- processTotalSeconds));
        }
  return 0; 
  }
