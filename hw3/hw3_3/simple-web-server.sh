#!/usr/bin/env bash
#
# A very simple HTTP server on port 80 that
# replies with the current date/time.  Each
# request is logged to stdout as well.

PORT=80

while true; do
  # Wait for one connection, then:
  { 
    # Send HTTP response headers + body
    echo -e "HTTP/1.1 200 OK\r"
    echo -e "Content-Type: text/plain\r"
    echo -e "\r"
    date
  } | nc -l -p "$PORT" -q 1

  # Log to the script’s own terminal (or to journal when run as a service)
  echo "[$(date '+%F %T')] → Replied on port $PORT"
done
