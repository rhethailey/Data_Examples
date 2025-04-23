#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include <cmath>

// lazy definition of now()
auto now() {
    return std::chrono::high_resolution_clock::now();
}

int main()
{    
    // define freqency and time
    double freq = 1000;                            // 1000 Hz
    int interval = (1000 / freq);                  // convert to ms
    auto dt = std::chrono::milliseconds(interval); // convet to std::chrono


    // open logFile
    std::ofstream logFile;
    std::string logFileName = "myLogFile.txt";
    logFile.open(logFileName);


    // define start time & next loop start
    auto start = now();
    auto nextLoop = start + std::chrono::milliseconds(dt);;
    
    // run 100 samples
    for(int i = 0; i < 100; i++){

        // sleep until
        std::this_thread::sleep_until(nextLoop);


        // time info for pretty data logging
        auto timePoint = std::chrono::system_clock::to_time_t(now());
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()) % 1000;
        auto timeinfo = std::localtime(&timePoint);
        
        {   
            // time logging: clock time, iteraction count
            logFile << std::put_time(timeinfo, "%H:%M:%S") << "." << std::setfill('0') << std::setw(3) << ms.count() << "," << std::to_string(i) << ",";

            // data logging: sin(pi * i / freq)
            logFile << std::to_string(sin(i * 3.14 / freq)) << "\n";
        }

        // define next loop based on refresh rate
        nextLoop += std::chrono::milliseconds(dt);
    }

    // close file
    logFile.close();
}