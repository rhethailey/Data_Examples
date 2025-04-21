#include <iostream>
#include <thread>
#include <chrono>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>

int wait_for_server(std::string sendip) {
    // pre connect shit
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(12347);

    // Convert IPv4 addresses from text to binary form
    if (inet_pton(AF_INET, sendip.c_str(), &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        return -1;
    }

    std::cout << "Waiting for Process 1...\n";

    // Attempt to connect to server
    while (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        usleep(100000); // Wait 100ms before retrying
    }

    std::cout << "Connected to Process 1, now starting execution.\n";

    // Cleanup
    close(sock);

    return 0;
}

int main()
{
   
    std::cout << "Waiting..." << std::endl;

    int foo;
    std::string ip = "0.0.0.0";
    foo = wait_for_server(ip);


    // *** run executable *** 
    for(int i = 0; i < 10; i++){
        std::cout << "hello client \tn:" << i << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return 0;
}