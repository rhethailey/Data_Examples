import socket
import time

# (IP, port)
# serverAddress = ("MyIp", 12345)

# for local testing
serverAddress = ("127.0.0.1", 12345)
StringSocket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

string = "Hello, world\t"
i = 0.0

# 
while(1):
    # assign string to send w/ changing variable
    stringNew = string + str(i)
    
    # encode string to address
    StringSocket.sendto(stringNew.encode(),serverAddress)
    
    # visual confirmation
    print(stringNew)
    
    time.sleep(0.5)
    i = i+1