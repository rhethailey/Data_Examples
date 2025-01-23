/**
 * @file udpManagerClass.h
 * @author Rhet O. Hailey (rhet.hailey@auburn.edu)
 * @brief UDP Class manager used for the DARE project to utilize sending and receiving threaded UDP communications
 * @version 0.1
 * @date 2024-06-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef udpManager_H
#define udpManager_H

// udp include
#include <arpa/inet.h>
#include <atomic>
#include <cstring>
#include <math.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <sstream>
#include <queue>
#include <variant>

// udp define
#define MAX_BUFFER_SIZE 1024

// Queue define to hold most recent values
using MultiType = std::variant<double, std::string>;

// DataType to be shared define to hold most recent values
enum class DataType{
    STRING,
    DOUBLE
};

class UdpManager {
public:
    UdpManager(const std::string& ip, int port, double freq);
    ~UdpManager();

    void setDataType(DataType newDataType);
    void startUdpYap();
    void startUdpListener();
    void stop();
    void updateValue(const std::string& newValue);
    void updateValue(const double& newValue);
    std::atomic<bool>& getStopFlag();
    // std::string getMail();
    // double getMail();
    MultiType getMail();
    // std::variant<double, std::string> getMail();  

private:
    void udpYap();
    void udpListener();

    DataType dataType;  // Add this to indicate the type of data being sent
    std::shared_ptr<std::string> sharedStringPtr;
    std::shared_ptr<double> sharedDoublePtr;
    std::atomic<bool> stopFlag;
    std::string ip;
    int port;
    double freq;
    int interval = (1 / freq) * 1000000000; // convert to ns
    std::thread udpYapThread;
    std::thread udpListenerThread;
    int socketFd;
    std::mutex valueMutex;   // Mutex to protect sharedStringPtr for sending values
    std::mutex receiveMutex; // Mutex to protect sharedStringPtr for receiving values
    std::queue<MultiType> receivedMail;

};

#endif // udpManager_H