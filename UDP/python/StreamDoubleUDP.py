import socket
import time
import struct
import math

# (IP, port)
# serverAddress = ("MyIp", 12345)

# for local testing
serverAddress = ("127.0.0.1", 12345)
MySocket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

i = 0.0
delay = 0.5

while(1):
    # for changing double
    y = math.sin(i)
    
    # < little endian double
    byte_representation = struct.pack('<d', y)
    
    # Send byte to address
    MySocket.sendto(byte_representation,serverAddress)
    print(f"Sent double: {y} as bytes: {byte_representation}")
    
    # sleep
    time.sleep(delay)
    i = i + delay