# LORA_WIFI_SCAN
SCANS WIFI NETWORK with an ESP32 and sends MAC of strongest WIFI via LORA to The Things Network TTN V3 Stack Community Edition.
On the backend a Google script is waiting to estimate the position of the node via the recieved MAC Adress. With this method you can do indoor location estimation

WARNINGS

Before you compile please ensure you modify all your credentials and used sensors in "credentials.h" which have been marked REPLACEMEUSER
USE a Proper POWER SOURCE.
This Source Code is provided "AS it is" . If you use this code you agree that any harm, damage, burn or injuries are YOUR RESPONSIBILITY! The Author may not be held liable for any damage that might happen if you try this code out.
Legal validity of this disclaimer This disclaimer is to be regarded as part of the internet publication which you were referred from. If sections or individual terms of this statement are not legal or correct, the content or validity of the other parts remain uninfluenced by this fact.

Referrals and links The author is not responsible for any contents linked or referred to from his pages - unless he has full knowledge of illegal contents and would be able to prevent the visitors of his site from viewing those pages. If any damage occurs by the use of information presented there, the author might not be liable. Furthermore the author is not liable for any postings or messages published by users of discussion boards, guest books or mailing lists provided.

Installation
1. Install Arduino IDE + ESP32 extension
2. Install MCCI LMIC as a Lib
3. Install a Google Script on Google Drive: For Installation of the Google script use the follwing information:
- https://github.com/Uspizig/Ttn-gooogle-script
- https://developers.google.com/maps/documentation/geolocation/get-api-key -> you will need a key and take care about billing. Hint: without credit card verification this won't work
- https://developers.google.com/maps/documentation/geolocation/overview - fill in your API-Key to the google scrpt file
- Consider Payment is needed if you make tooo many requests.

For usage with Arduino ESP32 V2.0 consider this link: .. otherwise it won't compile: https://github.com/mcci-catena/arduino-lmic/issues/714
 
 
Keep in Mind: This is only for DEMO. NEVER EVER use this in Production environment.


Also ensure you have installed the following libs: You might not need them for this sketch and you can comment out a lot.

Code Sources that influenced this Source Code and some libaries you may need:

https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_auth/mqtt_auth.ino
https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/
RGBW for SK6812:
Original code by Jim Bumgardner (http://krazydad.com).
Modified by David Madison (http://partsnotincluded.com).
Extended by Christoph Wempe
https://gist.github.com/CWempe?direction=desc&sort=created

Adafruit BME/BMP280/SGP30 sketches/libs
Please consider buying their products due to their great work in Arduino libs and Open Source(HW) Community
https://github.com/adafruit/Adafruit_SGP30
https://github.com/adafruit/Adafruit_BMP280_Library

Bosch BME680 BSEC Lib
https://github.com/BoschSensortec/BSEC-Arduino-library

Andreas Spiess
OTA Sketch for OTA Update of the ESP32 over WiFi
https://github.com/SensorsIot/ESP32-OTA



This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License. This means if you use or feature this work in any website, publication, presentation etc, you must clearly attribute/ credit the work to uspizig (but not in any way that suggests that uspizig endorses you or your use of the work). You may not use this work for commercial purposes without explicit permission. 
