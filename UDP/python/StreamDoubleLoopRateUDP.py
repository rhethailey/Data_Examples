import socket
import time
import datetime
import struct
import numpy as np


def maintain_loop_rate(target_rate_hz):
    """Maintain a consistent loop rate."""
    target_period = 1.0 / target_rate_hz  # Calculate the target period (seconds per loop)
    start_time = time.time()  # Record the start time of the current loop iteration
    
    def sleep_for_remaining_time():
        """Sleep for the remaining time to maintain the loop rate."""
        elapsed_time = time.time() - start_time  # Calculate elapsed time
        remaining_time = target_period - elapsed_time  # Calculate remaining time
        if remaining_time > 0:
            time.sleep(remaining_time)  # Sleep for the remaining time if needed
    
    return sleep_for_remaining_time

# Define a square wave function
def square_wave(t, frequency=1):
    return np.sign(np.sin(2 * np.pi * frequency * t))



# *** main ***
# (IP, port)
# serverAddress = ("MyIp", 12345)

# for local testing
serverAddress = ("127.0.0.1", 12345)
MySocket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

time_i = datetime.datetime.now()  # current timestamp

# Example usage:
target_rate = 200  # Target loop rate Hz
loop_rate_maintainer = maintain_loop_rate(target_rate)
while True:
    time_e = datetime.datetime.now() - time_i # elapsed time
    
    # square wave for personal use with a frequency of 2pi
    y = 0.4*square_wave(time_e.total_seconds(),1/(np.pi*2)) + 0.7
    
    # < little endian double
    packed_data = struct.pack('<d',y) ## cpp uses little endian
    
    # Send byte to address
    MySocket.sendto(packed_data,serverAddress) 
    print(f"Sent double: {y} as bytes: {packed_data}")
    
    # Ensure consistent loop rate
    loop_rate_maintainer()
        
    # Update the start time for the next loop iteration
    loop_rate_maintainer = maintain_loop_rate(target_rate)