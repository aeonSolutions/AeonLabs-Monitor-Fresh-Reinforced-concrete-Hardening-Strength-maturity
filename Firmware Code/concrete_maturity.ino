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

const static char* GITHUB_ROOT_CA_RSA_SHA384 PROGMEM = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFajCCBPGgAwIBAgIQDNCovsYyz+ZF7KCpsIT7HDAKBggqhkjOPQQDAzBWMQsw\n" \
"CQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMTAwLgYDVQQDEydEaWdp\n" \
"Q2VydCBUTFMgSHlicmlkIEVDQyBTSEEzODQgMjAyMCBDQTEwHhcNMjMwMjE0MDAw\n" \
"MDAwWhcNMjQwMzE0MjM1OTU5WjBmMQswCQYDVQQGEwJVUzETMBEGA1UECBMKQ2Fs\n" \
"aWZvcm5pYTEWMBQGA1UEBxMNU2FuIEZyYW5jaXNjbzEVMBMGA1UEChMMR2l0SHVi\n" \
"LCBJbmMuMRMwEQYDVQQDEwpnaXRodWIuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0D\n" \
"AQcDQgAEo6QDRgPfRlFWy8k5qyLN52xZlnqToPu5QByQMog2xgl2nFD1Vfd2Xmgg\n" \
"nO4i7YMMFTAQQUReMqyQodWq8uVDs6OCA48wggOLMB8GA1UdIwQYMBaAFAq8CCkX\n" \
"jKU5bXoOzjPHLrPt+8N6MB0GA1UdDgQWBBTHByd4hfKdM8lMXlZ9XNaOcmfr3jAl\n" \
"BgNVHREEHjAcggpnaXRodWIuY29tgg53d3cuZ2l0aHViLmNvbTAOBgNVHQ8BAf8E\n" \
"BAMCB4AwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMIGbBgNVHR8EgZMw\n" \
"gZAwRqBEoEKGQGh0dHA6Ly9jcmwzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydFRMU0h5\n" \
"YnJpZEVDQ1NIQTM4NDIwMjBDQTEtMS5jcmwwRqBEoEKGQGh0dHA6Ly9jcmw0LmRp\n" \
"Z2ljZXJ0LmNvbS9EaWdpQ2VydFRMU0h5YnJpZEVDQ1NIQTM4NDIwMjBDQTEtMS5j\n" \
"cmwwPgYDVR0gBDcwNTAzBgZngQwBAgIwKTAnBggrBgEFBQcCARYbaHR0cDovL3d3\n" \
"dy5kaWdpY2VydC5jb20vQ1BTMIGFBggrBgEFBQcBAQR5MHcwJAYIKwYBBQUHMAGG\n" \
"GGh0dHA6Ly9vY3NwLmRpZ2ljZXJ0LmNvbTBPBggrBgEFBQcwAoZDaHR0cDovL2Nh\n" \
"Y2VydHMuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0VExTSHlicmlkRUNDU0hBMzg0MjAy\n" \
"MENBMS0xLmNydDAJBgNVHRMEAjAAMIIBgAYKKwYBBAHWeQIEAgSCAXAEggFsAWoA\n" \
"dwDuzdBk1dsazsVct520zROiModGfLzs3sNRSFlGcR+1mwAAAYZQ3Rv6AAAEAwBI\n" \
"MEYCIQDkFq7T4iy6gp+pefJLxpRS7U3gh8xQymmxtI8FdzqU6wIhALWfw/nLD63Q\n" \
"YPIwG3EFchINvWUfB6mcU0t2lRIEpr8uAHYASLDja9qmRzQP5WoC+p0w6xxSActW\n" \
"3SyB2bu/qznYhHMAAAGGUN0cKwAABAMARzBFAiAePGAyfiBR9dbhr31N9ZfESC5G\n" \
"V2uGBTcyTyUENrH3twIhAPwJfsB8A4MmNr2nW+sdE1n2YiCObW+3DTHr2/UR7lvU\n" \
"AHcAO1N3dT4tuYBOizBbBv5AO2fYT8P0x70ADS1yb+H61BcAAAGGUN0cOgAABAMA\n" \
"SDBGAiEAzOBr9OZ0+6OSZyFTiywN64PysN0FLeLRyL5jmEsYrDYCIQDu0jtgWiMI\n" \
"KU6CM0dKcqUWLkaFE23c2iWAhYAHqrFRRzAKBggqhkjOPQQDAwNnADBkAjAE3A3U\n" \
"3jSZCpwfqOHBdlxi9ASgKTU+wg0qw3FqtfQ31OwLYFdxh0MlNk/HwkjRSWgCMFbQ\n" \
"vMkXEPvNvv4t30K6xtpG26qmZ+6OiISBIIXMljWnsiYR1gyZnTzIg3AQSw4Vmw==\n" \
"-----END CERTIFICATE-----\n";


//----------------------------------------------------------------------------------------
// Components Testing  **************************************
bool SCAN_I2C_BUS = false;
bool TEST_FINGERPRINT_ID_IC=true;

//----------------------------------------------------------------------------------
#include <math.h>
#include <cmath>
#include "SPI.h"
#include <semphr.h>

// custom includes
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

// MCU frequency management
#include "manage_mcu_freq.h"

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
#include "FFat.h"

FILE_CLASS* drive = new FILE_CLASS(mserial);

// OTA updates
#include <esp32FOTA.hpp>
esp32FOTA esp32FOTA("esp32-fota-http", "0.0.0", false);

// WIFI Class
#include "m_wifi.h"
M_WIFI_CLASS* mWifi= new M_WIFI_CLASS();

// Geo Location
#include "m_geolocation.h"
GEO_LOCATION_CLASS geoLocation = GEO_LOCATION_CLASS();

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

bool deviceDisconnected_BLE_callback = false;
bool BLE_advertise_Started = false;

bool newValueToSend = false;
String $BLE_CMD="";
bool newCMDarrived = false;
SemaphoreHandle_t MemLockSemaphoreBLE_RX = xSemaphoreCreateMutex();

float txValue = 0;
String valueReceived="";


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      interface->BLE_IS_DEVICE_CONNECTED = true;
      deviceDisconnected_BLE_callback=false;
      interface->onBoardLED->led[0] = interface->onBoardLED->LED_BLUE;
      interface->onBoardLED->statusLED(100, 1);
    };

    void onDisconnect(BLEServer* pServer) {
      interface->BLE_IS_DEVICE_CONNECTED = false;
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
      mserial->ble=false;
      delay(10);

      String txValue = String(pCharacteristic->getValue().c_str());
      txValue.trim();
      mserial->printStrln("Transmitted TX Value: " + String(txValue.c_str()) );

      if (txValue.length() == 0) {
        mserial->printStr("Transmitted TX Value: empty ");
      }
    }

    void onRead(BLECharacteristic *pCharacteristic) {
      mserial->printStr("TX onRead...");    
      //pCharacteristic->setValue("OK");
    }
  }; 
                    
class pCharacteristicRX_Callbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      mserial->ble=false;
      delay(10);

      String rxValue = String(pCharacteristic->getValue().c_str());
      rxValue.trim();
      mserial->printStrln("Received RX Value: " + String(rxValue.c_str()) );

      if (rxValue.length() == 0) {
        mserial->printStr("Received RX Value: empty ");
      }
      
      $BLE_CMD=rxValue;
      mserial->ble=true;

      xSemaphoreTake(MemLockSemaphoreBLE_RX, portMAX_DELAY); 
        newCMDarrived=true; // this needs to be the last line       
      xSemaphoreGive(MemLockSemaphoreBLE_RX);
      delay(50);   
    } 

    void onRead(BLECharacteristic *pCharacteristic) {
      mserial->printStr("RX onRead...");    
      //pCharacteristic->setValue("OK");
    }

  }; 

//------------------------------------------------------------------
long int runningTimeMillis;
long int prevMeasurementMillis;

void setup() { 
  // Firmware Build Version / revision ______________________________
  interface->firmware_version="1.0.7";

  MemLockSemaphoreBLE_RX = xSemaphoreCreateMutex();

  interface->UARTserial = &UARTserial;
  mserial->start(115200);
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

  //init storage drive
  drive->partition_info();
  ONBOARD_LED_CLASS* onBoardLED = new ONBOARD_LED_CLASS();
  if (drive->init(FFat, "storage", 2, mserial,  onBoardLED ) == false)
    while(1);
    
  // Init Maturity  __________________________________________
  maturity-> EXT_PLUG_DI_IO = 6;
  pinMode( maturity->EXT_PLUG_DI_IO, INPUT );
  maturity->init(interface, onBoardSensors, mserial, interface->onBoardLED);

  // ......................................................................................................
  // .......................... END OF IO & PIN CONFIGURATION..............................................
  // ......................................................................................................

  interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
  interface->onBoardLED->statusLED(100, 0);
  
  // init onboard sensors
  onBoardSensors->init(interface, mserial);

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


  if ( !interface->loadSettings() ){
    interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
    interface->onBoardLED->led[0] = interface->onBoardLED->LED_GREEN;
    interface->onBoardLED->statusLED(100, 2);
  }

  interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
  interface->onBoardLED->statusLED(100, 0);

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

  changeMcuFreq(interface, interface->WIFI_FREQUENCY);
  mserial->printStrln("setting to Boot min CPU Freq = " +String(getCpuFrequencyMhz()));
  mserial->printStrln("");

  // init BLE 
  BLE_init();

  //interface->add_wifi_network("TheScientist", "angelaalmeidasantossilva");

  //init wifi
  mWifi->init(interface, mserial, interface->onBoardLED);
  // init Geo Location
  geoLocation.init(interface, mWifi, mserial);
  
  mWifi->WIFIscanNetworks();
  
  // check for firmwate update
  forceFirmwareUpdate();

  interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
  interface->onBoardLED->statusLED(100, 0);

  Serial.println("[APP] Free memory: " + String(esp_get_free_heap_size()) + " bytes");
  dataverse->init(interface, mWifi, mserial);

  //Init GBRL 
  gbrl.init(interface, maturity, mserial);

  interface->onBoardLED->led[0] = interface->onBoardLED->LED_BLUE;
  interface->onBoardLED->statusLED(100, 0);

  interface->$espunixtimePrev = millis();
  interface->$espunixtimeStartMeasure = millis(); 
  
  interface->$espunixtimeDeviceDisconnected = millis();

  runningTimeMillis = millis();
  prevMeasurementMillis = millis();
  
  mserial->printStrln("\nsetup is completed. You may start using the " + String(DEVICE_NAME) );
  mserial->printStrln("Type $? for a List of commands.");

  interface->onBoardLED->led[0] = interface->onBoardLED->LED_GREEN;
  interface->onBoardLED->statusLED(100, 1);
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

// *****************************************************************
bool welcomeIntroDone=false;
String dataStr="";
long int eTime;
// adc_power_release()


void loop() {

  if ( interface->BLE_IS_DEVICE_CONNECTED  && welcomeIntroDone==false ) {
    delay(3000);
    welcomeIntroDone=true;
    dataStr="Connected to the Smart Concrete Maturity device (" + String(interface->firmware_version) + ")"+ String(char(10)) + String(char(13)) +"Type $? or $help to see a list of available commands"+ String(char(10));
    dataStr+=String(interface->rtc.getDateTime(true))+String(char(10))+String(char(10));
    
    if (interface->getNumberWIFIconfigured() == 0 ){
      dataStr += "no WiFi Networks Configured"+String(char(10)) + String(char(10));
    }

    interface->sendBLEstring(dataStr); 
  }

// ................................................................................

  if ( deviceDisconnected_BLE_callback && onBoardSensors->motionShakeDetected(5) && ( interface->CURRENT_CLOCK_FREQUENCY != interface->WIFI_FREQUENCY ) ) {
        Serial.println("next 0");
    deviceDisconnected_BLE_callback=false;
    changeMcuFreq(interface, interface->WIFI_FREQUENCY);
    interface->onBoardLED->led[0] = interface->onBoardLED->LED_BLUE;
    interface->onBoardLED->statusLED(100, 1);
    
    WiFi.mode(WIFI_STA);
    // init BLE 
    BLE_init();
    runningTimeMillis= millis();
  }
    
  if (deviceDisconnected_BLE_callback && interface->CURRENT_CLOCK_FREQUENCY >= interface->WIFI_FREQUENCY){
          Serial.println("next `1");
      interface->$espunixtimeDeviceDisconnected=millis();
      deviceDisconnected_BLE_callback=true;
      btStop();
      delay(100);
      BLEDevice::deinit();
      WiFi.disconnect(true);
      delay(100);
      WiFi.mode(WIFI_MODE_NULL);

      changeMcuFreq(interface, interface->MIN_MCU_FREQUENCY);
      
      interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
      interface->onBoardLED->led[1] = interface->onBoardLED->LED_GREEN;
      interface->onBoardLED->statusLED(100, 2);
    }
  

// ................................................................................

  eTime = millis() - prevMeasurementMillis;
  //ToDo : force BLE dsiconnect after [time] of innactivity
  if ( interface->BLE_IS_DEVICE_CONNECTED==false && interface->LIGHT_SLEEP_EN && interface->Measurments_EN==true) {
    if (eTime < maturity->config.MEASUREMENT_INTERVAL ){
      interface->onBoardLED->turnOffAllStatusLED();
      mserial->printStr("Entering light sleep....");
      esp_sleep_enable_timer_wakeup( ( (maturity->config.MEASUREMENT_INTERVAL - eTime) /1000)  * uS_TO_S_FACTOR);
      delay(100);
      esp_light_sleep_start();
      mserial->printStrln("wake up done.");
      if ( interface->BLE_IS_DEVICE_CONNECTED){
            interface->sendBLEstring( "waking up. You may enter any GBRL command now.");
      }
    }
  }

// ................................................................................ 
  // check for measurement interval time and do measurement data collection from the DS18b20
  maturity->request_measurment_data();

  if (maturity->hasNewMeasurementValues==true ){
    prevMeasurementMillis=millis();
  }

// ................................................................................

  if (interface->BLE_IS_DEVICE_CONNECTED && maturity->hasNewMeasurementValues==true  && (millis() != prevMeasurementMillis) ) {
    mserial->printStrln("Device connected...");
    float maturityVal = maturity->custom_maturity(maturity->DATASET_NUM_SAMPLES);
    float strenghtVal = maturity->custom_strenght(maturity->DATASET_NUM_SAMPLES);

    dataStr = "Current Temperature: " + String(roundFloat(maturity->last_measured_probe_temp,2)) + String(char(176))+String("C  ");
    dataStr += "Maturity: "+ String(roundFloat( maturityVal ,2)) + String(char(176)) + String("C.h  ");
    dataStr += "Strength: " + String(roundFloat( strenghtVal,2) ) + String(" MPa") + String(char(10));
    dataStr += "Unique Data Fingerprint ID:"+String(char(10));
    dataStr += CryptoICserialNumber(interface)+"-"+macChallengeDataAuthenticity(interface, String(interface->rtc.getDateTime(true)) + String(roundFloat(maturity->last_measured_probe_temp,2))+"-"+String(roundFloat(maturityVal ,2))+"-"+String(roundFloat(strenghtVal ,2))  );
    dataStr += String(char(10) + String(char(10)) );
    interface->sendBLEstring( dataStr);

    mserial->printStrln("BLE output...");
    mserial->printStr("TTF( Maturity): ");
    mserial->printStrln(String(roundFloat(maturityVal ,2)));

    mserial->printStr("Strenght(MPa): ");
    mserial->printStrln(String(strenghtVal ));

    maturity->hasNewMeasurementValues=false;
  }
  
  // ................................................................................

  geoLocation.get_ip_geo_location_data();
  dataverse->UploadToDataverse(interface->BLE_IS_DEVICE_CONNECTED);


 // ................................................................................    
  if (mserial->readSerialData()){
    String $BLE_CMD_Serial= mserial->serialDataReceived;
    if (gbrl.commands($BLE_CMD_Serial, mserial->DEBUG_TO_USB ) == false){
      if( onBoardSensors->commands($BLE_CMD_Serial, mserial->DEBUG_TO_USB ) == false){
        if (mWifi->gbrl_commands($BLE_CMD_Serial, mserial->DEBUG_TO_USB ) == false){    
          if (maturity->gbrl_commands($BLE_CMD_Serial, mserial->DEBUG_TO_USB ) == false){
            if( dataverse->gbrl_commands($BLE_CMD_Serial, mserial->DEBUG_TO_USB ) == false){
              if ($BLE_CMD_Serial.indexOf("$")==-1 || $BLE_CMD_Serial.indexOf("$")>0){
                mserial->printStrln("$ CMD ERROR \r\n");
              }
            }
          }
        }
      }
    }
  }

 // ................................................................................    
  if (mserial->readUARTserialData()){
    String $BLE_CMD_Serial_UART= mserial->serialDataReceived;
    if (gbrl.commands($BLE_CMD_Serial_UART, mserial->DEBUG_TO_UART ) == false){
      if( onBoardSensors->commands($BLE_CMD_Serial_UART, mserial->DEBUG_TO_UART ) == false){
        if (mWifi->gbrl_commands($BLE_CMD_Serial_UART, mserial->DEBUG_TO_USB ) == false){    
          if (maturity->gbrl_commands($BLE_CMD_Serial_UART, mserial->DEBUG_TO_UART ) == false){
            if( dataverse->gbrl_commands($BLE_CMD_Serial_UART, mserial->DEBUG_TO_UART ) == false){
              if ($BLE_CMD_Serial_UART.indexOf("$")==-1 || $BLE_CMD_Serial_UART.indexOf("$")>0){
                mserial->printStrln("$ CMD ERROR \r\n");
              }
            }
          }
        }
      }
    }
  }
// ................................................................................
  if (newCMDarrived){
    Serial.println("[APP] Free memory: " + String(esp_get_free_heap_size()) + " bytes");

    xSemaphoreTake(MemLockSemaphoreBLE_RX, portMAX_DELAY); 
      newCMDarrived=false; // this needs to be the last line       
    xSemaphoreGive(MemLockSemaphoreBLE_RX);

    if (gbrl.commands($BLE_CMD, mserial->DEBUG_TO_BLE ) == false){
      if( onBoardSensors->commands($BLE_CMD,  mserial->DEBUG_TO_BLE ) == false){
        if (maturity->gbrl_commands($BLE_CMD,  mserial->DEBUG_TO_BLE ) == false){
          if( dataverse->gbrl_commands($BLE_CMD,  mserial->DEBUG_TO_BLE ) == false){
            if ($BLE_CMD.indexOf("$")==-1 || $BLE_CMD.indexOf("$")>0){
              interface->sendBLEstring("$ CMD ERROR \r\n");
            }else{
              interface->sendBLEstring("$ CMD UNK \r\n");
            }
          }
        }
      }
    }

  }
// ................................................................................

  if ( interface->forceFirmwareUpdate == true ){
    forceFirmwareUpdate();
  }
}

// *******************************************************************************************
void forceFirmwareUpdate(){
    interface->forceFirmwareUpdate=false;

    if( WiFi.status() != WL_CONNECTED ){
      if (interface->getNumberWIFIconfigured()>0){
        if ( interface->BLE_IS_DEVICE_CONNECTED)
            interface->sendBLEstring("connecting to a wifi network. one moment...");
        
        mWifi->start(10000, 5); // TTL , n attempts 
        delay(500);

        if ( interface->BLE_IS_DEVICE_CONNECTED)
            interface->sendBLEstring("done.\n");
      
      }else{
        mserial->printStrln("Firmware: No Wifi networks configured");
        if ( interface->BLE_IS_DEVICE_CONNECTED)
          interface->sendBLEstring("Firmware: No Wifi networks configured.\n");
      }
    }

  if(WiFi.status() != WL_CONNECTED){
    Serial.println ("Firmware: WIFI not connected");
    return;
  }

  if ( interface->BLE_IS_DEVICE_CONNECTED){
    interface->sendBLEstring("requesting internet IP ...");
    delay(500);
  }
  geoLocation.get_ip_address();
  
  if ( interface->BLE_IS_DEVICE_CONNECTED){
    interface->sendBLEstring("done. \nrequesting Geo Location data ...");
    delay(500);
  }
  geoLocation.get_ip_geo_location_data();
  
  if ( interface->BLE_IS_DEVICE_CONNECTED){
    if ( interface->geoLocationInfoJson.containsKey("country") && interface->geoLocationInfoJson.containsKey("regionName") ){
      mserial->printStrln("done. \n Country:" + String(interface->geoLocationInfoJson["country"].as<char*>()) + "\n Region: " + String(interface->geoLocationInfoJson["regionName"].as<char*>()) + "\n");
      interface->sendBLEstring("done. \n Country:" + String(interface->geoLocationInfoJson["country"].as<char*>()) + "\n Region: " + String(interface->geoLocationInfoJson["regionName"].as<char*>()) + "\n");
      delay(500);
    }
  }
  // Local RTC
  if ( interface->BLE_IS_DEVICE_CONNECTED){
    interface->sendBLEstring("syncronizing with a Internet Time server...");
    delay(500);
  }
  interface->init_NTP_Time();
  mWifi->updateInternetTime();
  
  if ( interface->BLE_IS_DEVICE_CONNECTED){
    delay(500);
    interface->sendBLEstring("done. \nRequesting the lastest firmware revision....");
    delay(500);
  }
  
  //init OTA updates
  {
    auto cfg = esp32FOTA.getConfig();
    cfg.name          = "esp32-fota-http";
    cfg.manifest_url  = "https://github.com/aeonSolutions/AeonLabs-Monitor-Fresh-Reinforced-concrete-Hardening-Strength-maturity/releases/download/openFirmware/version.json";
    cfg.sem           = SemverClass( interface->firmware_version.substring(0, interface->firmware_version.indexOf(".")).toInt() , interface->firmware_version.substring( interface->firmware_version.indexOf(".")+1, interface->firmware_version.lastIndexOf(".")).toInt(), interface->firmware_version.substring( interface->firmware_version.lastIndexOf(".")+1, interface->firmware_version.length() ).toInt() ); // major, minor, patch
    cfg.check_sig     = false; // verify signed firmware with rsa public key
    cfg.unsafe        = true; // disable certificate check when using TLS
    cfg.root_ca       = new CryptoMemAsset("Root CA", GITHUB_ROOT_CA_RSA_SHA384, strlen(GITHUB_ROOT_CA_RSA_SHA384)+1 );
    //cfg.pub_key       = MyRSAKey;
    //cfg.use_device_id = false;
    esp32FOTA.setConfig( cfg );
  }

  //esp32FOTA.setManifestURL( manifest_url );
  bool updatedNeeded = esp32FOTA.execHTTPcheck();
  if (updatedNeeded) {
    interface->onBoardLED->led[0] = interface->onBoardLED->LED_BLUE;
    interface->onBoardLED->statusLED(100,0); 
    mserial->printStrln("new firmware version found. Starting update ...");
    if ( interface->BLE_IS_DEVICE_CONNECTED){
      delay(500);
      interface->sendBLEstring("done. \nNew firmware version found. Starting to download and upodate. The Device will reboot when completed. ");
      delay(500);
    }

    esp32FOTA.execOTA();
    WiFi.disconnect(true);
    return;
  }else{
    mserial->printStrln("no firmware update needed.");
    if ( interface->BLE_IS_DEVICE_CONNECTED){
      delay(500);
      interface->sendBLEstring("done. \nNo new firmware available");
      delay(500);
    }
    interface->onBoardLED->led[0] = interface->onBoardLED->LED_GREEN;
    interface->onBoardLED->statusLED(100, 1);   
  }

  WiFi.disconnect(true);

}
