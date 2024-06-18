#!/bin/bash

# Variables
serverIP="131.104.48.71:50547"
fileName="wonderland.txt"
total=0
max=0
min=999999  # A large number as initial value

# Print header
echo -e "Trial\tTime (ms)"

# Run the client 20 times
for i in {1..20}
do
    # Measure the time it takes to run the command
    start=$(date +%s.%N)
    ./sendFile "$fileName" "$serverIP"
    end=$(date +%s.%N)
    
    # Calculate the duration using awk and convert to milliseconds
    duration=$(awk -v start=$start -v end=$end 'BEGIN {print (end - start) * 1000}')
    
    # Print the duration for this trial
    printf "%-5s\t%s\n" "$i" "$duration"
    
    # Add to total time
    total=$(awk -v total=$total -v duration=$duration 'BEGIN {print total + duration}')
    
    # Check if this duration is a max or min
    max=$(awk -v max=$max -v duration=$duration 'BEGIN {print (duration > max) ? duration : max}')
    min=$(awk -v min=$min -v duration=$duration 'BEGIN {print (duration < min) ? duration : min}')
done

# Calculate average time using awk
average=$(awk -v total=$total 'BEGIN {print total / 20}')

# Print results
echo -e "\nAverage Time: $average ms"
echo "Minimum Time: $min ms"
echo "Maximum Time: $max ms"
