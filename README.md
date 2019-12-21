# serial_bridge
Point-to-point UART through WiFi on esp8266.
Ready to use in PlatformIO with Arduino framework.

# Setting
  If setting not found or pin D0(GPIO16) pulled up to VCC been start "setting mode"
  esp start AP with name SSID password 12345678.

  Page with setting http://192.168.4.1/.
  
  AP - if checked, up Fi-Wi AP and create tcp-server(192.168.4.1:2000), else connect to WiFI and to tcp-server.

  for AP mode:
    host, port - not use.

  for STA mode:
    host, port - for tcp client by default use 192.168.4.1:2000.
    
  
