/**
 * @file sendStringUDP
 * @brief Quick Example to send string with udpManagerClass.h
 * @version 0.1
 */

// custom includes
#include "udpManager.h"


int main()
{
    // Define IP address and port // Hard coded shit
    const std::string ip = "127.0.0.1";     // for local testing
    int port = 12345;
    int freq = 1; // (Hz)

    // *** UDP MANAGEMENT *** //
    UdpManager udpSend(ip, port, freq);
    // Define data type sent in the UDP manager
    udpSend.setDataType(DataType::DOUBLE); // Lazy initialization || Update in future iteration
    // Start UDP
    udpSend.startUdpYap();                 // starts sending values at defined frequency
    
    int delay = 500;
    double i, y, dt = 500;
    while(true){
        y = sin(i);
        
        // update sent string
        udpSend.updateValue(y);
        std::cout << "Sent Double:\t" << std::to_string(y) << std::endl;    

        // sleep
        // although this thread is running at ~2Hz, the UDP manager sends at the defined frequency
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        i = i+(dt/1000);
    }

    return 0;
}