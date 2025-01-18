/**
 * @file udpManagerClass.cpp
 * @author Rhet O. Hailey (rhet.hailey@auburn.edu)
 * @brief Logger Class functions used for the DARE project to utilize sending and receiving dynamic logging
 * @version 0.1
 * @date 2024-07-09
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "logManagerClass.h"

// if *NO* custom fileName is desired
LogManager::LogManager(int freq)
    : stopFlag(false)
    , dataReady(false)
    , freq(freq) {
    std::string logFileName = grabLogFileName();
    logFile.open(logFileName);
}

// if custom fileName is desired
LogManager::LogManager(const std::string& logFileName, int freq)
    : stopFlag(false)
    , dataReady(false)
    , freq(freq) {
    logFile.open(logFileName);
}

LogManager::~LogManager() {
    if (!stopFlag.load()) {stopLogging();}
    if (logFile.is_open()) { logFile.close(); }
}

std::string LogManager::getCurrentDateTime() {
    std::time_t now = std::time(nullptr);
    std::tm* ptm = std::localtime(&now);

    std::stringstream ss;
    ss << std::put_time(ptm, "%Y_%m_%d");
    return ss.str();
}

/// @brief generate file name with task, subject, and session
std::string LogManager::grabLogFileName() {
    std::string userInput;
    std::cout << "Enter filename: ";
    if (std::getline(std::cin, userInput)) {
        std::istringstream iss(userInput);
    }
    userInput = "log/" + userInput + ".txt";
    std::cout << "\n";
    return userInput;
}

    // std::string fileName;
    // std::string dateString = getCurrentDateTime();

    // int subjectNumber;
    // int sessionNumber;
    // int taskType;
    // std::string taskString;

    // // customizable filename types
    // std::vector<std::string> options = {"test", "perturb"};

    // std::cout << "Enter subject number: ";
    // std::cin >> subjectNumber;

    // std::cout << "Enter session number: ";
    // std::cin >> sessionNumber;

    // std::cout << "Enter Task Type (0: test, 1: Perturbation, 2... ): ";
    // std::cin >> taskType;

    // // Error Handling
    // while (taskType > options.size() - 1) {
    //     std::cout << "\nSilly goose, try again (0: test, 1: Perturbation, 2... ): ";
    //     std::cin >> taskType;
    // }

    // taskString = options[taskType];

    // fileName = dateString + "_Sub" + std::to_string(subjectNumber) + "_T" + std::to_string(sessionNumber) + taskString +
    //            ".txt";

//     return fileName;
// }

/// @brief push header to file - may need to be changed
/// @param fs
void LogManager::printLogHeader(std::string& myHeader) {
    logFile << "TIME,ITERATION,";
    logFile << myHeader <<"\n";
}


/// @brief push update data & publish
/// @param myData 
void LogManager::logData(std::string& myData) {
    std::lock_guard<std::mutex> lock(stringMutex);
    currentData = &myData;
    dataReady = true;
}

std::atomic<bool>& LogManager::getStopFlag() {
    return stopFlag;
}

void LogManager::startLogging() {
    loggerThread = std::thread(&LogManager::loggingThread, this);
}

void LogManager::stopLogging() {
    stopFlag = true;
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // for (int i=0;i<50;i++){
    //     if (loggerThread.joinable()) { loggerThread.join(); std::cout<<"joined";}   
    //     std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    // }
    // loggerThread.join();
    // loggerThread.detach();
    // std::cout << "Before join\n";    
    if (loggerThread.joinable()) { loggerThread.join(); }
    // loggerThread.detach();   
    // std::cout << "Close file\n";
    // logFile.close();

    
    // std::cout << "before close\n";
    
    // std::cout << "logManager stopLogging after close\n";
    // loggerThread.join();

    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

auto LogManager::now() {
    return std::chrono::high_resolution_clock::now();
}

auto LogManager::nextWrite() {
    using std::chrono::operator""ms;
    // return now() + 0.1ms;
    auto dt = std::chrono::milliseconds(static_cast<long long int>(LogManager::fs));
    return now() + dt;
}

void LogManager::logDataLine() {
    auto start{now()};
    // for pretty data logging
    auto timePoint = std::chrono::system_clock::to_time_t(now());
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()) % 1000;
    auto timeinfo = std::localtime(&timePoint);
    // log data guarded by mutex
    {   
        std::lock_guard<std::mutex> lock(fileMutex);
        logFile << std::put_time(timeinfo, "%H:%M:%S") << "." << std::setfill('0') << std::setw(3) << ms.count() << "," << std::to_string(iter) << ",";
        logFile << *currentData << "\n";
    }
    iter++;
}

/// @brief if you want to log an individual line
/// @param lineData 
void LogManager::logDataLine(std::string lineData) {
    auto start{now()};
    // for pretty data logging
    auto timePoint = std::chrono::system_clock::to_time_t(now());
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()) % 1000;
    auto timeinfo = std::localtime(&timePoint);

    // log data guarded by mutex
    {
        std::lock_guard<std::mutex> lock(fileMutex);
        logFile << std::put_time(timeinfo, "%H:%M:%S") << "." << std::setfill('0') << std::setw(3) << ms.count() << "," << std::to_string(iter) << ",";
        logFile << lineData << "\n";
    }
    iter++;
}

void LogManager::loggingThread() {
    
    // now + dt();
    auto loopNext = nextWrite();
    int i = 0;
    while (!stopFlag) {
        auto start{now()};
        std::this_thread::sleep_until(loopNext);
        
        if (stopFlag) break;  // Check the flag immediately after waking up

        // for pretty data logging
        auto timePoint = std::chrono::system_clock::to_time_t(now());
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()) % 1000;
        auto timeinfo = std::localtime(&timePoint);
        // log data by line
        
        {   
            std::lock_guard<std::mutex> lock(stringMutex);
            logDataLine();
        }

        // add dt to keep loop rates
        loopNext += std::chrono::milliseconds(static_cast<long long int>(LogManager::fs));
        i++;
    }
}