# AeonLabs Monitor Fresh Reinforced Concrete Harding/Strength (maturity)
This repo contain the Kicad project files for a smart PCB able to monitor Fresh Reinforced concrete Harding/Strength (maturity). 
The smart PCB is a 12bit smart DAQ and cand be downloaded on this [repository](https://github.com/aeonSolutions/openScienceResearch-Smart-DAQ-Device-able-to-Upload-Live-Experimental-Sensor-Data-to-a-Data-Repo). 


## Smart DAQ for LIVE Experimental Data
This repository holds the firmware C code compatible with Tensilica's Xtensa LX6/7 microprocessors. Files are located in the folder "[Firmware Code](https://github.com/aeonSolutions/openScienceResearch-Smart-DAQ-Device-able-to-Upload-Live-Experimental-Sensor-Data-to-a-Data-Repo/tree/main/Firmware%20Code)".

<br>

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
