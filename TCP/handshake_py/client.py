import socket
import time
from pynput.mouse import Button, Controller

# client waiting for server

def wait_for_server(sendip: str, port: int = 12347) -> int:
    # dummy port
    # PORT = 12347

    # Create socket
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    except socket.error as e:
        print(f"Socket creation error: {e}")
        return -1

    server_address = (sendip, port)
    print("Waiting for Process 1...")

    # Attempt to connect with retries
    while True:
        try:
            sock.connect(server_address)
            break
        except socket.error:
            time.sleep(0.1)  # Sleep for 100ms before retrying

    print("Connected to Process 1, now starting execution.")

    # Cleanup
    sock.close()
    return 0

def main():
    print("Testing...")
    wait_for_server("0.0.0.0",12345)

    # define mouse as controller
    # mouse = Controller()

    for i in range(10):
        print(f"Hello\t n:{i}")
        # mouse.click(Button.left) 
        time.sleep(1)

if __name__ == "__main__":
    main()