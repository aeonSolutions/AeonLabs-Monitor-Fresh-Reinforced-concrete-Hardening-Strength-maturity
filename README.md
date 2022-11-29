# AeonLabs Monitor Fresh Reinforced Concrete Harding/Strength (maturity)
This repo contains intructions on how to assemble a smart PCB able to monitor Fresh Reinforced concrete Harding to determine concrete maturity and estimate its streght. 
The smart PCB is a 12bit smart DAQ. The Kicad project files can be downloaded on this [repository](https://github.com/aeonSolutions/openScienceResearch-Smart-DAQ-Device-able-to-Upload-Live-Experimental-Sensor-Data-to-a-Data-Repo). Those who prefer buying already assembled, ready to deploy and do live monitoring at a construction site can buy at my [Tindie store](https://www.tindie.com/stores/aeonlabs/).  

Proposed assembly solution utilizes a LiPo 2000mAh battery and is able to monitory, in regular intraday intervals, and send data to a nearby edge server for more than one month without requiring a recharge. Nowadays there are many sensors that can be connected for this monitoring task. The most common for this kind of applications are the Sensorion SHt3x , the Maxim Integrated DS18B20 (see part list below).

Other smart device setup solutions are possible. For instance the one with a LiPo battery is suitable for embbeding the smart device inside the concrete element itself one whant to do monitoring. Is also possible to embbed on the sensor proble and the smart DAQ encolsure placed at the surface of a concrete element. Depending on power availablity the smart DAQ has a built in mciro USB plug able to connect to any USB smartphone or tablet charger. 

<br>

## Parts needed to assemble one ready to deploy at any construction site

-  [Electronic Case Enclosure Waterproof Junction Box](https://s.click.aliexpress.com/e/_DCuKwB1)
-  [3.7V 2000mAh 103450 Lipo Polymer Lithium Rechargeable Battery](https://s.click.aliexpress.com/e/_DDm07rl)
-  [Micro SH 1.0 2Pin connector](https://s.click.aliexpress.com/e/_DmqVKcj)


*Two sensor options for monitoring:*
- Temperature and Humidity sensor proble
  - [SHT30 metallic proble](https://s.click.aliexpress.com/e/_DDNDKHd) [[Datasheet with sensor specs](https://sensirion.com/media/documents/EA647515/61641D0C/Sensirion_Humidity_Sensors_SHT3x_Datasheet_analog.pdf)]
  - [SHT30 platic proble](https://s.click.aliexpress.com/e/_Dn2w7Z1)   [[Datasheet with sensor specs](https://sensirion.com/media/documents/EA647515/61641D0C/Sensirion_Humidity_Sensors_SHT3x_Datasheet_analog.pdf)]
- Temperature sensor proble
  - [DS18b20](https://s.click.aliexpress.com/e/_DnnJJBd) [[Datasheet with sensor specs](https://datasheets.maximintegrated.com/en/ds/DS18B20.pdf)]

## Pinout 
To connect a sensor to the smart DAQ follow the convention wire colors:
- Red for power (max 5.0V DC)  (PWR pin)
- Black for Ground             (GND pin)
- Yellow for Data Clock        (SDA pin)
- Whilte for Data Measurements (SCL pin)

## Firmware Code
Anyone can program a custom firmware code to run on this smart DAQ device able to do fresh concrete LIVE monitoring. 

I have available here on Github a firmware C code compatible with Tensilica's Xtensa LX6/7 microprocessors. Files are located in the folder "[Firmware Code](https://github.com/aeonSolutions/openScienceResearch-Smart-DAQ-Device-able-to-Upload-Live-Experimental-Sensor-Data-to-a-Data-Repo/tree/main/Firmware%20Code)". This firmware code is ready to do concrete maturity sensor measurements and upload measured sensor data to a repository on a Dataverse. For more info see [www.dataverse.org](http://www.dataverse.org)

<br>

## Firmware Download / Burner
To program the smart DAQ device for concrete monitoring is necessary a Firmware downloader device able to download into it a binary program. Anyone can use the PCB available on this repository [AeonLabs-MCU-Burner-USB-to-UART-TTL](https://github.com/aeonSolutions/AeonLabs-MCU-Burner-USB-to-UART-TTL) to fabricate a USB to UART MCU program downloader. Compatible with Arduino Studio, VS Code and Visual Studio programming IDEs (among many others). Those who prefer buying already assembled, ready to deploy and do live monitoring at a construction site can buy at my [Tindie store](https://www.tindie.com/stores/aeonlabs/).  

![](https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/blob/main/progIDE.png)

<br />

### Smart PCB Hardware Specifications 
The hardware specifications for the 12bit pcb with dimensions of 23.5x43.5mm are the following:

- QFN 56 Dual Xtensa LX7 Core Processors running up to 240MHz
  -	RISC V ultra-low power co-processor
  - 512Kb RAM (PSRAM max 1 Gb);
  - 16Mb SOIC 8 NOR SPI Flash Memory (max 1Gb);
  - 2.4GHz ISM wireless connectivity;
  - Up to 118 12bit ADC Multiplexed DAQ channels;
- Authentication & Security:
  - SOIC-8 ATSHA204A SHA-256 high-security hardware authentication IC for secure and unique communication between devices;
- Power management
  - DFN-6 AUR9718 high efficiency step-down 3.3V 1.5A DC converter;
- Onboard sensors:
  - DFN-8 SHT3.x; temperature sensor with a precision of 1.5C;
  - DFN-8 SHT3.x humidity sensor;
  - LGA-14 LSM6DS3 a 6-axis accelerometer and gyroscope;
  - reference voltage sessor calibration wiht temperature and humidity
- 	External connectivity for up to 118 sensors:
  -  1x I2C pin terminal connector (shared)
  -  1x 12 to 16bit (oversampling & digitization) digital terminal connector (shared)
  -  1x 12 to 16bit (oversampling & digitization) analog terminal connector (shared) with manual scale selection ohmeter via jumper pin
  -  12 to 16bit (oversampling & digitization) voltage reference analyser with temperature compensation 

<br>

**Smart PCB design wiht a built in 1.69" LCD TFT IPS display**

Another version of this smart DAQ will be available next December that includes built in a 1.69" LCD TFT IPS display with ability to display LIVE experimental data being collected.. Stay tuned for updates. 

<br>

 **In real life this smart DAQ is able to:**
- connect to all kinds of 3V to 5V Digital sensors;
- connect to all kinds of sensors compatible with the I2C protocol. (max 118 sensors simultaneously)
- measure voltage in the range of  [0;3.3V]
- measure electrical resistance [0; 10^6] Ohm 
- do temperature and humidity compensation on all measurements 
- has a voltage reference sensor for improved accuracy on ADC measurements  
- has a motion sensor to know if anyone moved a specimen during an experiment

<br>
<br>

## PCB circuit Schematic 
The PCB circuit schematic is available in PDF located in the folder "PCB Schematic"

https://github.com/aeonSolutions/openScienceResearch-Smart-DAQ-Device-able-to-Upload-Live-Experimental-Sensor-Data-to-a-Data-Repo/tree/main/PCB%20Schematic

## PCB design files
The PCB gerber files are located in the folder "PCB Gerber Files" and are ready to order online on any pcb fabrication store. (PCBWay, AllPCB, Eurocircuits, etc)
<br>
<br>
other open hardware PCB electronics are also available on my PCB catalogue that maybe worth have a look.  The catalogue includes the KiCad project files of similar smart DAQ compatible with the firmware code available on this repository:

https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/blob/main/Smart%20DAQ/README.md

![](https://github.com/aeonSolutions/openScienceResearch-Smart-DAQ-Device-able-to-Upload-Live-Experimental-Sensor-Data-to-a-Data-Repo/blob/main/LDAD%20fron.png)
![](https://github.com/aeonSolutions/openScienceResearch-Smart-DAQ-Device-able-to-Upload-Live-Experimental-Sensor-Data-to-a-Data-Repo/blob/main/LDAD%20back.png)

<br>
<br>

______________________________________________________________________________________________________________________________
### License
2022 Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International

https://creativecommons.org/licenses/by-nc-sa/4.0/

______________________________________________________________________________________________________________________________

<br />
<br />

### Be supportive of my dedication and work towards technology education and buy me a cup of coffee
Buy me a cup of coffee, a slice of pizza or a book to help me study, eat and think new PCB design files.

[<img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" data-canonical-src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" height="70" />](https://www.buymeacoffee.com/migueltomas)

<br />
<br />

### Make a donation on Paypal
Make a donation on paypal and get a TAX refund*.

[![](https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/blob/main/paypal_small.png)](http://paypal.me/mtpsilva)


### Support all these open hardware projects and become a patreon  
Liked any of my PCB KiCad Designs? Help and Support my open work to all by becomming a LDAD Patreon.
In return I will give a free PCB design in KiCad to all patreon supporters. To learn more go to patreon.com. Link below.

[![](https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/blob/main/patreon_small.png)](https://www.patreon.com/ldad)
