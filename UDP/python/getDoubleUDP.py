# ----- An UDP server in Python that receives temperature values from clients-----

import socket
import datetime
import struct


# lazy way to define now
def now():
    time = datetime.datetime.now()
    return time

# Define the IP address and the Port Number
# (IP, port)
# serverAddress = ("MyIp", 12345)

# for local testing
ip = "127.0.0.1"
port = 12345

listeningAddress = (ip, port)

# Create a datagram based server socket that uses IPv4 addressing scheme
datagramSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# try to listen
try:
    datagramSocket.bind(listeningAddress)
    print(f"Server is listening on {ip}:{port}")
except Exception as e:
    print(f"Failed to bind: {e}")
    exit(1)

# initialization of timestamps
time_i = datetime.datetime.now()  # for total time
now_old = now()                   # for loop time
ave_diff = 0

while(True):
    # read byte and unpack
    stringRead, location = datagramSocket.recvfrom(8); # bytes
    value = struct.unpack('d', stringRead[:8])[0]
    
    
    # pretty time stats
    time_d = (now() - time_i).total_seconds()
    diff = (now() - now_old).total_seconds()
    ave_diff = (diff*.1 + ave_diff*.9)/1
    now_old = now()
    
    # print    
    print(f"{time_d:.3f}s" + f"\taveDelta: {ave_diff:.4f}\t" + f"\tcurrDelta: {diff:.4f}\t" + f"Received: {value:f}")