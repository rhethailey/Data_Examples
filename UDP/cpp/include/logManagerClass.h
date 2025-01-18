#ifndef logManagerClass_H
#define logManagerClass_H

#include <atomic>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>
#include <thread>

class LogManager {
public:
    /// @param fs - refresh rate in ms
    LogManager(int freq);
    LogManager(const std::string& logFileName, int freq);
    ~LogManager();

    /// @brief push header to file
    void printLogHeader(std::string& myHeader);

    /// @brief data logger to use with threads
    /// @param myData - text to push into file
    void logData(std::string& myData);
    // void logData(std::reference_wrapper<std::string>(myData));

    std::atomic<bool>& getStopFlag();

    void startLogging();
    void stopLogging();
    
    /// @brief if you want to log an individual line
    /// @param lineData 
    void logDataLine(std::string lineData);

private:
    void logDataLine();
    void loggingThread();
    auto now();
    auto nextWrite();

    std::string grabLogFileName();
    std::string getCurrentDateTime();
    
    std::ofstream logFile;
    int freq;                 // Hz
    int fs = (1000/freq); // ms
    int iter = 0;

    std::mutex stringMutex;
    std::mutex fileMutex;
    std::thread loggerThread;
    std::atomic<bool> stopFlag;  // why?// why?
    std::atomic<bool> dataReady; // why?
    std::string* currentData;     // why?
};

#endif // logManagerClass_H