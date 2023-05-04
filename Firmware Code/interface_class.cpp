/*
 Copyright (c) 2023 Miguel Tomas, http://www.aeonlabs.science

License Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
You are free to:
   Share — copy and redistribute the material in any medium or format
   Adapt — remix, transform, and build upon the material

The licensor cannot revoke these freedoms as long as you follow the license terms. Under the following terms:
Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. 
You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.

NonCommercial — You may not use the material for commercial purposes.

ShareAlike — If you remix, transform, or build upon the material, you must distribute your contributions under
 the same license as the original.

No additional restrictions — You may not apply legal terms or technological measures that legally restrict others
 from doing anything the license permits.

Notices:
You do not have to comply with the license for elements of the material in the public domain or where your use
 is permitted by an applicable exception or limitation.
No warranties are given. The license may not give you all of the permissions necessary for your intended use. 
For example, other rights such as publicity, privacy, or moral rights may limit how you use the material.


Before proceeding to download any of AeonLabs software solutions for open-source development
 and/or PCB hardware electronics development make sure you are choosing the right license for your project. See 
https://github.com/aeonSolutions/PCB-Prototyping-Catalogue/wiki/AeonLabs-Solutions-for-Open-Hardware-&-Source-Development
 for Open Hardware & Source Development for more information.

*/

#include "interface_class.h"
#include "Arduino.h"
#include "m_file_functions.h"
#include "mserial.h"
#include <WiFiMulti.h>

INTERFACE_CLASS::INTERFACE_CLASS(){
  this->firmware_version="-.-.-";
}

void INTERFACE_CLASS::init( mSerial* mserial, bool DEBUG_ENABLE) {  
  this->mserial=mserial;
  this->mserial->DEBUG_EN=DEBUG_ENABLE;
  this->onBoardLED = new ONBOARD_LED_CLASS();
  this->settings_defaults();

  // I2C IOs  __________________________
  this->I2C_SDA_IO_PIN = 8;
  this->I2C_SCL_IO_PIN = 9;
  // Power Saving ____________________________________
  this->LIGHT_SLEEP_EN=false;
  // External Ports IO Pin assignment ________________________________
  this-> EXT_PLUG_PWR_EN = 2;
  // ________________ Onborad LED  _____________
  this->onBoardLED->LED_RED = 11;
  this->onBoardLED->LED_BLUE = 13;
  this->onBoardLED->LED_GREEN = 10;

  this->onBoardLED->LED_RED_CH = 0;
  this->onBoardLED->LED_BLUE_CH = 2;
  this->onBoardLED->LED_GREEN_CH = 9;
  // ___________ MCU freq ____________________
  this-> SAMPLING_FREQUENCY = 240; 
  this-> WIFI_FREQUENCY = 80; // min WIFI MCU Freq is 80-240
  this->MIN_MCU_FREQUENCY = 10;
  this-> SERIAL_DEFAULT_SPEED = 115200;


  this->mserial->printStrln ("Firmware Build: " + String(__DATE__ ) + " " + String(__TIME__) + " version: " + this->firmware_version + "\n");
  //this->rtc.setTime(1609459200);  // 1st Jan 2021 00:00:00
  this->rtc.offset = this->config.gmtOffset_sec; // change offset value
  this->rtc.setTimeStruct( CompileDateTime(String(__DATE__ ).c_str(), String(__TIME__).c_str() ) ); 
  configTime(this->config.gmtOffset_sec, this->config.daylightOffset_sec, this->config.ntpServer.c_str());
}

// ***********************************************************************
void INTERFACE_CLASS::init_BLE(BLECharacteristic *pCharacteristicTX){
  this->pCharacteristicTX=pCharacteristicTX;
  this->mserial->pCharacteristicTX=pCharacteristicTX;
}

// **************************************************
void INTERFACE_CLASS::settings_defaults(){
  this->$espunixtime = time(0);
  this->$espunixtimePrev = time(0);
  this->$espunixtimeStartMeasure= time(0);

  this->WIFI_FREQUENCY=80 ; // min WIFI MCU Freq is 80-240
  this->MIN_MCU_FREQUENCY=10;
  
  this->SERIAL_DEFAULT_SPEED=115200;    

  this->DATA_VALIDATION_KEY = "A9CD7F1B6688159B54BBE862F638FF9D29E0FA5F87C69D27BFCD007814BA69C9";

  this->config.firmwareUpdate="auto";
  this->config.isBatteryPowered=false;
  this->config.isWIFIenabled=false;
   
  this->Measurments_EN=false;
  this->Measurments_NEW=false;
  this->config.SENSOR_DATA_FILENAME="concrete_curing.csv";
  this->config.DEVICE_PASSWORD="";

  this->LIGHT_SLEEP_EN=false;
  this->BLE_IS_DEVICE_CONNECTED=false;
  this->config.DEVICE_BLE_NAME="Slab 12A";
  this->forceFirmwareUpdate=false;

  this->clear_wifi_networks();
  
   this->mserial->printStrln("settings defaults loaded.");
}
// ****************************************************
void INTERFACE_CLASS::setBLEconnectivityStatus(bool status){
  this->BLE_IS_DEVICE_CONNECTED = status;
  this->mserial->BLE_IS_DEVICE_CONNECTED= status;
  if (status == false)
    this->$espunixtimeDeviceDisconnected = millis();
}
// ........................................
bool INTERFACE_CLASS::getBLEconnectivityStatus(){
  return this->BLE_IS_DEVICE_CONNECTED;
}

// ****************************************************
void INTERFACE_CLASS::clear_wifi_networks(){
  for(int i=0; i<5 ; i++){
    this->config.ssid[i] = "";
    this->config.password[i] = "";
  }
  this->number_WIFI_networks=0;
}

// ****************************************************
void INTERFACE_CLASS::add_wifi_network(String  ssid, String password){

    this->config.ssid[this->number_WIFI_networks]=ssid;
    this->config.password[ this->number_WIFI_networks]= password;
    this->number_WIFI_networks++;
    if( this->number_WIFI_networks>4)
      this->number_WIFI_networks=0;
}

// ***************************************************
int INTERFACE_CLASS::getNumberWIFIconfigured(){  
  return  this->number_WIFI_networks;
}

void INTERFACE_CLASS::setNumberWIFIconfigured(uint8_t num){  
  this->number_WIFI_networks= num;
}
// ****************************************************

void INTERFACE_CLASS::init_NTP_Time(char* ntpServer_, long gmtOffset_sec_, int daylightOffset_sec_, long NTP_request_interval_){
  this->config.ntpServer = String(ntpServer_);
  this->config.gmtOffset_sec = gmtOffset_sec_; // 3600 for 1h difference
  this->config.daylightOffset_sec = daylightOffset_sec_;
  this->config.NTP_request_interval=NTP_request_interval_;// 64 sec.
  this->NTP_last_request=0;
  this->rtc =  ESP32Time(this->config.gmtOffset_sec);
  configTime(this->config.gmtOffset_sec, this->config.daylightOffset_sec, this->config.ntpServer.c_str());

  this->mserial->printStrln("set RTC clock to firmware Date & Time ...");  
  rtc.setTimeStruct(CompileDateTime(__DATE__, __TIME__)); 

  this->mserial->printStrln(rtc.getDateTime(true));

}

// ***************************************************************************
void INTERFACE_CLASS::sendBLEstring(String message,  uint8_t sendTo ){
  if (sendTo == mSerial::DEBUG_NONE )
    return;
  
  if( message == "")
      message= "empty message found.\n";


  if ( ( sendTo == mSerial::DEBUG_TO_BLE || sendTo == mSerial::DEBUG_BOTH_USB_UART_BLE || sendTo == mSerial::DEBUG_TO_BLE_UART) && this->BLE_IS_DEVICE_CONNECTED==true ){
    if (WiFi.status() == WL_CONNECTED){
      WiFi.disconnect(true);
      delay(300);
    }
    this->mserial->sendBLEstring(message, sendTo);
  }

  if ( sendTo != mSerial::DEBUG_NONE && sendTo != mSerial::DEBUG_TO_BLE ){
    this->mserial->printStr(message, sendTo);
  }
}

// --------------------------------------------------------------------------

bool INTERFACE_CLASS::saveSettings(fs::FS &fs){
  this->config.DEBUG_EN = this->mserial->DEBUG_EN; // ON / OFF
  this->config.DEBUG_TO = this->mserial->DEBUG_TO; // UART, BLE   
  this->config.DEBUG_TYPE = this->mserial->DEBUG_TYPE; // Verbose // Errors 
  this->config.DEBUG_SEND_REPOSITORY = this->mserial->DEBUG_SEND_REPOSITORY ;
  
    this->mserial->printStrln("Saving the Smart Device settings...");

  if (fs.exists("/settings.cfg") )
    fs.remove("/settings.cfg");

  File settingsFile = fs.open("/settings.cfg", FILE_WRITE); 
  if ( !settingsFile ){
    Serial.println("error creating settings file.");
    return false;
  }
  Serial.println( String("Start Position =") + String(settingsFile.position() ) );

// auto;  0;0;   0;TheScientist;angelaalmeidasantossilva; ;; ;; ;; ;;    ntp server missing; 1725911728;-572489782;-2033959301;   1;1;1;   1; 11;100;0;    concrete_curing.csv; device pw; ble name;

  settingsFile.print( this->config.firmwareUpdate + String(';'));

  settingsFile.print( String(this->config.isBatteryPowered) + String(';'));

  settingsFile.print( String(this->config.POWER_PLUG_ALWAYS_ON) + String(';'));

  settingsFile.print( String(this->config.isWIFIenabled) + String(';') );

  String ssid="";
  String pwd="";
  for(int i=0; i<5 ; i++){
    settingsFile.print( this->config.ssid[i] + String(';'));
    settingsFile.print( this->config.password[i] + String(';'));
  }

  settingsFile.print( this->config.ntpServer + String(';'));

  settingsFile.print( String(this->config.gmtOffset_sec) + String(';'));

  settingsFile.print( String(this->config.daylightOffset_sec) + String(';'));

  settingsFile.print( String(this->config.NTP_request_interval) + String(';'));

  settingsFile.print( String(this->config.onboard_motion_sensor_en) + String(';'));

  settingsFile.print( String(this->config.onboard_temperature_en) + String(';'));

  settingsFile.print( String(this->config.onboard_humidity) + String(';'));

  settingsFile.print( String(this->config.DEBUG_EN) + String(';'));

  settingsFile.print( String(this->config.DEBUG_TO) + String(';'));

  settingsFile.print( String(this->config.DEBUG_TYPE) + String(';'));

  settingsFile.print( String(this->config.DEBUG_SEND_REPOSITORY) + String(';'));

  settingsFile.print( this->config.SENSOR_DATA_FILENAME + String(';'));

  settingsFile.print( this->config.DEVICE_PASSWORD + String(';'));

  settingsFile.print( this->config.DEVICE_BLE_NAME + String(';'));

  settingsFile.close();
  return true;
}

// ********************************************************************************
bool INTERFACE_CLASS::loadSettings(fs::FS &fs){
  this->mserial->printStr("Loading the Smart Device settings...");
  
  File settingsFile = fs.open("/settings.cfg", FILE_READ);
  if (!settingsFile){
    this->mserial->printStrln("not found.");
    return false;
  }
  if (settingsFile.size() == 0){
    this->mserial->printStrln("Invalid file");
    return false;    
  }

  File settingsFile2 = fs.open("/settings.cfg", FILE_READ);
 // Serial.println("full contents:");
 // Serial.println( settingsFile2.readString() );
 // Serial.println(" === END ===== ");
        
    this->mserial->printStrln("done. Size: " + String( settingsFile.size()) );

          // firmmware update  ***************************
    this->config.firmwareUpdate = settingsFile.readStringUntil(';');
      // ******************* Power management **************
    String temp= settingsFile.readStringUntil(';');
    this->config.isBatteryPowered = *(temp.c_str()) != '0';
    temp= settingsFile.readStringUntil(';');
    this->config.POWER_PLUG_ALWAYS_ON = *(temp.c_str()) != '0';
      // ********************* WIFI *************************
    temp= settingsFile.readStringUntil(';');
    this->config.isWIFIenabled = *(temp.c_str()) != '0';

    this->setNumberWIFIconfigured(0);
    for(int i=0; i<5; i++){
      temp = settingsFile.readStringUntil(';');
      temp.trim();
      this->config.ssid[i] = temp;

      temp = settingsFile.readStringUntil(';');
      temp.trim();
      this->config.password[i] = temp;
      
      if (this->config.ssid[i].length()>0 && isStringAllSpaces(this->config.ssid[i])==false ){
        this->setNumberWIFIconfigured(this->getNumberWIFIconfigured()+1);
      }
    }
    this->mserial->printStrln( "number of wifi networks is " + String(this->getNumberWIFIconfigured()));

    // RTC: NTP server ***********************
    this->config. ntpServer= settingsFile.readStringUntil(';');

    this->config.gmtOffset_sec = atol(settingsFile.readStringUntil( ';' ).c_str() ); 

    temp = settingsFile.readStringUntil(';');
    this->config.daylightOffset_sec = temp.toInt();
    
    this->config.NTP_request_interval = atol(settingsFile.readStringUntil( ';' ).c_str() ); 

      // ********************** onboard sensors *********************
    temp= settingsFile.readStringUntil(';');
    this->config.onboard_motion_sensor_en = *(temp.c_str()) != '0';

    temp= settingsFile.readStringUntil(';');
    this->config.onboard_temperature_en = *(temp.c_str()) != '0';

    temp= settingsFile.readStringUntil(';');
    this->config.onboard_humidity = *(temp.c_str()) != '0';

    // ************* DEBUG *****************
    temp= settingsFile.readStringUntil(';');
    this->config.DEBUG_EN = *(temp.c_str()) != '0';

    temp = settingsFile.readStringUntil(';');
    this->config.DEBUG_TO = temp.toInt();

    temp = settingsFile.readStringUntil(';');
    this->config.DEBUG_TYPE = temp.toInt();

    temp= settingsFile.readStringUntil(';');
    this->config.DEBUG_SEND_REPOSITORY = *(temp.c_str()) != '0';

    this->config.SENSOR_DATA_FILENAME = settingsFile.readStringUntil(';');
    Serial.print("Loading settings : filename is ");
    Serial.println(this->config.SENSOR_DATA_FILENAME);

    this->config.DEVICE_PASSWORD = settingsFile.readStringUntil(';');
    this->config.DEVICE_BLE_NAME = settingsFile.readStringUntil(';');

  settingsFile.close();

  this->mserial->DEBUG_EN = this->config.DEBUG_EN ; 
  this->mserial->DEBUG_TO = this->config.DEBUG_TO; 
  this->mserial->DEBUG_TYPE = this->config.DEBUG_TYPE; 
  this->mserial->DEBUG_SEND_REPOSITORY = this->config.DEBUG_SEND_REPOSITORY ;

  return true;
}