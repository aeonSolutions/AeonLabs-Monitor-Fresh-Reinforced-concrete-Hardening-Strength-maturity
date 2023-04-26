[Main Catalogue](https://github.com/aeonSolutions/PCB-Prototyping-Catalogue) >> 
[Construction Industry](https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/tree/main/Construction%20Industry)  >>  [Construction Site](https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/tree/main/Construction%20Industry/Construction%20Site)   >>  Smart Concrete Curing Device

[![Telegram](https://img.shields.io/badge/join-telegram-blue.svg?style=for-the-badge)](https://t.me/+W4rVVa0_VLEzYmI0)
 [![WhatsApp](https://img.shields.io/badge/join-whatsapp-green.svg?style=for-the-badge)](https://chat.whatsapp.com/FkNC7u83kuy2QRA5sqjBVg) 
 [![Donate](https://img.shields.io/badge/donate-$-brown.svg?style=for-the-badge)](http://paypal.me/mtpsilva)
 [![Say Thanks](https://img.shields.io/badge/Say%20Thanks-!-yellow.svg?style=for-the-badge)](https://saythanks.io/to/mtpsilva)
![](https://img.shields.io/github/last-commit/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity?style=for-the-badge)
<a href="https://trackgit.com">
<img src="https://us-central1-trackgit-analytics.cloudfunctions.net/token/ping/lb25ecpd5t9it62w6rjl" alt="trackgit-views" />
</a>
![](https://views.whatilearened.today/views/github/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity.svg)



# Smart Concrete Curing Device

**The problem**

Currently, there are commercial solutions for this smart device, for instance, [Maturix](https://maturix.com) smart solution and also [Giatech](https://www.giatecscientific.com/products/concrete-sensors/smartrock-maturity-meter/)'s smart solution. Both Cloud-based.  And this is a limitation. 

**The Ideia**

This smart device is able to do active monitoring of fresh concrete at any construction site to determine accurately the concrete strength by the hour. This gives an advantage to a construction site team, as is able to accurately know the exact day the concrete structural element can have formworks removed and this saves time and money. The one available here on GitHub can be customized to function without the need for a cloud server, and instead provide the information on the smart device display only. It can also be configured to do measurements upload to a local server located inside the construction site. 

Alternativly, when is required  LIVE remote access to the concrete data, this smart device is capable of LIVE experimental data measurements with a **unique FingerPrint ID** configuted to do measurements and upload directly to an open data repository , such as a [dataverse](https://dataverse.org).

<br>

This repo contains instructions on how to assemble a smart PCB able to monitor Fresh Reinforced concrete Hardening to determine concrete maturity and estimate its strength. 
The smart PCB is a 12-bit smart DAQ. The Kicad project files can be downloaded on this [repository](https://github.com/aeonSolutions/openScienceResearch-Smart-DAQ-Device-able-to-Upload-Live-Experimental-Sensor-Data-to-a-Data-Repo). Those who prefer buying already assembled, ready to deploy and do live monitoring at a construction site can buy at my [Tindie store](https://www.tindie.com/stores/aeonlabs/).  

Nowadays there are many sensors that can be connected for this monitoring task. The most common for this kind of applications are the Sensorion SHt3x, and the Maxim Integrated DS18B20 (see part list below).

This smart device has the ATSHA204A chip a SHA-256 high-security hardware authentication IC for secure and unique data exchange and communication between the smart DAQ and the Edge server.

<p align="center">
<img src="https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/designs/smart%20concrete%20maturity%20inside%20transparent%20enclosure.jpeg" height="350">
</p>

The proposed assembly solution utilizes a LiPo 2000mAh battery and is able to monitor, in regular intraday intervals, and send data to a nearby edge server using WIFI or Bluetooth BLE for more than one month without requiring a recharge. Other smart device setup solutions are possible. For instance, the one with a LiPo battery is suitable for embedding the smart device inside the concrete element itself one wants to do monitoring. Is also possible to embed the sensor probe into the fresh concrete while the smart DAQ enclosure is placed at the surface of the concrete element. Depending on power availability the smart DAQ has a built-in micro USB plug able to connect to any USB smartphone or tablet charger.


<p align="center">
<img src="https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/designs/smart%20concrete%20maturity%20inside%20enclosure.jpeg" height="350">

<img src="https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/designs/smart_fresh_concrete_monitoring_device.jpeg" height="350">
</p>

<br>

<p align="center">
  <img src="https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/media/20230426_142427.jpg" width=250>
  <img src="https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/media/20230426_142450.jpg" width=250>
  <img src="https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/media/20230426_142535.jpg" width=250>
  <img src="https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/media/20230426_142549.jpg" width=250>
  <img src="https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/media/20230426_142602.jpg" width=250>
  <img src="https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/media/20230426_142615.jpg" width=250>
  <img src="https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/media/20230426_142633.jpg" width=250>
  <img src="https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/media/20230426_142645.jpg" width=250>
  <img src="https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/media/20230426_142658.jpg" width=250>
  <img src="https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/media/20230426_142711.jpg" width=250>
</p>

<br>

### ![](https://github.com/aeonSolutions/AeonLabs-WIFI-BLE-RFID-NFC-Desk-Contacless-Reader-Writer/blob/main/designs/working_green.png) Status
  Fully working.

[March 24, 2023] - The firmware 1.0.3 for Over the Air Updates is now released to the public. Goto the [releases](https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/releases) section to learn more.  

<br>

### Now selling on Tidnie

| [<img src="https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/blob/main/tindie_sell.png" alt="" height="60">](https://www.tindie.com/stores/aeonlabs/) | To all those not into electronics and still liked and want to own any of the listed PCB for smart devices can buy the PRO version on Tindie ready to install and use. |
|-------------|------|

<br />
<br />

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
  - reference voltage sensor calibration with temperature and humidity
- 	External connectivity:
  -  1x I2C pin terminal connector (shared)
  -  1x 12 to 16bit (oversampling & digitization) digital terminal connector (shared)


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

<p align="center">
<img src="https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/designs/pcb_front.png" width="350">

<img src="https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/blob/main/designs/pcb_back.png" width="350">

</p>

<br>
<br>

________________________________________________________________________________________________________________

## [Go back to main catalogue page](https://github.com/aeonSolutions/PCB-Prototyping-Catalogue)

________________________________________________________________________________________________________________

<br>

## Author

You can get in touch with me on my LinkedIn Profile:

#### Miguel Tomas

[![LinkedIn Link](https://img.shields.io/badge/Connect-Miguel--Tomas-blue.svg?logo=linkedin&longCache=true&style=social&label=Connect)](https://www.linkedin.com/in/migueltomas/)

You can also follow my GitHub Profile to stay updated about my latest projects: [![GitHub Follow](https://img.shields.io/badge/Connect-Miguel--Tomas-blue.svg?logo=Github&longCache=true&style=social&label=Follow)](https://github.com/aeonSolutions)

<br>

### Be supportive of my dedication and work towards technology education and buy me a cup of coffee
The PCB Desgin Files i provide here for anyone to use are free. If you like this Smart Device or use it, please consider buying me a cup of coffee, a slice of pizza or a book to help me study, eat and think new PCB design files.

<p align+"left">
<a href="https://www.buymeacoffee.com/migueltomas">
   <img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" data-canonical-src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" height="35" />
 </a>
<br>
<img src="https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/blob/main/Designs/bmc_qr.png" height="130">
</p>

<br />

### Make a donation on Paypal
Make a donation on paypal and get a TAX refund*.

[![](https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/blob/main/paypal_small.png)](http://paypal.me/mtpsilva)

<br>

### Support all these open hardware projects and become a patreon  
Liked any of my PCB KiCad Designs? Help and Support my open work to all by becomming a LDAD Patreon.
In return I will give a free PCB design in KiCad to all patreon supporters. To learn more go to patreon.com. Link below.

[![](https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/blob/main/patreon_small.png)](https://www.patreon.com/ldad)

<br />
<br />

______________________________________________________________________________________________________________________________
### License
2023 Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International

https://creativecommons.org/licenses/by-nc-sa/4.0/
