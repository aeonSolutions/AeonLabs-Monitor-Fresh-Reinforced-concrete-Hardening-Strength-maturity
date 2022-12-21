[![Telegram](https://img.shields.io/badge/join-telegram-blue.svg?style=for-the-badge)](https://t.me/+W4rVVa0_VLEzYmI0)
 [![WhatsApp](https://img.shields.io/badge/join-whatsapp-green.svg?style=for-the-badge)](https://chat.whatsapp.com/FkNC7u83kuy2QRA5sqjBVg) 
 [![Donate](https://img.shields.io/badge/donate-$-brown.svg?style=for-the-badge)](http://paypal.me/mtpsilva)
 [![Say Thanks](https://img.shields.io/badge/Say%20Thanks-!-yellow.svg?style=for-the-badge)](https://saythanks.io/to/mtpsilva)
![](https://img.shields.io/github/last-commit/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity?style=for-the-badge)
<a href="https://trackgit.com">
<img src="https://us-central1-trackgit-analytics.cloudfunctions.net/token/ping/lb25ecpd5t9it62w6rjl" alt="trackgit-views" />
</a>
![](https://views.whatilearened.today/views/github/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity.svg)



# AeonLabs Monitor Fresh Reinforced Concrete Hardening/Strength (maturity)
This repo contains instructions on how to assemble a smart PCB able to monitor Fresh Reinforced concrete Hardening to determine concrete maturity and estimate its strenght. 
The smart PCB is a 12bit smart DAQ. The Kicad project files can be downloaded on this [repository](https://github.com/aeonSolutions/openScienceResearch-Smart-DAQ-Device-able-to-Upload-Live-Experimental-Sensor-Data-to-a-Data-Repo). Those who prefer buying already assembled, ready to deploy and do live monitoring at a construction site can buy at my [Tindie store](https://www.tindie.com/stores/aeonlabs/).  

Nowadays there are many sensors that can be connected for this monitoring task. The most common for this kind of applications are the Sensorion SHt3x , the Maxim Integrated DS18B20 (see part list below).

This smart device has the ATSHA204A chip a SHA-256 high-security hardware authentication IC for secure and unique data exchange and communication between the smart DAQ and the Edge server.

![](https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/designs/smart%20concrete%20maturity%20inside%20transparent%20enclosure.jpeg)


Proposed assembly solution utilizes a LiPo 2000mAh battery and is able to monitory, in regular intraday intervals, and send data to a nearby edge server using WIFI or Bluetooth BLE for more than one month without requiring a recharge. Other smart device setup solutions are possible. For instance the one with a LiPo battery is suitable for embedding the smart device inside the concrete element itself one wants to do monitoring. Is also possible to embed the sensor probe into the fresh concrete while the smart DAQ enclosure is placed at the surface of the concrete element. Depending on power availability the smart DAQ has a built in micro USB plug able to connect to any USB smartphone or tablet charger.

![](https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/designs/smart%20concrete%20maturity%20inside%20enclosure.jpeg)

<br>

### ![](https://github.com/aeonSolutions/AeonLabs-WIFI-BLE-RFID-NFC-Desk-Contacless-Reader-Writer/blob/main/designs/working_green.png) Status
  Fully working.

<br>

## How to assemble on smart concrete maturity device
Check the [wiki](https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/wiki) above for detailed instructions on:

- how to assemble one smart PCB concrete maturity device
- How to buy one on Tindie
- How to install the firmware code
- How to use it at any construction site. 
- & more...

<br />
<br>

### Smart PCB Hardware Specifications 
The hardware specifications for the 12bit pcb with dimensions of 54x48mm are the following:

- QFN 56 Dual Xtensa LX7 Core Processors running up to 240MHz
  -	RISC V ultra-low power co-processor
  - 512Kb RAM;
  - 8Mb SOIC 8 NOR SPI Flash Memory;
  - 2.4GHz ISM wireless connectivity;
  - 1x 12-16bit ADC Multiplexed DAQ channels;
- Authentication & Security:
  - SOIC-8 ATSHA204A SHA-256 high-security hardware authentication IC for secure and unique communication between devices;
- Power management
  - DFN-6 AUR9718 high efficiency step-down 3.3V 1.5A DC converter;
- Onboard sensors:
  - DFN-8 SHT3.x; temperature sensor with a precision of 1.5C;
  - DFN-8 SHT3.x humidity sensor;
  - LGA-14 LSM6DS3 a 6-axis accelerometer and gyroscope;
  - reference voltage sessor calibration wiht temperature and humidity
- 	External connectivity:
  -  1x I2C pin terminal connector (shared)
  -  1x 12 to 16bit (oversampling & digitization) digital terminal connector (shared)
  -  1x 12 to 16bit (oversampling & digitization) analog terminal connector (shared) with manual scale selection ohmeter via jumper pin
  -  12 to 16bit (oversampling & digitization) voltage reference analyser with temperature compensation
-  built in a 1.69" LCD TFT IPS display with ability to display LIVE experimental data being collected. 

<br>
<br>

 **In real life this smart DAQ is able to:**
- Can be powered using a 2 wire LiPo battery with a JST connector or by Micro USB 
- Connect to all kinds of I2C Temperature and Humidity sensors;
- Measure voltage in the range of  [0;3.3V]
- Measure electrical resistance [0; 10^6] Ohm 
- Do temperature and humidity compensation on all measurements 
- Has a voltage reference sensor for improved accuracy on ADC measurements  
- Has a motion sensor to know if anyone moved de smart device from its place

<br>
<br>

## PCB Gerber files
The PCB gerber files are located in the folder "gerber" and are ready to order online on any pcb fabrication store. (PCBWay, AllPCB, Eurocircuits, etc)
<br>
<br>
![](https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/designs/pcb_front.png)
![](https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/designs/pcb_back.png)

<br>
<br>

________________________________________________________________________________________________________________

## [Go back to main catalogue page](https://github.com/aeonSolutions/PCB-Prototyping-Catalogue)

________________________________________________________________________________________________________________

<br>
<br>

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
