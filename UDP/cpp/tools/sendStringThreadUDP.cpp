/**
 * @file sendStringUDP
 * @brief Quick Example to send string with udpManagerClass.h
 * @version 0.1
 */

// custom includes
#include "udpManagerClass.h"
#include <atomic>

void udpUpdateSend(UdpManager& udpManager,
    std::atomic<bool>& stopFlag,
    std::string myString,
    int delay) {
   
    std::string streamValue;

    int i = 0;
    while (!stopFlag.load()) {
        streamValue = myString + std::to_string(i);
        udpManager.updateValue(streamValue);

        std::cout << "Sent String:\t" << streamValue << std::endl;    

        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        i++
    }
}

std::string addCounter(std::string myString){

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
    
    int delay = 500;
    
    std::thread sendThread(
        udpUpdateSend, std::ref(udpSend), std::ref(stopFlag), myString, delay);

    int i = 0;
    while(true){
        // newString = myString + std::to_string(i);
        
        // update sent string
        udpSend.updateValue(newString);
        std::cout << newString << std::endl;    

        // sleep
        // although this thread is running at ~2Hz, the UDP manager sends at the defined frequency
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        i++;
    }

}