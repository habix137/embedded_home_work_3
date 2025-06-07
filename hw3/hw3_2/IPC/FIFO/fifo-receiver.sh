#!/usr/bin/env bash

# Configuration
FIFO_PATH="/tmp/myfifo"

# 1. Make sure the FIFO exists
if [[ ! -p "$FIFO_PATH" ]]; then
  echo "Error: FIFO $FIFO_PATH not found. Run the sender first to create it." >&2
  exit 1
fi

# 2. Continuously read from the FIFO and print immediately
echo "Listening on $FIFO_PATH … (Ctrl-C to quit)"
while IFS= read -r line < "$FIFO_PATH"; do
  # this will unblock the sender’s echo
  echo "Received: $line"
done
