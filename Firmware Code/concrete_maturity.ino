/********************************************************************/
// First we include the libraries
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <math.h>
/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 6
#define EXT_PLUG_PWR_EN 2 
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
float ds_temp;

// EVALUATION OF CONCRETE STRENGTH BY MONITORING CONCRETE TEMPERATURE USING SENSOR
// More Info here: https://www.irjet.net/archives/V6/i3/IRJET-V6I3592.pdf
// Maturity Temperature Time Factor
// TTF = ∑ (Ta – T0) ∆t

/* Where,
     TTF - temperature-time factor, degree hours
     Ta - average concrete temperature during each time interval, °C
     T0 - datum temperature,-10°C
     ∆t - time interval, hours
*/

/* EXPRESSION FOR STRENGTH
     Expression for calculating strength
        MR =Su.e- ( Ʈ / TTF)^α   (Reference ASTM C1074
Where,
   TTF - Average TTF to date of calc. 
   MR - flexural strength or compressive strength
   Su - ultimate expected flexural strength,
   Ʈ - time coefficient
   α - shape parameter
*/

float init_temp=20;
float deltaTime=1.0; // 1h internval
float measure_temp[720];
float ttf[720];

float mr[720];
float su=30; // 30 Mpa
float time_coef ;
float shape_param;

int measure_counter=-1;

float avg_ttf(){
  float ttf=0.0;
  for(int i=0; i<measure_counter; i++){
    ttf=ttf+(measure_temp[i]-init_temp)*deltaTime;
  }
 return ttf;  
}

float strenght(){
  float mr=0.0;
  for(int i=0; i<measure_counter; i++){
    mr=su* pow(exp(-(time_coef-avg_ttf())),  shape_param);
  }
 return mr;    
}

void reset_data(){
  int size = sizeof(mr)/sizeof(mr[0]);
  for(int i=0; i<size; i++){
    mr[i]=0;
    measure_temp[i]=0;
    ttf[i]=0;
 }  
}
/********************************************************************/ 

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
float txValue = 0;
const int LED = 34; // Could be different depending on the dev board. I used the DOIT ESP32 dev board.

//std::string rxValue; // Could also make this a global var to access it in loop()

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");

        for (int i = 0; i < rxValue.length(); i++) {
           Serial.print(rxValue[i]); 
           } 
           Serial.println(); 
           // Do stuff based on the command received from the app 
           if (rxValue.find("ON") != -1) {
              Serial.println("Turning ON!");
               digitalWrite(LED, HIGH);
          } else if (rxValue.find("OFF") != -1) {
            Serial.println("Turning OFF!"); 
            digitalWrite(LED, LOW); 
          } 
          Serial.println(); 
          Serial.println("*********");
        } 
      } 
  }; 
                    
void setup() { 


  Serial.begin(115200); 
  delay(5000);
  Serial.print("Reset dataset");
  reset_data();
  Serial.println("DONE.");
   
  pinMode(LED, OUTPUT); 
  // Create the BLE Device 
  BLEDevice::init("Concrete Maturity");  // Give it a name 
  
  // Create the BLE Server 
  BLEServer *pServer = BLEDevice::createServer(); 
  pServer->setCallbacks(new MyServerCallbacks());
  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);
  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                      
  pCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");

   Serial.println("Dallas Temperature IC"); 
   // Start up the library 
   sensors.begin(); 
}

float roundFloat(float var) {
    // 37.66666 * 100 =3766.66
    // 3766.66 + .5 =3767.16    for rounding off value
    // then type cast to int so value is 3767
    // then divided by 100 so the value converted into 37.67
    float value = (int)(var * 100 + .5);
    return (float)value / 100;
}


void loop() {
     // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus 
  /********************************************************************/
  Serial.print(" Requesting temperatures..."); 
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  Serial.println("DONE"); 
 /********************************************************************/

  measure_counter =measure_counter+1;
  int size = sizeof(mr)/sizeof(mr[0]);
  if(measure_counter> size){
    measure_counter=0;
  }
  
  Serial.println("Temperature("+String(measure_counter)+") is: "); 
  measure_temp[measure_counter]=sensors.getTempCByIndex(0);
  Serial.println(measure_temp[measure_counter]);
  Serial.println("DONE");
  txValue=measure_temp[measure_counter];
  Serial.println("Conv DONE");
  
  if (deviceConnected) {
    Serial.println("Device conencted...");
    String dataStr =  "T:"+String(roundFloat(measure_temp[measure_counter]))+" M:"+String(roundFloat(avg_ttf()))+"\n";
    Serial.println("DataTX size="+String(dataStr.length()));  
    
    Serial.print("*** Sent Value (STR): ");
    Serial.print(dataStr);
    Serial.println(" ***");
    
    const char* txString = dataStr.c_str();
        
    Serial.print("*** Sent Value(CHAR): ");
    Serial.print(txString);
    Serial.println(" ***");

    //    pCharacteristic->setValue(&txValue, 1); // To send the integer value
    //    pCharacteristic->setValue("Hello!"); // Sending a test message
    pCharacteristic->setValue(txString);
    
    pCharacteristic->notify(); // Send the value to the app!

    // You can add the rxValue checks down here instead
    // if you set "rxValue" as a global var at the top!
    // Note you will have to delete "std::string" declaration
    // of "rxValue" in the callback function.
//    if (rxValue.find("ON") != -1) {
//      Serial.println("Turning ON!");
//      digitalWrite(LED, HIGH);
//    }
//    else if (rxValue.find("OFF") != -1) {
//      Serial.println("Turning OFF!");
//      digitalWrite(LED, LOW);
//    }
  }
  delay(1000);
}
