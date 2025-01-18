// custom includes
#include "logManager.h"
#include "stringManager.h"
// #include "udpManager.h"
#include "udpManagerClass.h"

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

#include <chrono>
#include <iostream>
#include <thread>

// UDP globals
#define MAX_BUFFER_SIZE 1024

void valueMail(UdpManager& udpManager, std::atomic<bool>& stopFlag) {
    std::vector<std::string> myVec{};

    auto start = std::chrono::high_resolution_clock::now();
    while (!stopFlag.load()) {
        // Check for received messages

        // this is lazy as shit?
        // std::vector<std::string> myVec{};
        std::vector<float> myVec{};
        // std::array<double,2> myVec{};
        std::string receivedMessage = udpManager.getMail();
        std::stringstream ss(receivedMessage);

        if (!receivedMessage.empty()) {
            while (ss.good()) {
                std::string substr;
                getline(ss, substr, ',');
                // myVec.
                myVec.push_back(std::stof(substr));
            }
            // myVec[0] = myVec[0] + rand() % 1;
            std::cout << "Received message: \t";
            for (size_t i = 0; i < myVec.size(); i++) {
                std::cout << myVec[i] << "\t";
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> array_duration = end - start;
            std::cout << "\t dt: " << array_duration.count() << "\n";
            start = std::chrono::high_resolution_clock::now();
            // std::cout << "\n";/

            // Process the received message
            // std::cout << "Received message: " << receivedMessage << std::endl;
        }
    }
}

// #include <array>
// #include <vector>
// #include <chrono>
// #include <iostream>

// int main() {
//     const int iterations = 1;
//     std::array<int, 28> arr;
//     std::vector<int> vec(28);

//     // Initialize arrays
//     for (int i = 0; i < 28; ++i) {
//         arr[i] = i;
//         vec[i] = i;
//     }

//     // Benchmark std::array
//     auto start = std::chrono::high_resolution_clock::now();
//     for (int iter = 0; iter < iterations; ++iter) {
//         for (int i = 0; i < 28; i++) {
//             arr[i] = rand() % 10;
//         }
//     }
//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> array_duration = end - start;
//     std::cout << "std::array time: " << array_duration.count() << " seconds\n";

//     // Benchmark std::vector
//     start = std::chrono::high_resolution_clock::now();
//     for (int iter = 0; iter < iterations; ++iter) {
//         for (int i = 0; i < 28; i++) {
//             vec[i] = rand() % 10;
//         }
//     }
//     end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> vector_duration = end - start;
//     std::cout << "std::vector time: " << vector_duration.count() << " seconds\n";

//     return 0;
// }

// int main() {
//     if (__cplusplus == 202101L) std::cout << "C++23";
//     else if (__cplusplus == 202002L) std::cout << "C++20";
//     else if (__cplusplus == 201703L) std::cout << "C++17";
//     else if (__cplusplus == 201402L) std::cout << "C++14";
//     else if (__cplusplus == 201103L) std::cout << "C++11";
//     else if (__cplusplus == 199711L) std::cout << "C++98";
//     else std::cout << "pre-standard C++." << __cplusplus;
//     std::cout << "\n";
// }

int main() {
    // Define IP address and port
    const std::string ip = "127.0.0.1";
    int port = 12346;

    // is there any penalto to this being slower than the send?
    // boost.asio if this ever comes into play
    int interval = 1; // (ms)

    // UdpManager udpSend(ip, 12346, interval);
    UdpManager udpGetMail(ip, port, interval);

    // Start UDP's
    // udpSend.startUdpYap();
    udpGetMail.startUdpListener();

    // Atomic flag to stop the value updater thread
    std::atomic<bool> stopFlag(false);

    // This Thread prints the updated value
    std::thread getThread(valueMail, std::ref(udpGetMail), std::ref(stopFlag));

    // Simulate main thread doing something else for 10 seconds
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Stop the UdpManager and the value updater thread
    stopFlag = true;
    if (getThread.joinable()) { getThread.join(); }

    // udpSend.stop();

    return 0;
}