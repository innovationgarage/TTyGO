# About

ElCheapoAIS-nmea is a relay for AIS messages, sending received AIS messages, including your own position, speed etc
to land using your regular (satellite) internet connection.

# Hardware installation

* Connect the A, B and GND screw headers to an NMEA 0183 output on your AIS or NMEA hub
* Connect the USB cable to a USB charger

# Firewall configuration

You need to open outgoing tcp traffic to the ports 1026 and 1024 to elcheapoais.innovationgarage.tech (93.188.234.96).

# Default configuration

* NMEA bus
  * 38400 baud, 8bit, no flow control, 1 stop bit (8N1).

* AIS downsampling
  * Lowest of a total of 100 messages / sec or 10 messages / sec / mmsi.

# First time configuration

1) Connect the USB to a PC (the USB normally used to power the device)

2) Wait until the serial port appears. In Windows you can see it in the Device Manager:
![image](images/device_manager.PNG)

3) Open a terminal program like PuTTY or Minicom and instruct it to connect to
the USB serial port the device presents as (/dev/ttyACM? under linux,
COM?: under windows). 

In PuTTY the configuration for the early screenshot would look like:
![image](images/putty.PNG)

You should be presented with a Linux command prompt, e.g. something like

    root@orangepizero:~# 

You can now use any of the commands below to configure the device.

# Network configuration

To connect to the WiFi called MYSSID with the password MYPASSWORD use
the nmcli command like this

    nmcli con add con-name SOMENAME ifname wlan0 type wifi ssid MYSSID wifi-sec.key-mgmt wpa-psk wifi-sec.psk MYPASSWORD
    nmcli con up SOMENAME

You can verify that you have acquired an ip number using the command

    ifconfig wlan0

# NMEA configuration

All the NMEA / AIS configuration resides in the file
/etc/elcheapoais/config. It can be inspected using

    cat /etc/elcheapoais/config

The format is just name="value". Note that values need to be quoted,
and no extra space is allowed outside of the quotes.

There is no simple way to edit the file, but you can replace it in its
entirety:

    cat > /etc/elcheapoais/config <<EOF
    Copy-paste in modified file contents here
    EOF
    
## Station ID autodetection

If the station ID is set to "unknown", the device will not start normal operation, but will instead listen for AIVDO messages
from the AIS, extracting the MMSI from the first one received, set that as station id and reboot.

This is the factory default setting for stationid.

# Applying new config

Apply new configuration by rebooting the device by issuing the command

    reboot now

# Readonly filesystem

To enable readonly root, run

    armbian-config

and select System -> Overlayroot. From then on, to make changes to the filesystem, first run

    overlayroot-chroot

# Notifier light patterns

The system will blink the two status LEDs on the board to indicdate successfull connection to the NMEA bus and to the internet / the GeoCloud server. Both LED:s being on means everything is correctly set up. Any flashing or LED:s being off means there is a problem connecting to any of the services. For more details on blinking patterns, see https://github.com/innovationgarage/ElCheapoAIS-nmea/blob/master/notifier.json
