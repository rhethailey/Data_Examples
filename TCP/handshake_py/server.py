import socket
import time

def wait_for_client():
    HOST = ''         # Listen on all interfaces (equivalent to INADDR_ANY)
    PORT = 12347
    BACKLOG = 1       # Max number of queued connections

    try:
        # Create socket
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

        # Bind to the port
        server_socket.bind((HOST, PORT))

        # Start listening
        server_socket.listen(BACKLOG)
        print("Process 1 ready. Waiting for Process 2 to connect...")

        # Accept a connection
        client_socket, client_address = server_socket.accept()
        print("Process 2 connected! Starting execution...")

        # Close the connection
        client_socket.close()
        server_socket.close()

    except socket.error as e:
        print(f"Socket error: {e}")
        exit(1)

def main():
    print("Testing...")
    wait_for_client()

    for i in range(10):
        print(f"Hello\t n:{i}")
        time.sleep(1)
        
if __name__ == "__main__":
    main()