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

#define uS_TO_S_FACTOR 1000000

//----------------------------------------------------------------------------------------
// Components Testing  **************************************
bool SCAN_I2C_BUS = false;
bool TEST_FINGERPRINT_ID_IC=false;

//----------------------------------------------------------------------------------
#include <math.h>
#include <cmath>
#include "SPI.h"
#include <semphr.h>

// custom includes **********************************
// custom functions
#include "m_file_functions.h"
// Interface class ******************************
#include "interface_class.h"
INTERFACE_CLASS* interface = new INTERFACE_CLASS();
#define DEVICE_NAME "Smart Concrete Curing Device."

// GBRL commands  ***************************
#include "gbrl.h"
GBRL gbrl = GBRL();

// DATAVERSE **********************************
#include "dataverse.h"
DATAVERSE_CLASS* dataverse = new DATAVERSE_CLASS();


// Maturity Management and Calculations
#include "maturity.h"
MATURITY_CLASS* maturity = new MATURITY_CLASS();

// Onboard sensors  *******************************
#include "onboard_sensors.h"
ONBOARD_SENSORS* onBoardSensors = new ONBOARD_SENSORS();

// unique figerprint data ID 
#include "m_atsha204.h"

// serial comm
#include <HardwareSerial.h>
HardwareSerial UARTserial(0);

#include "mserial.h"
mSerial* mserial = new mSerial(true, &UARTserial);

// File class
#include "m_file_class.h"
#include <esp_partition.h>
#include "FS.h"
#include <LittleFS.h>

FILE_CLASS* drive = new FILE_CLASS(mserial);

// OTA updates
#include <esp32FOTA.hpp>
esp32FOTA* esp32fota;

// WIFI Class
#include "m_wifi.h"
M_WIFI_CLASS* mWifi= new M_WIFI_CLASS();

// Certificates
#include "cert/github_cert.h"
/********************************************************************/ 

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

BLECharacteristic *pCharacteristicTX, *pCharacteristicRX;
BLEServer *pServer;
BLEService *pService;


bool BLE_advertise_Started = false;

bool newValueToSend = false;
String $BLE_CMD="";
bool newCMDarrived = false;
SemaphoreHandle_t MemLockSemaphoreBLE_RX = xSemaphoreCreateMutex();

float txValue = 0;
String valueReceived="";


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      mWifi->setBLEconnectivityStatus (true);
      mserial->printStr("BLE connection init ", mserial->DEBUG_BOTH_USB_UART);
      
      interface->onBoardLED->led[0] = interface->onBoardLED->LED_BLUE;
      interface->onBoardLED->statusLED(100, 1);

      String dataStr="Connected to the Smart Concrete Maturity device (" + String(interface->firmware_version) + ")"+ String(char(10)) + String(char(13)) +"Type $? or $help to see a list of available commands"+ String(char(10));
      dataStr+=String(interface->rtc.getDateTime(true))+String(char(10))+String(char(10));
    
      if (mWifi->getNumberWIFIconfigured() == 0 ){
        dataStr += "no WiFi Networks Configured"+String(char(10)) + String(char(10));
      }
      //interface->sendBLEstring(dataStr, mserial->DEBUG_TO_BLE);  
    }

    void onDisconnect(BLEServer* pServer) {
      mWifi->setBLEconnectivityStatus (false);
      
      interface->onBoardLED->led[0] = interface->onBoardLED->LED_BLUE;
      interface->onBoardLED->statusLED(100, 0.5);
      interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
      interface->onBoardLED->statusLED(100, 0.5);
      interface->onBoardLED->led[0] = interface->onBoardLED->LED_BLUE;
      interface->onBoardLED->statusLED(100, 0.5);

      pServer->getAdvertising()->start();
    }
};

class pCharacteristicTX_Callbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      String txValue = String(pCharacteristic->getValue().c_str());
      txValue.trim();
      mserial->printStrln("Transmitted TX Value: " + String(txValue.c_str()) , mserial->DEBUG_BOTH_USB_UART);

      if (txValue.length() == 0) {
        mserial->printStr("Transmitted TX Value: empty ", mserial->DEBUG_BOTH_USB_UART);
      }
    }

    void onRead(BLECharacteristic *pCharacteristic) {
      mserial->printStr("TX onRead...", mserial->DEBUG_BOTH_USB_UART);    
      //pCharacteristic->setValue("OK");
    }
  }; 
                    
class pCharacteristicRX_Callbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      delay(10);

      String rxValue = String(pCharacteristic->getValue().c_str());
      rxValue.trim();
      mserial->printStrln("Received RX Value: " + String(rxValue.c_str()), mserial->DEBUG_BOTH_USB_UART );

      if (rxValue.length() == 0) {
        mserial->printStr("Received RX Value: empty " , mserial->DEBUG_BOTH_USB_UART);
      }
      
      $BLE_CMD = rxValue;
      mWifi->setBLEconnectivityStatus(true);
      
      xSemaphoreTake(MemLockSemaphoreBLE_RX, portMAX_DELAY); 
        newCMDarrived=true; // this needs to be the last line       
      xSemaphoreGive(MemLockSemaphoreBLE_RX);

      delay(50);   
    } 

    void onRead(BLECharacteristic *pCharacteristic) {
      mserial->printStr("RX onRead..." , mserial->DEBUG_BOTH_USB_UART);    
      //pCharacteristic->setValue("OK");
    }

  }; 

//------------------------------------------------------------------

long int prevMeasurementMillis;

void setup() { 
  // Firmware Build Version / revision ______________________________
  interface->firmware_version="1.0.16";

  MemLockSemaphoreBLE_RX = xSemaphoreCreateMutex();

  interface->UARTserial = &UARTserial;
  mserial->start(115200);
  
  mserial->DEBUG_EN=false;
  mserial->DEBUG_TYPE = mserial->DEBUG_TYPE_INFO;

  interface->init(mserial, true); // debug EN ON

  // ......................................................................................................
  // .......................... START OF IO & PIN CONFIGURATION..............................................
  // ......................................................................................................
  // I2C IOs  __________________________
  interface->I2C_SDA_IO_PIN = 8;
  interface->I2C_SCL_IO_PIN = 9;
  // Power Saving ____________________________________
  interface->LIGHT_SLEEP_EN=false;

  // External Ports IO Pin assignment ________________________________
  interface-> EXT_PLUG_PWR_EN = 2;
  pinMode(interface->EXT_PLUG_PWR_EN,OUTPUT);
  digitalWrite(interface->EXT_PLUG_PWR_EN ,LOW);

  // Battery Power Monitor ___________________________________
  pinMode(interface->BATTERY_ADC_IO, INPUT);

  // ________________ Onborad LED  _____________
  interface->onBoardLED->LED_RED = 11;
  interface->onBoardLED->LED_BLUE = 13;
  interface->onBoardLED->LED_GREEN = 10;

  interface->onBoardLED->LED_RED_CH = 0;
  interface->onBoardLED->LED_BLUE_CH = 2;
  interface->onBoardLED->LED_GREEN_CH = 9;
  interface->onBoardLED->init();

  // ___________ MCU freq ____________________
  interface-> SAMPLING_FREQUENCY = 240; 
  interface-> WIFI_FREQUENCY = 80; // min WIFI MCU Freq is 80-240
  interface->MIN_MCU_FREQUENCY = 10;
  interface-> SERIAL_DEFAULT_SPEED = 115200;

  interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
  interface->onBoardLED->statusLED(100, 0);

  //init storage drive
  drive->partition_info();
  if (drive->init(LittleFS, "storage", 2, mserial,   interface->onBoardLED ) == false)
    while(1);
  
  interface->settings_defaults();

  if ( !interface->loadSettings() ){
    interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
    interface->onBoardLED->led[0] = interface->onBoardLED->LED_GREEN;
    interface->onBoardLED->statusLED(100, 2);
  }
    
  // init onboard sensors
  onBoardSensors->init(interface, mserial);
  onBoardSensors->initRollTheshold();

  // Init Maturity  __________________________________________
  maturity-> EXT_PLUG_DI_IO = 6;
  pinMode( maturity->EXT_PLUG_DI_IO, INPUT );
  
  // ......................................................................................................
  // .......................... END OF IO & PIN CONFIGURATION..............................................
  // ......................................................................................................

  
  /*
  Partition Table Information =========================================================
  8Mb Flash				
  # Name	   Type	 SubType	  Offset	     Size
  nvs	      data	 nvs	      0x9000	    0x5000
  otadata	  data	 ota	      0xe000	    0x2000
  factory	  app	  factory	    0x10000	    0x1C0000
  ota_0	    app	  ota_0	      0x2C0000	  0x1C0000
  ota_1	    app	  ota_1	      0x480000	  0x1C0000
  eeprom	  data	 0x99	      0x640000	  0x1000
  storage	  data	 fatfs	    0x641000	  0x1C0000
  ====================================================================================
  */


  if (SCAN_I2C_BUS){
    onBoardSensors->I2Cscanner();
  }

  if (TEST_FINGERPRINT_ID_IC){
    mserial->printStrln("Testing the Unique FingerPrind ID for Sensor Data Measurements");

    mserial->printStr("This Smart Device  Serial Number is : ");
    mserial->printStrln(CryptoICserialNumber(interface));

    mserial->printStrln("Testing Random Genenator: " + CryptoGetRandom(interface));
    mserial->printStrln("");

    mserial->printStrln("Testing Sensor Data Validation hashing");
    mserial->printStrln( macChallengeDataAuthenticity(interface, "TEST IC"));
    mserial->printStrln("");
  }
  
  mserial->printStrln("\nMicrocontroller specifications:");
  interface->CURRENT_CLOCK_FREQUENCY=getCpuFrequencyMhz();
  mserial->printStr("Internal Clock Freq = ");
  mserial->printStr(String(interface->CURRENT_CLOCK_FREQUENCY));
  mserial->printStrln(" MHz");

  interface->Freq = getXtalFrequencyMhz();
  mserial->printStr("XTAL Freq = ");
  mserial->printStr(String(interface->Freq));
  mserial->printStrln(" MHz");
  
  interface->Freq = getApbFrequency();
  mserial->printStr("APB Freq = ");
  mserial->printStr(String(interface->Freq/1000000));
  mserial->printStrln(" MHz\n");

  interface->setMCUclockFrequency( interface->WIFI_FREQUENCY);
  mserial->printStrln("setting to Boot min CPU Freq = " +String(getCpuFrequencyMhz()));
  mserial->printStrln("");

  // init BLE 
  BLE_init();
  
  //init wifi
  mWifi->init(interface, drive, interface->onBoardLED);

  mWifi->WIFIscanNetworks();
  
  // check for firmwate update
  mWifi->startFirmwareUpdate();

  interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
  interface->onBoardLED->statusLED(100, 0);
  
  maturity->init(interface, onBoardSensors, mserial, mWifi);
  
  dataverse->init(interface, mWifi, mserial);

  //Init GBRL 
  gbrl.init(interface, mWifi);

  interface->onBoardLED->led[0] = interface->onBoardLED->LED_BLUE;
  interface->onBoardLED->statusLED(100, 0);

  interface->$espunixtimePrev = millis();
  interface->$espunixtimeStartMeasure = millis(); 
  
  mWifi->$espunixtimeDeviceDisconnected = millis();

  prevMeasurementMillis = millis();
  mserial->printStrln("Free memory: " + addThousandSeparators( std::string( String(esp_get_free_heap_size() ).c_str() ) ) + " bytes");
  
  mserial->printStrln("\nsetup is completed. You may start using the " + String(DEVICE_NAME) );
  mserial->printStrln("Type $? for a List of commands.\n");

  interface->onBoardLED->led[0] = interface->onBoardLED->LED_GREEN;
  interface->onBoardLED->statusLED(100, 1);
}
// *****************************************************************

String dataStr="";
long int eTime;
long int statusTime = millis();
long int beacon = millis();
// adc_power_release()


void loop() {
if (millis() - beacon > 10000){
  beacon = millis();
  mserial->printStrln("(" + String(beacon) + ") Free memory: " + addThousandSeparators( std::string( String(esp_get_free_heap_size() ).c_str() ) ) + " bytes\n", mSerial::DEBUG_TYPE_VERBOSE, mSerial::DEBUG_ALL_USB_UART_BLE);
}

// ................................................................................
  if ( onBoardSensors->motionShakeDetected(5) ){
    if ( interface->CURRENT_CLOCK_FREQUENCY < interface->WIFI_FREQUENCY )  {
      mserial->printStrln("motion shake: setting min WIFI freq.");
      mWifi->resumeWifiMode();
      // init BLE 
      BLE_init();
    }
  }

// ............................................................................. 
  // disconnected for at least 3min
  if (  mWifi->getBLEconnectivityStatus()==false && ( millis() - mWifi->$espunixtimeDeviceDisconnected > 180000) && interface->CURRENT_CLOCK_FREQUENCY >= interface->WIFI_FREQUENCY){
      mserial->printStrln("setting min MCU freq.");
      btStop();
      //BLEDevice::deinit(); // crashes the device

      WiFi.disconnect(true);
      delay(100);
      WiFi.mode(WIFI_MODE_NULL);

      interface->setMCUclockFrequency( interface->MIN_MCU_FREQUENCY);
      interface->CURRENT_CLOCK_FREQUENCY = interface->MIN_MCU_FREQUENCY;
      mWifi->setBLEconnectivityStatus(false);

      interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
      interface->onBoardLED->led[1] = interface->onBoardLED->LED_GREEN;
      interface->onBoardLED->statusLED(100, 2);
    }
  
// ................................................................................

  eTime = millis() - prevMeasurementMillis;
  if ( mWifi->getBLEconnectivityStatus()==false && interface->LIGHT_SLEEP_EN && maturity->Measurments_EN==true) {
    if (eTime < maturity->config.MEASUREMENT_INTERVAL ){
      mserial->printStr("Entering light sleep....");
      interface->onBoardLED->turnOffAllStatusLED();

      esp_sleep_enable_timer_wakeup( ( (maturity->config.MEASUREMENT_INTERVAL - eTime) /1000)  * uS_TO_S_FACTOR);
      delay(100);
      esp_light_sleep_start();
      mserial->printStrln("wake up done.");
    }
  }
  prevMeasurementMillis = millis();

if ( (millis() - statusTime > 10000) && maturity->Measurments_EN == true ){ //10 sec
    statusTime = millis();
    interface->onBoardLED->led[1] = interface->onBoardLED->LED_GREEN;
    interface->onBoardLED->statusLED(100, 0.04);
}else if  (millis() - statusTime > 10000){
    statusTime = millis();
    interface->onBoardLED->led[1] = interface->onBoardLED->LED_RED;
    interface->onBoardLED->statusLED(100, 0.04);
}

// ................................................................................ 
  // check for measurement interval time and do measurement data collection from the DS18b20
  maturity->request_measurment_data();

  if (maturity->hasNewMeasurementValues==true ){
    dataStr =  interface->DeviceTranslation("probing_temp") + ": " + String(roundFloat(maturity->last_measured_probe_temp,2)) + String(char(176))+String("C");
    dataStr += String(char(10) + String(char(10)) );
    
    bool sendTo= mSerial::DEBUG_BOTH_USB_UART;
    if (mWifi->getBLEconnectivityStatus() )
      sendTo = mSerial::DEBUG_ALL_USB_UART_BLE;      
    
    interface->sendBLEstring( dataStr, sendTo);
    maturity->hasNewMeasurementValues=false;
  }
  
  // ................................................................................

 dataverse->UploadToDataverse(mWifi->getBLEconnectivityStatus());
 // ................................................................................    

  if (mserial->readSerialData()){
    GBRLcommands(mserial->serialDataReceived, mserial->DEBUG_TO_USB);
  }
 // ................................................................................    

  if (mserial->readUARTserialData()){
    GBRLcommands(mserial->serialUartDataReceived, mserial->DEBUG_TO_UART);
  }
// ................................................................................

  if (newCMDarrived){
    xSemaphoreTake(MemLockSemaphoreBLE_RX, portMAX_DELAY); 
      newCMDarrived=false; // this needs to be the last line       
    xSemaphoreGive(MemLockSemaphoreBLE_RX);

    GBRLcommands($BLE_CMD, mserial->DEBUG_TO_BLE);
  }
// ................................................................................

  if ( mWifi->forceFirmwareUpdate == true )
    mWifi->startFirmwareUpdate();

} // LOOP END
//  ********************************************

void GBRLcommands(String command, uint8_t sendTo){
  if (gbrl.commands(command, sendTo) == false){
    if( onBoardSensors->gbrl_commands(command, sendTo ) == false){
      if (mWifi->gbrl_commands(command, sendTo ) == false){    
          if (maturity->gbrl_commands(command, sendTo ) == false){
            if( dataverse->gbrl_commands(command, sendTo ) == false){
              if ( command.indexOf("$")>-1){
                interface->sendBLEstring("$ CMD ERROR \r\n", sendTo);
              }else{
                // interface->sendBLEstring("$ CMD UNK \r\n", sendTo);
              }
            }
          }
        
      }
    }
  }
}
// ************************************************************

void BLE_init(){
  // Create the BLE Device 
  BLEDevice::init(String("SCC " + interface->config.DEVICE_BLE_NAME).c_str());  // max 29 chars
  
  // Create the BLE Server 
  pServer = BLEDevice::createServer(); 
  pServer->setCallbacks(new MyServerCallbacks());
  // Create the BLE Service
  pService = pServer->createService(SERVICE_UUID);
  
  // Create a BLE Characteristic

  pCharacteristicTX = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );


  pCharacteristicTX->addDescriptor(new BLE2902());

  pCharacteristicRX = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_RX,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  pCharacteristicTX->setCallbacks(new pCharacteristicTX_Callbacks());
  pCharacteristicRX->setCallbacks(new pCharacteristicRX_Callbacks());

  interface->init_BLE(pCharacteristicTX);

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
}
// *******************************************************************************************
