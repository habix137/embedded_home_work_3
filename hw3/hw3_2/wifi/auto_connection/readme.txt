>> sudo nano /usr/local/bin/auto_connect_wifi.sh
>> sudo chmod +x /usr/local/bin/auto_connect_wifi.sh
>> crontab -e
>> nmcli device disconnect wlan0
>> sudo tail -f /var/log/auto-connect.log
