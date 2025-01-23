// custom includes
#include "udpManager.h"
#include <atomic>

// function to get value for thread
// hypothetically this could be run w/o delay
void udpUpdateGet(UdpManager& udpManager,
    std::atomic<bool>& stopFlag,
    int delay) {

    int i = 0;
    while(!stopFlag.load(),i<10)
    {
        MultiType mail = udpManager.getMail();

        // Check the type of mail and print it
        if (std::holds_alternative<std::string>(mail))
        {
            std::cout << i << " - Received STRING: "  << std::get<std::string>(mail) << std::endl;
        }
        else if (std::holds_alternative<double>(mail))
        {
            std::cout << i << " - Received DOUBLE: "  << std::get<double>(mail) << std::endl;
        }
        else
        {
            std::cout << i << " - No message received." << std::endl;
        }
        // although this thread is running at ~1Hz, the UDP manager sends at the defined frequency
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        i++;
    }
    std::cout << "**** finished threaded loop *****\n\n";
}

int main()
{
    // Define IP address and port
    const std::string ip = "127.0.0.1"; // for local testing
    int port = 12345;
    int freq = 2; // (Hz)

    // *** UDP MANAGEMENT *** //
    UdpManager udpGetMail(ip, port, freq);
    // Define data type sent in the UDP manager
    udpGetMail.setDataType(DataType::STRING); // Lazy initialization || Update in future iteration

    // Start UDP's
    udpGetMail.startUdpListener();

    // delay for update rate (ms)
    int delay = 500;
    // atomic stop flag to stop the updated thread
    std::atomic<bool> stopFlag(false);

    // threaded loop to update value iteratively
    std::thread getThread(
        udpUpdateGet, std::ref(udpGetMail), std::ref(stopFlag), delay);
    
    // wait and stop executable
    std::this_thread::sleep_for(std::chrono::seconds(10));
    stopFlag = true;

    // join threads
    if (getThread.joinable()) { getThread.join(); } 
    // stop UDP manager
    udpGetMail.stop();
    return 0;
}