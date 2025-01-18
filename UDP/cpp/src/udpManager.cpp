/**
 * @file udpManagerClass.cpp
 * @author Rhet O. Hailey (rhet.hailey@auburn.edu)
 * @brief UDP Class functions used for the DARE project to utilize sending and receiving threaded UDP communications
 * @version 0.1
 * @date 2024-06-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "udpManagerClass.h"

// UdpManager::UdpManager(const std::string& ip, int port, int interval)
//     : ip(ip)
//     , port(port)
//     , interval(interval)
//     , stopFlag(false)
//     , sharedStringPtr(std::make_shared<std::string>("")) {
//     socketFd = socket(AF_INET, SOCK_DGRAM, 0);
//     if (socketFd == -1) { throw std::runtime_error("Failed to create socket"); }
// }

UdpManager::UdpManager(const std::string& ip, int port, double freq)
    : ip(ip)
    , port(port)
    , freq(freq)
    , stopFlag(false)
    , sharedStringPtr(std::make_shared<std::string>("dummy"))
    , sharedDoublePtr(std::make_shared<double>(2.0)) // Initialize sharedDoublePtr
{
    socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFd == -1) { throw std::runtime_error("Failed to create socket"); }
}

UdpManager::~UdpManager() {
    stop();
    if (udpYapThread.joinable()) udpYapThread.join();
    if (udpListenerThread.joinable()) udpListenerThread.join();
    close(socketFd);
}

void UdpManager::setDataType(DataType newDataType) {
    std::lock_guard<std::mutex> lock(valueMutex); // Protect against concurrent access
    dataType = newDataType;
}

void UdpManager::startUdpYap() {
    stopFlag = false;
    udpYapThread = std::thread(&UdpManager::udpYap, this);
}

void UdpManager::startUdpListener() {
    stopFlag = false;
    udpListenerThread = std::thread(&UdpManager::udpListener, this);
    std::cout << "*** Started listener ***\n";
}

void UdpManager::stop() {
    stopFlag = true;
    if (udpYapThread.joinable()) udpYapThread.join();
    if (udpListenerThread.joinable()) udpListenerThread.join();
}

void UdpManager::updateValue(const std::string& newValue) {
    std::lock_guard<std::mutex> lock(valueMutex);
    *sharedStringPtr = newValue;
}

void UdpManager::updateValue(const double& newValue) {
    std::lock_guard<std::mutex> lock(valueMutex);
    *sharedDoublePtr = newValue;
};

std::atomic<bool>& UdpManager::getStopFlag() {
    return stopFlag;
}

// deprecated from string
// void UdpManager::udpYap() {
//     sockaddr_in serverAddress;
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
//     serverAddress.sin_port = htons(port);

//     auto startTime = std::chrono::high_resolution_clock::now();
//     auto nextSendTime = startTime + std::chrono::milliseconds(interval);

//     while (!stopFlag.load()) {
//         auto now = std::chrono::high_resolution_clock::now();
//         if (now >= nextSendTime) {
//             nextSendTime = now + std::chrono::milliseconds(interval);

//             std::string message;
//             {
//                 std::lock_guard<std::mutex> lock(valueMutex);
//                 auto elapsedSeconds = std::chrono::duration<double>(now - startTime).count();
//                 std::ostringstream oss;
//                 oss.precision(3);
//                 oss << std::fixed << elapsedSeconds << "," << *sharedStringPtr;
//                 // oss << *sharedStringPtr;
//                 message = oss.str();
//             }

//             sendto(socketFd, message.c_str(), message.length(), 0, (sockaddr*)&serverAddress, sizeof(serverAddress));

//             // std::cout << "Sent message: " << message << std::endl;
//         }
//     }
// }

void UdpManager::udpYap() {
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
    serverAddress.sin_port = htons(port);

    auto startTime = std::chrono::high_resolution_clock::now();
    auto nextSendTime = startTime + std::chrono::nanoseconds(interval);
    double newValue = 1.0;

    while (!stopFlag.load()) {
        auto now = std::chrono::high_resolution_clock::now();
        if (now >= nextSendTime) {
            nextSendTime = now + std::chrono::nanoseconds(interval);

            if (dataType == DataType::STRING) {
                std::string message;
                {
                    std::lock_guard<std::mutex> lock(valueMutex);
                    message = *sharedStringPtr; // Use the shared string value
                }
                sendto(
                    socketFd, message.c_str(), message.length(), 0, (sockaddr*)&serverAddress, sizeof(serverAddress));

                // Optional: Debugging
                // std::cout << "Sent string: " << message << std::endl;

            } else if (dataType == DataType::DOUBLE) {
                double value;
                {
                    std::lock_guard<std::mutex> lock(valueMutex);
                    value = *sharedDoublePtr; // Use the shared double value
                }
                sendto(socketFd, &value, sizeof(value), 0, (sockaddr*)&serverAddress, sizeof(serverAddress));
                // Optional: Debugging
                // std::cout << "Sent double: " << value << std::endl;

            } else {
                std::cout << "DataType Initialization Failed" << std::endl;
                break;
            }
        }
    }
}

double UdpManager::getMail() {
    // std::cout << "tried to get mail\n";

    std::lock_guard<std::mutex> lock(receiveMutex);
    if (!receivedMail.empty()) {
        double message = receivedMail.front();
        receivedMail.pop();

        // std::cout << "Mess age from UDP: " << message << std::endl;
        return message;

    // } else {
    //     // std::cout << "Did not receive UDP" << std::endl;
    //     return 3.1; // Return empty string if no message received
    }
}

// void UdpManager::udpListener() {
//     sockaddr_in serverAddress;
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
//     serverAddress.sin_port = htons(port);

//     if (bind(socketFd, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
//         throw std::runtime_error("Failed to bind socket");
//     }

//     char buffer[MAX_BUFFER_SIZE];

//     while (!stopFlag.load()) {
//         sockaddr_in clientAddress;
//         socklen_t clientAddressLen = sizeof(clientAddress);

//         int bytesReceived =
//             recvfrom(socketFd, buffer, MAX_BUFFER_SIZE - 1, 0, (sockaddr*)&clientAddress, &clientAddressLen);
//         if (bytesReceived > 0) {
//             if (bytesReceived == sizeof(double){
//                 double receivedValue;
//             }
//             buffer[bytesReceived] = '\0';
//             // {
//             //     std::lock_guard<std::mutex> lock(valueMutex);
//             //     *sharedStringPtr = buffer;
//             // }
//             double receivedMessage[bytesReceived];

//             {
//                 std::lock_guard<std::mutex> lock(receiveMutex);
//                 receivedMail.push(*receivedMessage);
//                 // receivedMail.
//             }

//             // std::cout << "Received message: " << *sharedStringPtr << std::endl;
//         }

//         std::this_thread::sleep_for(std::chrono::milliseconds(interval));
//     }
// }

void UdpManager::udpListener() {
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
    serverAddress.sin_port = htons(port);

    if (bind(socketFd, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        throw std::runtime_error("Failed to bind socket");
    }

    char buffer[MAX_BUFFER_SIZE];

    while (!stopFlag.load()) {
        sockaddr_in clientAddress;
        socklen_t clientAddressLen = sizeof(clientAddress);

        // Clear buffer before each receive
        std::memset(buffer, 0, sizeof(double));

        // Receive data
        int bytesReceived =
            recvfrom(socketFd, &buffer, sizeof(double), 0, (sockaddr*)&clientAddress, &clientAddressLen);

        if (bytesReceived == -1) {
            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
            continue; // Skip this iteration and wait for the next cycle
        }

        if (bytesReceived > 0) {
            if (bytesReceived == sizeof(double)) { // Ensure we received exactly a double
                double receivedValue;

                // Copy the raw byte data into the double (unsafe but standard way)
                std::memcpy(&receivedValue, buffer, sizeof(double));
                // std::cout << "Value direct from recfrom:\t" << receivedValue << std::endl;
                
                {
                    std::lock_guard<std::mutex> lock(receiveMutex);
                    receivedMail.push(receivedValue); // Push the received double value to the queue
                }

            } else {
                std::cerr << "Received data size mismatch. Expected: " << sizeof(double)
                          << " bytes, but got: " << bytesReceived << " bytes." << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(interval));
    }
}

// void UdpManager::udpListener() {
//     // Implementation of udpListener function

//     char buffer[MAX_BUFFER_SIZE];

//     while (!stopFlag.load()) {
//         // Receive data from UDP socket
//         // Assume buffer contains the received message

//         std::string receivedMessage(buffer);

//         // Store received message in the queue
//         {
//             std::lock_guard<std::mutex> lock(receiveMutex);
//             receivedMail.push(receivedMessage);
//         }
//     }
// }