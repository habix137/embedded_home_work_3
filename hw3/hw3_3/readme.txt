sudo systemctl daemon-reload
sudo systemctl enable --now webserver.service

# 3. Check status & logs
systemctl status webserver.service
journalctl -u webserver.service -f