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
#include "udpManager.h"

/// @brief UDP Manager
/// @param ip   - IP address
/// @param port - UDP port
/// @param freq - (int) expected frequency to read udp values 
UdpManager::UdpManager(const std::string &ip, int port, double freq)
    : ip(ip), port(port), freq(freq), stopFlag(false), sharedStringPtr(std::make_shared<std::string>("dummy")), sharedDoublePtr(std::make_shared<double>(2.0)) // Initialize sharedDoublePtr
{
    socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFd == -1)
    {
        throw std::runtime_error("Failed to create socket");
    }
}

UdpManager::~UdpManager()
{
    stop();
    if (udpYapThread.joinable())
        udpYapThread.join();
    if (udpListenerThread.joinable())
        udpListenerThread.join();
    close(socketFd);
}

/// @brief 
/// @param newDataType Available data types: STRING, DOUBLE
void UdpManager::setDataType(DataType newDataType)
{
    std::lock_guard<std::mutex> lock(valueMutex); // Protect against concurrent access
    dataType = newDataType;
}

void UdpManager::startUdpYap()
{
    stopFlag = false;
    udpYapThread = std::thread(&UdpManager::udpYap, this);
}

void UdpManager::startUdpListener()
{
    stopFlag = false;
    udpListenerThread = std::thread(&UdpManager::udpListener, this);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // initial delay
    std::cout << "*** Started listener ***\n";
}

void UdpManager::stop()
{
    stopFlag = true;
    if (udpYapThread.joinable())
        udpYapThread.join();
    if (udpListenerThread.joinable())
        udpListenerThread.join();
}

void UdpManager::updateValue(const std::string &newValue)
{
    std::lock_guard<std::mutex> lock(valueMutex);
    *sharedStringPtr = newValue;
}

void UdpManager::updateValue(const double &newValue)
{
    std::lock_guard<std::mutex> lock(valueMutex);
    *sharedDoublePtr = newValue;
};

std::atomic<bool> &UdpManager::getStopFlag()
{
    return stopFlag;
}

/// @brief UDP Talker Main Loop
void UdpManager::udpYap()
{
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
    serverAddress.sin_port = htons(port);

    auto startTime = std::chrono::high_resolution_clock::now();
    auto nextSendTime = startTime + std::chrono::nanoseconds(interval);
    double newValue = 1.0;

    while (!stopFlag.load())
    {
        auto now = std::chrono::high_resolution_clock::now();
        if (now >= nextSendTime)
        {
            nextSendTime = now + std::chrono::nanoseconds(interval);

            if (dataType == DataType::STRING)
            {
                std::string message;
                {
                    std::lock_guard<std::mutex> lock(valueMutex);
                    message = *sharedStringPtr; // Use the shared string value
                }
                sendto(
                    socketFd, message.c_str(), message.length(), 0, (sockaddr *)&serverAddress, sizeof(serverAddress));

                // Optional: Debugging
                // std::cout << "Sent string: " << message << std::endl;
            }
            else if (dataType == DataType::DOUBLE)
            {
                double value;
                {
                    std::lock_guard<std::mutex> lock(valueMutex);
                    value = *sharedDoublePtr; // Use the shared double value
                }
                sendto(socketFd, &value, sizeof(value), 0, (sockaddr *)&serverAddress, sizeof(serverAddress));
                // Optional: Debugging
                // std::cout << "Sent double: " << value << std::endl;
            }
            else
            {
                std::cout << "DataType Initialization Failed" << std::endl;
                break;
            }
        }
    }
}

/// @brief UDP Listener Value Grabber
MultiType UdpManager::getMail()
{
    std::lock_guard<std::mutex> lock(receiveMutex);
    {
        if (!receivedMail.empty())
        {
            // Peek at the front element
            MultiType message = receivedMail.front();
            MultiType oldMessage;
            // Check the type of the message against the current dataType
            if ((dataType == DataType::STRING && std::holds_alternative<std::string>(message)) ||
                (dataType == DataType::DOUBLE && std::holds_alternative<double>(message)))
            {
                // ZOH of last read message
                oldMessage = message;
                return message;     // Return the valid message
            }
            else
            {
                // ZOH of last read message
                return oldMessage;
                // std::cerr << "Data type mismatch. Expected type: "
                //           << (dataType == DataType::STRING ? "STRING" : "DOUBLE") << std::endl;
            }
        }
    }

    // Return an empty MultiType if there's no valid message
    return {};
}

/// @brief UDP Listener Main Loop
void UdpManager::udpListener()
{

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
    serverAddress.sin_port = htons(port);

    if (bind(socketFd, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        throw std::runtime_error("Failed to bind socket");
    }

    char buffer[MAX_BUFFER_SIZE];
    sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);

    while (!stopFlag.load())
    {
        // Clear buffer
        std::memset(buffer, 0, MAX_BUFFER_SIZE);

        // Receive data
        int bytesReceived = recvfrom(socketFd, buffer, MAX_BUFFER_SIZE, 0,
                                     (sockaddr *)&clientAddress, &clientAddressLen);

        if (bytesReceived > 0)
        {
            std::lock_guard<std::mutex> lock(receiveMutex); // Lock the queue

            if (dataType == DataType::STRING)
            {
                // Attempt to interpret the received data as a string
                std::string receivedValue(buffer, bytesReceived);
                receivedMail = {};                // clean queue for newest message
                receivedMail.push(receivedValue); // Add to the queue
                // std::cout << "Received STRING: " << receivedValue << std::endl;
            }
            else if (dataType == DataType::DOUBLE)
            {
                // Check if the received data can be interpreted as a double
                if (bytesReceived == sizeof(double))
                {
                    double receivedValue;
                    std::memcpy(&receivedValue, buffer, sizeof(double));
                    receivedMail = {};                // clean queue for newest message
                    receivedMail.push(receivedValue); // Add to the queue
                    // std::cout << "Received DOUBLE: " << receivedValue << std::endl;
                }
                else
                {
                    std::cerr << "Error: Received data does not match expected DOUBLE type." << std::endl;
                }
            }
            else

                std::cerr << "Error: Unknown data type specified." << std::endl;
        }
        else if (bytesReceived == -1)
        {
            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
        }
        else if (bytesReceived == 0)
        {
            std::cerr << "Error, 0 bytes: " << strerror(errno) << std::endl;
        }
    }

    // Pause briefly to avoid high CPU usage
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}