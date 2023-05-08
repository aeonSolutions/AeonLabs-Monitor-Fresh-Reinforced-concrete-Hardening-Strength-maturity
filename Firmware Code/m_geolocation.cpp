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

#include <ArduinoJson.h>
#include "m_geolocation.h"
#include <WiFiMulti.h>
#include "HTTPClient.h"
#include "manage_mcu_freq.h"


GEO_LOCATION_CLASS::GEO_LOCATION_CLASS(){
}

// **********************************************
void GEO_LOCATION_CLASS::init(INTERFACE_CLASS* interface, mSerial* mserial ){
  this->mserial=mserial;
  this->mserial->printStr("init GeoLocation ...");
  this->interface=interface;
  this->ErrMsgShown=false;
  
  this->REQUEST_DELTA_TIME  = 10*60*1000; // 10 min 
  this->$espunixtimePrev= millis(); 
  this->mserial->printStrln("done.");
}

// *************************************************************************
bool GEO_LOCATION_CLASS::get_ip_address(){
  if(WiFi.status() != WL_CONNECTED)
    return false;

  HTTPClient http;

  String serverPath = "http://api.ipify.org";
  this->mserial->printStr("request external IP address...");
  http.begin(serverPath.c_str());      
  // Send HTTP GET request
  int httpResponseCode = http.GET();
  if (httpResponseCode>0) {
    this->mserial->printStrln("done (" + String(httpResponseCode) + ")." );
    if (httpResponseCode == 200){
      this->interface->InternetIPaddress = http.getString();
      http.end();
      return true;
    }else{
      this->mserial->printStrln("Error retrieving External IP address");
    }
  }else {
    this->mserial->printStrln("response code error: " + String(httpResponseCode) );
  }
  // Free resources
  http.end();
  return false;
}

// *************************************************************************
bool GEO_LOCATION_CLASS::get_ip_geo_location_data(String ipAddress , bool override ){

  if ( override==false && ( ( millis() - this->$espunixtimePrev) < this->REQUEST_DELTA_TIME) )
    return false;
  
  if (WiFi.status() != WL_CONNECTED ){
    if (this->ErrMsgShown == false){
      this->ErrMsgShown = true;
      this->mserial->printStrln("GEO Location: unable to connect to WIFI.");
      this->interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
      this->interface->onBoardLED->statusLED(100, 1);
    }
    return false;
  }

  this->ErrMsgShown = false;
  this->$espunixtimePrev= millis();
  
  if(WiFi.status() != WL_CONNECTED)
    return false;

  HTTPClient http;
  String serverPath = "http://ip-api.com/json/";
  if (ipAddress==""){
    if (this->interface->InternetIPaddress==""){
      if ( false == this->get_ip_address() )
        return false;
    }
    serverPath += this->interface->InternetIPaddress;
  }else{
      serverPath += ipAddress;
  }

  this->mserial->printStrln( "requesting Geo Location data... one moment.\n" ); 
  
  http.begin(serverPath.c_str());      
  // Send HTTP GET request
  int httpResponseCode = http.GET();
      
  if (httpResponseCode<1 && httpResponseCode != 200 ) {
    if (this->ErrMsgShown == false){
      this->ErrMsgShown = true;
      this->mserial->printStrln("Http error " + String(httpResponseCode) );
    }

    return false;
  }
  
  String JSONpayload = http.getString();
     
  // Parse JSON object
        /*
      {
          "status":"success",
          "country":"Belgium",
          "countryCode":"BE",
          "region":"BRU",
          "regionName":"Brussels Capital",
          "city":"Brussels",
          "zip":"1000",
          "lat":50.8534,
          "lon":4.347,
          "timezone":"Europe/Brussels",
          "isp":"PROXIMUS",
          "org":"",
          "as":"AS5432 Proximus NV",
          "query":"37.62.11.2"
      }
      String stat = this->datasetInfoJson["status"];
      */

  DeserializationError error = deserializeJson(this->interface->geoLocationInfoJson, JSONpayload);
  if (error) {
      this->mserial->printStrln("Error deserializing JSON");
      
      interface->onBoardLED->led[0] = interface->onBoardLED->LED_RED;
      interface->onBoardLED->statusLED(100, 1);
      return false;
  }else{
    this->interface->requestGeoLocationDateTime= String( this->interface->rtc.getDateTime(true) );
  }
  // Free resources
  http.end();

  this->ErrMsgShown = false;
  interface->onBoardLED->led[0] = interface->onBoardLED->LED_GREEN;
  interface->onBoardLED->statusLED(100, 1);
  return true;
}

// *********************************************************
 bool GEO_LOCATION_CLASS::helpCommands(uint8_t sendTo ){
    String dataStr="\n" \
                    "$geo info                          - View Geo-location data information (requires a WIFI connection)\n\n";

    this->interface->sendBLEstring( dataStr,  sendTo ); 
    
    return false; 
 }
 // *********************************************************
bool GEO_LOCATION_CLASS::gbrl_commands(String $BLE_CMD, uint8_t sendTo ){
  String dataStr="";
  
  if($BLE_CMD=="$?"){
    return this->helpCommands(sendTo);
  }

  if($BLE_CMD=="$geo info"){

    if ( false == this->get_ip_geo_location_data( "", true ) ){
      return true;
    }

    dataStr="\nGeoLocation Data:\n";
    dataStr += "Internet I.P. address: " + this->interface->InternetIPaddress + "\n";
    dataStr += "Time of last request: " + this->interface->requestGeoLocationDateTime + "\n";
    
    if ( this->interface->geoLocationInfoJson.isNull() == true ){
      dataStr="NULL geoLocation data.\n";
      this->interface->sendBLEstring( dataStr,  sendTo ); 
      return true;
    }

    if(this->interface->geoLocationInfoJson.containsKey("lat")){
      float lat = this->interface->geoLocationInfoJson["lat"];
      dataStr += "Latitude: "+ String(lat,4) + "\n";
    }
    if(this->interface->geoLocationInfoJson.containsKey("lon")){
      float lon = this->interface->geoLocationInfoJson["lon"];
      dataStr += "Longitude: "+ String(lon,4) + "\n";
    }

    if(this->interface->geoLocationInfoJson.containsKey("regionName"))
      dataStr += String(this->interface->geoLocationInfoJson["regionName"].as<char*>()) + ", ";       
    
    if(this->interface->geoLocationInfoJson.containsKey("country"))
      dataStr += String(this->interface->geoLocationInfoJson["country"].as<char*>()); 
    
    if(this->interface->geoLocationInfoJson.containsKey("countryCode"))
      dataStr += "(" + String(this->interface->geoLocationInfoJson["countryCode"].as<char*>()) + ")\n\n"; 

    this->interface->sendBLEstring( dataStr,  sendTo ); 
    return true;
  }
  
  return false;
}
