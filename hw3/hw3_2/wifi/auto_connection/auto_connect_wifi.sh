#!/usr/bin/env bash
#
# Automatically connect wlan0 to a chosen Wi-Fi network if it is not
# already connected.  Logs everything to /var/log/auto-connect.log
#
SSID="habix_wifi"
PASS="1234554321ali"
IFACE="wlan0"
LOG="/var/log/auto-connect.log"

# Make sure Wi-Fi radio is enabled
nmcli radio wifi on

# Is wlan0 already associated with our SSID?
CURRENT=$(nmcli -t -f ACTIVE,SSID dev wifi list ifname "$IFACE" | \
          awk -F: '$1=="yes"{print $2}')

if [[ "$CURRENT" == "$SSID" ]]; then
    echo "$(date '+%F %T')  Already connected to $SSID" >> "$LOG"
    exit 0
fi

# Not connected – try to connect
echo "$(date '+%F %T')  Trying to connect to $SSID ..." >> "$LOG"
nmcli dev wifi connect "$SSID" password "$PASS" ifname "$IFACE" \
    >> "$LOG" 2>&1
