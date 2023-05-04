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

#include "dataverse.h"
#include <Arduino.h>
#include "manage_mcu_freq.h"

DATAVERSE_CLASS::DATAVERSE_CLASS(){

}

void DATAVERSE_CLASS::init(INTERFACE_CLASS* interface, M_WIFI_CLASS* mWifi, mSerial* mserial){
    this->mserial=mserial;
    this->mserial->printStr("init dataverse ...");
    this->interface=interface;
    this->mWifi= mWifi;

    this->config.API_TOKEN = "a85f5973-34dc-4133-a32b-c70dfef9d001";
    this->config.API_TOKEN_EXPIRATION_DATE="";

    this->config.SERVER_URL = "dataverse.harvard.edu";
    this->config.SERVER_PORT = 443;
    this->config.PERSISTENT_ID = "doi:10.7910/DVN/BTFHXT"; 
    this->config.DATASET_ID = "6987017";
    this->config.UPLOAD_DATASET_DELTA_TIME= 10*60*1000; // 10 min       
    this->config.UPLOAD_DATAVERSE_EN=true;

    this->DATASET_REPOSITORY_URL="";
    this->$espunixtimePrev= millis();
  
  // ToDo: load Dataverse settings
    this->ErrMsgShown = false;

    this->mserial->printStrln("done.");
}
    
// *********************************************************

bool DATAVERSE_CLASS::saveSettings(fs::FS &fs){
  File settingsFile = fs.open(F("/dataverse.cfg"), "w"); 
  if (!settingsFile)
    return false;

  settingsFile.write((byte *)&this->config, sizeof(this->config));
  settingsFile.close();
  return true;
}

bool DATAVERSE_CLASS::readSettings(fs::FS &fs){
  File settingsFile = fs.open("/dataverse.cfg", FILE_WRITE);
  if (!settingsFile)
    return false;

  settingsFile.read((byte *)&this->config, sizeof(this->config));
  settingsFile.close();
  return true;
}


// *********************************************************
//            Upload Dataset to Harvard's Dataverse
// *********************************************************
void DATAVERSE_CLASS::UploadToDataverse(bool ble_connected) {

  unsigned long  $timedif;


  if (this->config.UPLOAD_DATAVERSE_EN==false || this->interface->Measurments_EN==false)
      return;

  if ( ( millis() - this->$espunixtimePrev) < this->config.UPLOAD_DATASET_DELTA_TIME )
    return;
    

    if (this->interface->CURRENT_CLOCK_FREQUENCY <= this->interface->WIFI_FREQUENCY )
      changeMcuFreq(this->interface, this->interface->WIFI_FREQUENCY);
    
    if (WiFi.status() != WL_CONNECTED){
      this->mWifi->start(10000, 5); // TTL , n attempts 
      this->mWifi->updateInternetTime();
    }
    if (WiFi.status() != WL_CONNECTED ){
      if (this->ErrMsgShown == false){
        this->ErrMsgShown = true;
        this->mserial->printStrln("DATAVERSE: unable to connect to WIFI.");
    }
      return;
    }
  this->$espunixtimePrev= millis();

  interface->onBoardLED->led[0] = interface->onBoardLED->LED_BLUE;
  interface->onBoardLED->statusLED(100, 0);

  String tmp;
  char* tmpChr;
  JsonObject datasetObject = this->datasetInfoJson[0];
  boolean uploadStatusNotOK;

  if(datasetObject["data"].containsKey("id")){
    if(datasetObject["data"]["id"] != ""){
     tmp = String(datasetObject["data"]["id"].as<char*>());
      String rawResponse = GetInfoFromDataverse("/api/datasets/"+ tmp +"/locks");
      const size_t capacity =2*rawResponse.length() + JSON_ARRAY_SIZE(1) + 7*JSON_OBJECT_SIZE(1);
      DynamicJsonDocument datasetLocksJson(capacity);  
      // Parse JSON object
      DeserializationError error = deserializeJson(datasetLocksJson, rawResponse);
      if (error) {
        if (this->ErrMsgShown == false){
          this->ErrMsgShown = true;
          this->mserial->printStr("unable to retrive dataset lock status. Upload not possible. ERR: "+ String(error.f_str()));
        }
        //this->mserial->printStrln(rawResponse);
        interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
        interface->onBoardLED->statusLED(100, 5);
        this->mWifi->resumeStandbyMode();
        return;
      }else{
         String stat = datasetObject["status"];
         if(datasetObject.containsKey("lockType")){
           String locktype = String(datasetObject["data"]["lockType"].as<char*>());           
           this->mserial->printStrln("There is a Lock on the dataset: "+ locktype); 
           this->mserial->printStrln("Upload of most recent data is not possible without removal of the lock.");  
           // Do unlocking 
                
         }else{
            if (this->ErrMsgShown == false){
              this->ErrMsgShown = true;
              this->mserial->printStrln("The dataset is unlocked. Upload possible.");
            }
            interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
            interface->onBoardLED->statusLED(100, 5);
            this->mWifi->resumeStandbyMode();
            return;
        }
      }
    }else{
      if (this->ErrMsgShown == false){
        this->ErrMsgShown = true;
        this->mserial->printStrln("dataset ID is empty. Upload not possible. ");
      }
      interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
      interface->onBoardLED->statusLED(100, 5); 
      this->mWifi->resumeStandbyMode();
      return;
    }
  }else{
    if (this->ErrMsgShown == false){
      this->ErrMsgShown = true;
      this->mserial->printStrln("dataset metadata not loaded. Upload not possible. ");
    }
    interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
    interface->onBoardLED->statusLED(100, 5); 
    this->mWifi->resumeStandbyMode();
    return;
  }

  // Open the dataset file and prepare for binary upload
  File datasetFile = FFat.open("/"+ this->interface->config.SENSOR_DATA_FILENAME, FILE_READ);
  if (!datasetFile){
    if (this->ErrMsgShown == false){
      this->ErrMsgShown = true;
      this->mserial->printStrln("Dataset file not found");
    }
    interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
    interface->onBoardLED->statusLED(100, 5);
    this->mWifi->resumeStandbyMode();
    return;
  }
    
  String boundary = "7MA4YWxkTrZu0gW";
  String contentType = "text/csv";
  this->DATASET_REPOSITORY_URL =  "/api/datasets/:persistentId/add?persistentId="+this->config.PERSISTENT_ID;
  
  String datasetFileName = datasetFile.name();
  String datasetFileSize = String(datasetFile.size());
  this->mserial->printStrln("Dataset File Details:");
  this->mserial->printStrln("Filename:" + datasetFileName);
  this->mserial->printStrln("size (bytes): "+ datasetFileSize);
  this->mserial->printStrln("");
    
  int str_len = this->config.SERVER_URL.length() + 1; // Length (with one extra character for the null terminator)
  char SERVER_URL_char [str_len];    // Prepare the character array (the buffer) 
  this->config.SERVER_URL.toCharArray(SERVER_URL_char, str_len);    // Copy it over 
    
  this->mWifi->client.stop();
  this->mWifi->client.setCACert(HARVARD_ROOT_CA_RSA_SHA1);
  if (!this->mWifi->client.connect(SERVER_URL_char, this->config.SERVER_PORT)) {
        if (this->ErrMsgShown == false){
          this->ErrMsgShown = true;
          this->mserial->printStrln("Cloud server URL connection FAILED!");
          this->mserial->printStrln(SERVER_URL_char);
          int server_status = this->mWifi->client.connected();
          this->mserial->printStrln("Server status code: " + String(server_status));
        }
      interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
      interface->onBoardLED->statusLED(100, 5);
      this->mWifi->resumeStandbyMode();
      return;
  }
  this->mserial->printStrln("Connected to the dataverse of Harvard University"); 
  this->mserial->printStrln("");

  this->mserial->printStr("Requesting URL: " + this->DATASET_REPOSITORY_URL);

  // Make a HTTP request and add HTTP headers    
  String postHeader = "POST " + this->DATASET_REPOSITORY_URL + " HTTP/1.1\r\n";
  postHeader += "Host: " + this->config.SERVER_URL + ":" + String(this->config.SERVER_PORT) + "\r\n";
  postHeader += "X-Dataverse-key: " + this->config.API_TOKEN + "\r\n";
  postHeader += "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n";
  postHeader += "Accept: text/html,application/xhtml+xml,application/xml,application/json;q=0.9,*/*;q=0.8\r\n";
  postHeader += "Accept-Encoding: gzip,deflate\r\n";
  postHeader += "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n";
  postHeader += "User-Agent: AeonLabs LDAD Smart DAQ device\r\n";
  postHeader += "Keep-Alive: 300\r\n";
  postHeader += "Connection: keep-alive\r\n";
  postHeader += "Accept-Language: en-us\r\n";

  // jsonData header
  String jsonData = "{\"description\":\"LIVE Experimental data upload from LDAD Smart 12bit DAQ \",\"categories\":[\"Data\"], \"restrict\":\"false\", \"tabIngest\":\"false\"}";     
  String jsonDataHeader = "--" + boundary + "\r\n";
  jsonDataHeader += "Content-Disposition: form-data; name=\"jsonData\"\r\n\r\n";
  jsonDataHeader += jsonData+"\r\n";

  // dataset header
  String datasetHead = "--" + boundary + "\r\n";
  datasetHead += "Content-Disposition: form-data; name=\"file\"; filename=\"" + datasetFileName + "\"\r\n";
  datasetHead += "Content-Type: " + contentType + "\r\n\r\n";

  // request tail
  String tail = "\r\n--" + boundary + "--\r\n\r\n";

  // content length
  int contentLength = jsonDataHeader.length() + datasetHead.length() + datasetFile.size() + tail.length();
  postHeader += "Content-Length: " + String(contentLength, DEC) + "\n\n";
  
  // send post header
  int postHeader_len=postHeader.length() + 1; 
  char charBuf0[postHeader_len];
  postHeader.toCharArray(charBuf0, postHeader_len);
  this->mWifi->client.print(charBuf0);
  this->mserial->printStr(charBuf0);

  // send key header
  char charBufKey[jsonDataHeader.length() + 1];
  jsonDataHeader.toCharArray(charBufKey, jsonDataHeader.length() + 1);
  this->mWifi->client.print(charBufKey);
  this->mserial->printStr(charBufKey);

  // send request buffer
  char charBuf1[datasetHead.length() + 1];
  datasetHead.toCharArray(charBuf1, datasetHead.length() + 1);
  this->mWifi->client.print(charBuf1);
  this->mserial->printStr(charBuf1);

  // create buffer
  const int bufSize = 2048;
  byte clientBuf[bufSize];
  int clientCount = 0;

  while (datasetFile.available()) {
    clientBuf[clientCount] = datasetFile.read();
    clientCount++;
    if (clientCount > (bufSize - 1)) {
        this->mWifi->client.write((const uint8_t *)clientBuf, bufSize);
        clientCount = 0;
    }
  }
  datasetFile.close();
  if (clientCount > 0) {
      this->mWifi->client.write((const uint8_t *)clientBuf, clientCount);
      this->mserial->printStrln("[binary data]");
  }

  // send tail
  char charBuf3[tail.length() + 1];
  tail.toCharArray(charBuf3, tail.length() + 1);
  this->mWifi->client.print(charBuf3);
  this->mserial->printStr(charBuf3);

  // Read all the lines on reply back from server and print them to mserial
  this->mserial->printStrln("");
  this->mserial->printStrln("Response Headers:");
  String responseHeaders = "";

  while (this->mWifi->client.connected()) {
    // this->mserial->printStrln("while this->mWifi.client connected");
    responseHeaders = this->mWifi->client.readStringUntil('\n');
    this->mserial->printStrln(responseHeaders);
    if (responseHeaders == "\r") {
      this->mserial->printStrln("======   end of headers ======");
      break;
    }
  }

  String responseContent = this->mWifi->client.readStringUntil('\n');
  this->mserial->printStrln("Harvard University's Dataverse reply was:");
  this->mserial->printStrln("==========");
  this->mserial->printStrln(responseContent);
  this->mserial->printStrln("==========");
  this->mserial->printStrln("closing connection");
  this->mWifi->client.stop();

  interface->onBoardLED->led[0] = interface->onBoardLED->LED_GREEN;
  interface->onBoardLED->statusLED(100, 2);
}

// *********************************************************
//            Make data request to Dataverse
// *********************************************************
String DATAVERSE_CLASS::GetInfoFromDataverse(String url) {
            
  int str_len = this->config.SERVER_URL.length() + 1; // Length (with one extra character for the null terminator)
  char SERVER_URL_char [str_len];    // Prepare the character array (the buffer) 
  this->config.SERVER_URL.toCharArray(SERVER_URL_char, str_len);    // Copy it over 
    
  this->mWifi->client.stop();
  this->mWifi->client.setCACert(HARVARD_ROOT_CA_RSA_SHA1);
    
  if (!this->mWifi->client.connect(SERVER_URL_char, this->config.SERVER_PORT)) {
      if (this->ErrMsgShown == false){
        this->ErrMsgShown = true;
        this->mserial->printStrln("Cloud server URL connection FAILED!");
        this->mserial->printStrln(SERVER_URL_char);
        int server_status = this->mWifi->client.connected();
        this->mserial->printStrln("Server status code: " + String(server_status));
      }
      this->mWifi->resumeStandbyMode();
      return "";
  }
  
  this->mserial->printStrln("Connected to the dataverse of Harvard University"); 
  this->mserial->printStrln("");
  
  // We now create a URI for the request
  this->mserial->printStr("Requesting URL: ");
  this->mserial->printStrln(url);

  // Make a HTTP request and add HTTP headers    
  // post header
  String postHeader = "GET " + url + " HTTP/1.1\r\n";
  postHeader += "Host: " + this->config.SERVER_URL + ":" + String(this->config.SERVER_PORT) + "\r\n";
  //postHeader += "X-Dataverse-key: " + API_TOKEN + "\r\n";
  postHeader += "Content-Type: text/json\r\n";
  postHeader += "Accept: text/html,application/xhtml+xml,application/xml,application/json,text/json;q=0.9,*/*;q=0.8\r\n";
  postHeader += "Accept-Encoding: gzip,deflate\r\n";
  postHeader += "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n";
  postHeader += "User-Agent: AeonLabs LDAD Smart DAQ device\r\n";
  //postHeader += "Keep-Alive: 300\r\n";
  postHeader += "Accept-Language: en-us\r\n";
  postHeader += "Connection: close\r\n\r\n";

  // request tail
  String boundary = "7MA4YWxkTrZu0gW";
  String tail = "\r\n--" + boundary + "--\r\n\r\n";

  // content length
  int contentLength = tail.length();
  //postHeader += "Content-Length: " + String(contentLength, DEC) + "\n\n";
  
  // send post header
  int postHeader_len=postHeader.length() + 1; 
  char charBuf0[postHeader_len];
  postHeader.toCharArray(charBuf0, postHeader_len);
  this->mWifi->client.print(charBuf0);
  
  this->mserial->printStrln("======= Headers =======");
  this->mserial->printStrln(charBuf0);
  this->mserial->printStrln("======= End of Headers =======");
  
  // Read all the lines of the reply from server and print them to mserial
  String responseHeaders = "";

  this->mserial->printStr("Waiting for server response...");
  long timeout= millis();
  long ttl=millis()-timeout;
  while (this->mWifi->client.connected() && abs(ttl) < this->mWifi->HTTP_TTL) {
    ttl=millis()-timeout;      
    responseHeaders = this->mWifi->client.readStringUntil('\n');
    if (responseHeaders == "\r") {
      break;
    }
  }
  if (abs(ttl) < 10000){
    this->mserial->printStrln("OK");
  }else{
    this->mserial->printStrln("timed out.");  
  }
  
  String responseContent = this->mWifi->client.readStringUntil('\n');
  this->mWifi->client.stop();
  
  this->mWifi->resumeStandbyMode();
  
  return responseContent;
  this->ErrMsgShown = false;
  }


// *********************************************************
//            Get dataset metadata
// *********************************************************

void DATAVERSE_CLASS::getDatasetMetadata(){
  String rawResponse = GetInfoFromDataverse("/api/datasets/" +  this->config.DATASET_ID + "/locks");
  this->mserial->printStr("getDatasetMetadata json ini");
  // Parse JSON object
  DeserializationError error = deserializeJson(this->datasetInfoJson, rawResponse);
  if (error) {
    this->mserial->printStr("Get Info From Dataverse deserializeJson() failed: ");
    this->mserial->printStrln(error.f_str());
    this->mserial->printStrln("==== raw response ====");
    this->mserial->printStr(rawResponse);  
    this->mserial->printStr("========================");
    return;
  }else{
     String stat = this->datasetInfoJson["status"];
     this->mserial->printStrln("Status: "+ stat);   
     if(stat=="ERROR"){
       String code = this->datasetInfoJson["code"];
       String msg = this->datasetInfoJson["message"];
       this->mserial->printStrln("code : " + code);
       this->mserial->printStrln("message : " + msg);            
     }else{
        this->mserial->printStrln("The dataset is unlocked");     
     }
     
    String id = this->datasetInfoJson["data"]["id"];
    this->mserial->printStrln("DATASET ID: " + id);
  }
}

// ****************************************************
// *****************************************************
bool DATAVERSE_CLASS::gbrl_commands(String $BLE_CMD , uint8_t sendTo){
    String dataStr="";

    if($BLE_CMD=="$dv server"){
        dataStr="Dataverse Server Address is: "+ String(this->config.SERVER_URL) +"\n";
        dataStr +=   "\n";
        this->interface->sendBLEstring( dataStr);
        return true;
    }

    if($BLE_CMD=="$dv port"){
        dataStr="Dataverse Server port is: "+ String(this->config.SERVER_PORT) +"\n";
        dataStr +=   "\n";
        this->interface->sendBLEstring( dataStr);
        return true;
    }

    if($BLE_CMD=="$dv doi"){
        dataStr="Dataset Persistant ID is: "+ String(this->config.PERSISTENT_ID) +"\n";
        dataStr +=   "\n";
        this->interface->sendBLEstring( dataStr);
        return true;
    }

    if($BLE_CMD=="$dv doi"){
        dataStr="Dataset ID is: "+ String(this->config.DATASET_ID) +"\n";
        dataStr +=   "\n";
        this->interface->sendBLEstring( dataStr);
        return true;
    }

    if($BLE_CMD=="$dv doi"){
        dataStr="Dataset Dataset Upload Interval: "+ String(this->config.UPLOAD_DATASET_DELTA_TIME) +" (min)\n";
        dataStr +=   "\n";
        this->interface->sendBLEstring( dataStr);
        return true;
    }

    if($BLE_CMD=="$dv doi"){
        dataStr="Dataset ID is: "+ String(this->config.DATASET_ID) +"\n";
        dataStr +=   "\n";
        this->interface->sendBLEstring( dataStr);
        return true;
    }

    return this->helpCommands( $BLE_CMD, sendTo);
}

// ****************************************************

 bool DATAVERSE_CLASS::helpCommands(String $BLE_CMD , uint8_t sendTo){
    String dataStr="Dataverse Commands: (not yet fully implemented on this firmware version)\n" \
                    "$dv server                     - View Dataverse Server Address\n" \
                    "$dv set server [server url]    - Set a new Dataverse Server Address\n" \
                    "$dv port                       - View Dataverse Server Port\n" \
                    "$dv set port [port]            - Set a new Dataverse Server Port\n" \
                    "$dv doi                        - View Dataset Persistant ID\n" \
                    "$dv set doi [persistant ID]    - Set a new Dataset Persistant ID\n" \
                    "$dv id                         - View Dataset ID\n" \
                    "$dv set ui [time in min]       - Set a new upload Interval\n" \
                    "$dv ui                         - View Dataset Upload Interval in min\n" \
                    "$dv set id [id]                - Set a new Dataset ID\n" \
                    "$dv token                      - View Dataverse API TOKEN\n" \
                    "$dv set token                  - Set a new Dataverse API TOKEN\n\n";

    this->interface->sendBLEstring( dataStr); 
    return false;
 }

// -------------------------------------------------------------------------------