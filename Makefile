TTGOTERM=/dev/ttyUSB0
LIBDIR=~/Arduino/libraries
LIBS=OneButton U8g2 ESP8266Scheduler
LIBDIRS:=$(patsubst %,$(LIBDIR)/%,$(LIBS))

.PHONY: all
all: TTyGO/TTyGO.esp8266.esp8266.nodemcu.bin TTyGO/TTyGO.esp8266.esp8266.nodemcu.elf

TTyGO/TTyGO.esp8266.esp8266.nodemcu.bin TTyGO/TTyGO.esp8266.esp8266.nodemcu.elf: TTyGO/*.ino TTyGO/*.h $(LIBDIRS)
	~/bin/arduino-cli compile --fqbn esp8266:esp8266:nodemcu TTyGO -v

~/Arduino/libraries/%: ~/.arduino15/packages/esp8266
	~/bin/arduino-cli lib install $(notdir $@)

~/.arduino15/packages/esp8266: ~/bin/arduino-cli
	~/bin/arduino-cli core update-index --additional-urls http://arduino.esp8266.com/stable/package_esp8266com_index.json
	~/bin/arduino-cli core install esp8266:esp8266 --additional-urls http://arduino.esp8266.com/stable/package_esp8266com_index.json

~/bin/arduino-cli:
	cd ~; curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

.PHONY: upload
upload:
	~/bin/arduino-cli upload -p $(TTGOTERM) --fqbn esp8266:esp8266:nodemcu TTyGO
