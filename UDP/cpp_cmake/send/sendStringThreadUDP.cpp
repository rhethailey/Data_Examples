/**
 * @file sendStringUDP
 * @brief Quick Example to send string with udpManagerClass.h
 * @version 0.1
 */

// custom includes
#include "udpManager.h"
#include <atomic>

// function to update string for thread
// hypothetically this could be run w/o delay
void udpUpdateSend(UdpManager& udpManager,
    std::atomic<bool>& stopFlag,
    std::string myString,
    int delay) {
   
    std::string streamValue;

    int i = 0;
    while (!stopFlag.load()) {
        streamValue = myString + std::to_string(i);
        udpManager.updateValue(streamValue);

        std::cout << "Updated string:\t'" << streamValue << "'" << std::endl;    

        // sleep
        // although this thread is running at ~2Hz, the UDP manager sends at the defined frequency
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        i++;
    }
}


int main()
{
    // Define IP address and port // Hard coded shit
    const std::string ip = "127.0.0.1";     // for local testing
    int port = 12345;
    int freq = 1; // (Hz)

    std::string myString = "Hello, world\t";

    // *** UDP MANAGEMENT *** //
    UdpManager udpSend(ip, port, freq);
    // Define data type sent in the UDP manager
    udpSend.setDataType(DataType::STRING); // Lazy initialization || Update in future iteration
    // Start UDP
    udpSend.startUdpYap();                 // starts sending values at defined frequency
    
    // delay for update rate (ms)
    int delay = 500;
    // atomic stop flag to stop the updated thread
    std::atomic<bool> stopFlag(false);

    // threaded loop to update string iteratively
    std::thread sendThread(
        udpUpdateSend, std::ref(udpSend), std::ref(stopFlag), myString, delay);
    
    // wait and stop executable
    std::this_thread::sleep_for(std::chrono::seconds(10));
    stopFlag = true;

    // join threads
    if (sendThread.joinable()) { sendThread.join(); } 
    // Stop the UdpManager
    udpSend.stop();

    
    return 0; 
}