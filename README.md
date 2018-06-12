# AirBox for Raspberry PI #
Detect Air quality ,humidity , temperature and calculation comfort level.

## Referance material ##

- [i2c kernel documentation](https://www.kernel.org/doc/Documentation/i2c/dev-interface)
- [Official SHT31 c++ library for Arduino](https://github.com/adafruit/Adafruit_SHT31)
- [SHT31 manufacturer data sheet](https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/Humidity_and_Temperature_Sensors/Sensirion_Humidity_and_Temperature_Sensors_SHT3x_Datasheet_digital.pdf)
- [KQM2801AI c library for Raspberry PI](https://github.com/oknow23/KQM2801AI)
- [KQM2801AI data sheet](http://img005.hc360.cn/k2/M0D/FB/D8/wKhQxVirxHKEUAAnAAAAAFukSmo241.pdf)
## Prerequisites ##
 
This sh131 implimentation depends on i2c dev lib

- sudo apt-get install libi2c-dev
 
Below is also a good one to have while debugging, but be careful i2cdump from the below cause the sht31 interface to become unstable for me and requires a hard-reset to recover correctly.

- sudo apt-get install i2c-tools

Download lcdi2c library
- wget http://telecnatron.com/articles/Utility-To-Control-1602-LCD-On-Raspberry-Pi-Via-A-PCF8574-I2C-Backpack-Module/lcdi2c

install pip and line-bot-sdk for Line bot
- wget https://bootstrap.pypa.io/get-pip.py
- python2 get-pip.py
- pip install line-bot-sdk

## Build ##
- make
will output AirBox execute file.

## Execute ##
put AirBox and lcdi2c in the same place.
- ./AirBox

## LCD result show case ##
-  ──────────────────
- ∣ 25°C	49% 10.6PP  ∣
- ∣ Comfo  ok Bad    ∣
-  ──────────────────

Explain:
- 25°C		← Temperature
- 49%		← Humidity
- 10.6PP	← PPM
- Comfo 	← Comfort level
- ok 		← Humidity level
- Bad		← Air quality
 
