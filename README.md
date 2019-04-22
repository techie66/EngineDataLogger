# Engine Data Logger
This program is designed to work with custom hardware to act as data logger and central control ECU for a 1983 Honda CB1100F.
It is written to run on a raspberry pi zero W and communicate with arduinos and other sensors to control and record various aspects of the motorcycle's operation.

This was originally prototyped in Python and later converted to C++ for stability and speed.

# Dependencies
## Included:<br>
- I2C dev Lib - https://github.com/jrowberg/i2cdevlib  

## Not Included
- bluetooth.h - BLuetooth Headers
- bcm2835 library - http://www.airspayce.com/mikem/bcm2835/index.html  
- FlatBuffers - https://github.com/google/flatbuffers  
	- Google API for data serialization.  
	- Build header with  "flatc -c --gen-object-api cb1100f-app.fbs"  

# License
EngineDataLogger is licensed under the GNU GPLv2. Dependencies are licensed under their respective licenses. See LICENSE for full text of GPLv2.

