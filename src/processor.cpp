#include "processor.h"
#include "linux_parser.h"
// Will return the current cpu usage according to the following formula:
// Reading the data from the first line in /proc/stat
// cpu  userProcess nicedProcess systemProcess idle iowait ServicingIterrupts ServicingSoftInterruprs
// The function will sum all the values from userProcess nicedProcess systemProcess ServicingIterrupts and ServicingSoftInterruprs
// into totalJiffiesInUse
// The function will sum idle and iowait into totalJiffiesNotUsed
// every time the function will be called it will calculate how many delta Jiffies used since last call 
// and delta totalJiffiesNotUsed and will calculate the percentage of JiffiesUsed/TotaJiffiesSinceLastCall
float Processor::Utilization() { 
    long busyJiffiesCurrentCall = 0;
    long idleJiffiesCurrentCall = 0;

    std::string line;
    std::string key = "";
    std::string value;
    std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (filestream.is_open())
    {
        std::istringstream stream;
        while (key != "cpu")
        {
            std::getline(filestream,line);
            stream = std::istringstream(line);
            stream >> key;
        }

        int valueNumber = 0;
        while (stream >> value)
        {
            if (valueNumber == 3)
                idleJiffiesCurrentCall = std::stol(value);
            else
                busyJiffiesCurrentCall += std::stol(value);
            valueNumber++;
        }
        
        if (this->lastTotalJiffiesUsed == 0)
        {
            this->lastTotalJiffiesUsed = busyJiffiesCurrentCall;
            this->lastTotalJiffiesInIdle = idleJiffiesCurrentCall;
            return 0.0f;
        }
        else
        {
            int totalJiffiesSinceBoot = (busyJiffiesCurrentCall + idleJiffiesCurrentCall);
            float totalJiffiesSinceLastCall = (float)(totalJiffiesSinceBoot - (this->lastTotalJiffiesUsed + this->lastTotalJiffiesInIdle));
            float jiffiesUsedSinceLastCall = (float)(busyJiffiesCurrentCall - this->lastTotalJiffiesUsed);
            this->lastTotalJiffiesUsed = busyJiffiesCurrentCall;
            this->lastTotalJiffiesInIdle = idleJiffiesCurrentCall;

            return jiffiesUsedSinceLastCall/totalJiffiesSinceLastCall;
        }
    }
  return 0;
 }