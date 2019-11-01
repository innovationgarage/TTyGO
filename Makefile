TTGOTERM:=/dev/ttyUSB0
LIBS:=OneButton U8g2
FQBN:=esp8266:esp8266:nodemcu
# FQBN:=arduino:avr:leonardo

FILE_FQBN:=$(subst :,.,$(FQBN))

ifeq ($(firstword $(subst :, ,$(FQBN))),esp8266)
	PKG_PATH:=~/.arduino15/packages/esp8266
	LIBS:=$(LIBS) ESP8266Scheduler
else
        PKG_PATH:=~/.arduino15/packages/arduino/hardware/$(word 2, $(subst :, ,$(FQBN)))
endif

LIBDIR=~/Arduino/libraries
LIBDIRS:=$(patsubst %,$(LIBDIR)/%,$(LIBS))

.PHONY: all
all: TTyGO/TTyGO.$(FILE_FQBN).bin TTyGO/TTyGO.$(FILE_FQBN).elf

.PHONY: printenv
printenv:
	echo TTGOTERM=$(TTGOTERM)
	echo LIBS=$(LIBS)
	echo FQBN=$(FQBN)
	echo FILE_FQBN=$(FILE_FQBN)
	echo PKG_PATH=$(PKG_PATH)
	echo LIBDIR=$(LIBDIR)
	echo LIBDIRS=$(LIBDIRS)

TTyGO/TTyGO.$(FILE_FQBN).bin TTyGO/TTyGO.$(FILE_FQBN).elf: TTyGO/*.ino TTyGO/*.h $(LIBDIRS) $(PKG_PATH)
	~/bin/arduino-cli compile --fqbn $(FQBN) TTyGO -v

~/Arduino/libraries/%:
	~/bin/arduino-cli lib install $(notdir $@)

~/.arduino15/packages/esp8266: ~/bin/arduino-cli
	~/bin/arduino-cli core update-index --additional-urls http://arduino.esp8266.com/stable/package_esp8266com_index.json
	~/bin/arduino-cli core install esp8266:esp8266 --additional-urls http://arduino.esp8266.com/stable/package_esp8266com_index.json

~/.arduino15/packages/arduino/hardware/%: ~/bin/arduino-cli
	~/bin/arduino-cli core update-index
	~/bin/arduino-cli core install arduino:$(notdir $@)

~/bin/arduino-cli:
	cd ~; curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

.PHONY: upload
upload: TTyGO/TTyGO.$(FILE_FQBN).bin TTyGO/TTyGO.$(FILE_FQBN).elf
	~/bin/arduino-cli upload -p $(TTGOTERM) --fqbn $(FQBN) TTyGO

.PHONY: clean
clean:
	-rm TTyGO/TTyGO.*.bin TTyGO/TTyGO.*.elf TTyGO/TTyGO.*.hex
