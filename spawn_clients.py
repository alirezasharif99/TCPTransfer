import sys
import subprocess

if len(sys.argv) < 4:
    print("Usage: python spawn_clients.py <server-ip> <server-port> <file1> [<file2> ... <fileN>]")
    sys.exit(1)

server_ip = sys.argv[1]
server_port = sys.argv[2]
files = sys.argv[3:]

processes = []
for file in files:
    try:
        with open(file) as f:
            # Start a subprocess to run the client executable for each file
            proc = subprocess.Popen(["./sendFile", file, f"{server_ip}:{server_port}"])
            processes.append(proc)
    except FileNotFoundError:
        print(f"File '{file}' not found!")

# Wait for all subprocesses to complete
for proc in processes:
    proc.wait()

print("All clients have finished sending files.")
