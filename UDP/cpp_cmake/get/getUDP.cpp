// custom includes
#include "udpManager.h"
#include "typeinfo"

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
    
    for (int i = 0; i < 10; ++i) // Try retrieving messages 10 times
    {
        MultiType mail = udpGetMail.getMail();

        // Check the type of mail and print it
        if (std::holds_alternative<std::string>(mail))
        {
            std::cout << i << " - Received STRING: "  << std::get<std::string>(mail) << std::endl;
        }
        else if (std::holds_alternative<double>(mail))
        {
            std::cout << i << " - Received DOUBLE: "  << std::get<double>(mail) << std::endl;
        }
        else // if neither string nor double
        {
            std::cout << i << " - No message received." << std::endl;
        }
        
        // although this thread is running at ~1Hz, the UDP manager sends at the defined frequency
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    udpGetMail.stop();
    return 0;
}