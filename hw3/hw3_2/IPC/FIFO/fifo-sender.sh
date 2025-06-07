#!/usr/bin/env bash

# Configuration
FIFO_PATH="/tmp/myfifo"

# 1. Create the FIFO if it doesn’t already exist
if [[ ! -p "$FIFO_PATH" ]]; then
  echo "Creating FIFO at $FIFO_PATH"
  mkfifo "$FIFO_PATH"
fi

# 2. Read your terminal input line-by-line and send it into the FIFO
echo "Type lines of text; press Ctrl-D (EOF) when you’re done."
while IFS= read -r line; do
  # each echo will block until a reader reads from the FIFO
  echo "$line" > "$FIFO_PATH"
done
