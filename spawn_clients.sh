#!/bin/bash

# Ensure the correct number of arguments are provided
if [ "$#" -lt 3 ]; then
    echo "Usage: $0 <server-ip> <server-port> <file1> [<file2> ... <fileN>]"
    exit 1
fi

SERVER_IP=$1
SERVER_PORT=$2

# Shift the first two arguments to get the list of files
shift 2

# Use '&' to run client processes in the background to simulate simultaneous clients
for file in "$@"; do
    if [ ! -f "$file" ]; then
        echo "File '$file' not found!"
        continue
    fi
    ./sendFile "$file" "$SERVER_IP:$SERVER_PORT" &
done

# Wait for all background processes to complete
wait
