#!/bin/bash

# Display the PID of this script
echo "CPU Load Generator PID: $"
echo "To kill this process, run: kill $"
echo "Starting CPU load generation..."

# Function to generate CPU load using simple arithmetic
generate_load() {
    while true; do
        # Simple CPU-intensive loop without external dependencies
        sum=0
        for i in {1..100000}; do
            sum=$((sum + i * i))
        done
    done
}

# Create multiple background processes to utilize multiple cores
# For Orange Pi (usually 4 cores), create 4 processes
for i in {1..4}; do
    generate_load &
    echo "Started load generator process $i"
done

# Store background job PIDs
jobs -p > /tmp/cpu_load_pids

echo "CPU load processes started. Background PIDs:"
cat /tmp/cpu_load_pids
echo "To kill all: kill \$(cat /tmp/cpu_load_pids) or kill $"

# Keep the main script running
wait