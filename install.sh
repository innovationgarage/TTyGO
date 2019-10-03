[ "$TTGOTERM" ] || TTGOTERM=/dev/ttyUSB0

if ! [ -e ~/bin/arduino-cli ]; then
  (
    cd ~
    curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
  )
fi

export PATH=~/bin:$PATH

arduino-cli core update-index --additional-urls http://arduino.esp8266.com/stable/package_esp8266com_index.json
arduino-cli core install esp8266:esp8266 --additional-urls http://arduino.esp8266.com/stable/package_esp8266com_index.json

arduino-cli lib install OneButton U8g2 ESP8266Scheduler

arduino-cli compile --fqbn esp8266:esp8266:nodemcu TTyGO -v
arduino-cli upload -p $TTGOTERM --fqbn esp8266:esp8266:nodemcu TTyGO
