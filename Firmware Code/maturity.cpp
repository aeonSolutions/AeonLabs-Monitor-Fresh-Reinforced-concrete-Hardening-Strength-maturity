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

#include "maturity.h"
#include "Arduino.h"
#include "interface_class.h"
#include "m_math.h"
#include "onboard_sensors.h"
#include "FS.h"
#include <LittleFS.h>
// unique figerprint data ID 
#include "m_atsha204.h"
#include "manage_mcu_freq.h"
#include "m_file_functions.h"

MATURITY_CLASS::MATURITY_CLASS(){}

void MATURITY_CLASS::init(INTERFACE_CLASS* interface, ONBOARD_SENSORS* onBoardSensors, mSerial* mserial, ONBOARD_LED_CLASS* onboardLED){

    this->mserial= mserial;
    this->interface=interface;
    this->onBoardSensors=onBoardSensors;
    this->onboardLED = onboardLED;

    this->mserial->printStr( this->interface->DeviceTranslation("mat_sensor_init") + " " + String(this->EXT_PLUG_DI_IO) + "...");



    this->onewire = OneWire(this->EXT_PLUG_DI_IO); 
    this->sensors= DallasTemperature(&onewire);    
    this->sensors.begin();    // initialize the DS18B20 sensor
    
    this->last_measured_probe_temp=-1;
    this->last_measured_time_delta=-1;     
    this->DATASET_NUM_SAMPLES=0;
    
    this->Measurments_EN = false;
    this->Measurments_NEW=false;
    this->measurement_Start_Time = "-1";

    this->interface->config.POWER_PLUG_ALWAYS_ON=false;
    
    this->config.MEASUREMENT_INTERVAL = (1*60*1000); //  mili sec
        
    this->interface->POWER_PLUG_IS_ON=false;

    this->config.custom_maturity_equation_is_summation=true;
    this->hasNewMeasurementValues=false;
    this->currentDatasetPos=-1;

    this->config.custom_maturity_equation="(p-(-10))*dt";
    this->config.custom_strenght_equation="32*log(m)+10";

    for(int i=0; i<8 ; i++){
        this->measureValues[i]=0.0;
    }

    // {"a","h","pt","dt","p","m","t","u"};
    this->var[0] ="a"; 
    this->var[1] ="h"; 
    this->var[2] ="pt"; 
    this->var[3] ="dt"; 
    this->var[4] ="p"; 
    this->var[5] ="m"; 
    this->var[6] ="t"; 
    this->var[7] ="u";

    // {"*","(", ")","+","=","-","/","%","^"};
    
    this->signs[0] = "*";
    this->signs[1] = "(";
    this->signs[2] = ")";
    this->signs[3] = "+";
    this->signs[4] = "=";
    this->signs[5] = "-";
    this->signs[6] = "/";
    this->signs[7] = "%";
    this->signs[8] = "^";

    this->readSettings();
    this->mserial->printStrln("done.\n");

    this->ProbeSensorStatus(mSerial::DEBUG_BOTH_USB_UART);
}

// ************************************************
void MATURITY_CLASS::ProbeSensorStatus(uint8_t sendTo){


    String dataStr="";
    dataStr= this->interface->DeviceTranslation("num_temp_probes") + " " + String(this->sensors.getDS18Count(), DEC) +"\n";

    // report parasite power requirements
    dataStr += this->interface->DeviceTranslation("parasite_power") + ": "; 
    if (this->sensors.isParasitePowerMode()) dataStr +="ON\n";
    else dataStr += "OFF\n";

    this->onewire.reset_search();
    // assigns the first address found to insideThermometer
    if ( !this->onewire.search(this->insideThermometer) ) dataStr += this->interface->DeviceTranslation("unable_find_probe") + " ?\n";


    // show the addresses we found on the bus
    dataStr += this->interface->DeviceTranslation("probe_addr") + ": ";
    for (uint8_t i = 0; i < 8; i++)
    {
    if (this->insideThermometer [i] < 16)  dataStr += "0";
        dataStr += String(this->insideThermometer[i], HEX) ;
    }

    // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
    this->sensors.setResolution(this->insideThermometer, 9);


    dataStr += "\n " +  this->interface->DeviceTranslation("probe_resolution") + ": ";
    dataStr += String(this->sensors.getResolution(this->insideThermometer), DEC) + "\n\n"; 
    
    this->interface->sendBLEstring( dataStr, sendTo);  
}

// ********************** for devices with onboard PSRAM *************************************************
int MATURITY_CLASS::get_dataset_memory_availability(){
    int long freeMem= esp_get_free_heap_size();


    this->mserial->printStrln( this->interface->BaseTranslation("free_mem") + ": " + String( freeMem ) + " bytes");
    int num_max_elements = ( freeMem / ( sizeof(float)*4 +  sizeof(long int) ) )/5;
    this->mserial->printStrln( this->interface->DeviceTranslation("max_dataset_size") + ": " + String( num_max_elements ));

    return num_max_elements;
}


// _______________________________________________________
int MATURITY_CLASS::get_dataset_size(){
    return this->DATASET_NUM_SAMPLES;
}


// ******************* Maturity Models ************************************
 void MATURITY_CLASS::get_dataset_values(int pos ){
    if (this->currentDatasetPos==pos){
        //this->mserial->printStrln("data values already in memory");
        return;
    }

    File file = LittleFS.open("/" + this->interface->config.SENSOR_DATA_FILENAME, "r");
    int counter=0; 
    
    //String var[8]= {"a","h","pt","dt","p","m","t","u"};
    //                 0   1    2    3   4   5   6   7

    String  valStr[8]= {"0.0","0.0","0.0","0.0","0.0","0.0","0.0","0.0"};

    while (file.available()) {
        if (counter == 0 ) {
            // raed the header
            String bin = file.readStringUntil( char(10) );
            String bin2 = file.readStringUntil( char(10) ); 
        }
        
        if (counter < pos){
            file.readStringUntil( char(10) );
            counter++;    
        
        }else{
            String bin = file.readStringUntil( ';' ); // RTC Date & Time

            long int timeStart = atol(file.readStringUntil( ';' ).c_str() ); // start time 
            long int timeElapsed = atol(file.readStringUntil( ';' ).c_str() );  // time elapsed sincce previous measurement time

            valStr[3] = String( timeElapsed ); // time delta = t since last  measure
            valStr[4] = file.readStringUntil( ';' ); // probe temp = p
            valStr[0] = file.readStringUntil( ';' );  // onboard temp = a
            valStr[1] = file.readStringUntil( ';' );  // onboard humidity = h
            valStr[7] = file.readStringUntil( ';' );  // mcu temp = = u

            this->measureValues[0] = (valStr[0].toDouble());    // onboard temp
            this->measureValues[1] = (valStr[1].toDouble());    // onboard humidity
            this->measureValues[2] = 0.0;
            this->measureValues[3] = (valStr[3].toDouble())/(3600*1000);    // time delta in hourss; stored value in miliseconds
            this->measureValues[4] = (valStr[4].toDouble());    // probe temp
            this->measureValues[5] = 0.0;
            this->measureValues[6] = 0.0;
            this->measureValues[7] = (valStr[7].toDouble());    // mcu temp

            break;
        } 
    }
    this->currentDatasetPos=pos;
    file.close();
}

uint8_t  MATURITY_CLASS::get_number_model_vars(String equation){
    uint8_t count =0;

    //String var[8]= {"a","h","pt","dt","p","m","t","u"};
    //                 0   1    2    3   4   5   6   7

    for(int i=0; i< 8; i++){
        uint8_t idx = equation.indexOf(this->var[i]);
        bool isSignLeft=false;
        bool isSignRight=false;
        
        if (idx>-1 && idx !=255){
            for(int j=0; j< 8; j++){
                
                if(idx>0){
                    if (String(this->signs[j]) == String(equation[idx-1]))
                        isSignLeft=true;
                }else{
                    isSignLeft=true;
                }

                if( (idx + this->signs[j].length()) < (equation.length() - 1 ) ){
                    if (String(this->signs[j]) == String(equation[idx + this->signs[j].length()]))
                        isSignRight=true;
                }else{
                    isSignRight=true;
                }
            }
            
            if (isSignRight && isSignLeft){
                count++;
            }
        }
    } //end for

    return count;
}

// ******************* Maturity Models ************************************
double MATURITY_CLASS::custom_maturity(int pos){
    
    //String var[8]= {"a","h","pt","dt","p","m","t","u"};
    //                 0   1    2    3   4   5   6   7

    te_variable vars[]= { 
                            {.name="a", .address = this->measureValues + 0},  //This assumes that measureValues is double array!!
                            {.name="h", .address = this->measureValues + 1},  
                            {.name="pt", .address = this->measureValues + 2},  
                            {.name="dt", .address = this->measureValues + 3},  
                            {.name="p", .address = this->measureValues + 4},  
                            {.name="m", .address = this->measureValues + 5},  
                            {.name="t", .address = this->measureValues + 6},  
                            {.name="u", .address = this->measureValues + 7}
                        };
    const int numVars = 8; //This refers only to the number of entries in vars!!
 
    int err;
    te_expr *n = te_compile(this->config.custom_maturity_equation.c_str(), vars, numVars, &err);
    if (n) {
        double result=0.0;
        for (int i=0; i<(pos+1); i++){
            get_dataset_values(i);
            result += (double) te_eval(n); 
        }

        te_free(n);
        return result;
    } else {
        this->mserial->printStrln(this->config.custom_maturity_equation + " << " + this->interface->BaseTranslation("error_near_pos")  + " " + String(err-1) , this->mserial->DEBUG_TYPE_ERRORS);
        return (0.0/0.0);
    }
}


// ***************************** Strenght Models ********************************************

double MATURITY_CLASS::custom_strenght(int pos){
    //String var[8]= {"a","h","pt","dt","p","m","t","u"};
    //                 0   1    2    3   4   5   6   7

    te_variable vars[]= { 
                            {.name="a", .address = this->measureValues + 0},  //This assumes that measureValues is double array!!
                            {.name="h", .address = this->measureValues + 1},  
                            {.name="pt", .address = this->measureValues + 2},  
                            {.name="dt", .address = this->measureValues + 3},  
                            {.name="p", .address = this->measureValues + 4},  
                            {.name="m", .address = this->measureValues + 5},  
                            {.name="t", .address = this->measureValues + 6},  
                            {.name="u", .address = this->measureValues + 7}
                        };
    const int numVars = 8; //This refers only to the number of entries in vars!!

    int err;
    te_expr *n = te_compile(this->config.custom_strenght_equation.c_str(), vars, numVars, &err);

    if (n) {
        this->measureValues[5]= custom_maturity(pos);
        const double r = te_eval(n); 
        te_free(n);
        return (double) r;
    } else {
        this->mserial->printStrln(this->config.custom_strenght_equation + " << " + this->interface->BaseTranslation("error_near_pos") + " " + String(err-1)  , this->mserial->DEBUG_TYPE_ERRORS);
        return (0.0/0.0);
    }
}

 // *************************************************
 String MATURITY_CLASS::validateExpression(String expression){
    //String var[8]= {"a","h","pt","dt","p","m","t","u"};
    //                 0   1    2    3   4   5   6   7

    te_variable vars[]= { 
                            {.name="a", .address = this->measureValues + 0},  //This assumes that measureValues is double array!!
                            {.name="h", .address = this->measureValues + 1},  
                            {.name="pt", .address = this->measureValues + 2},  
                            {.name="dt", .address = this->measureValues + 3},  
                            {.name="p", .address = this->measureValues + 4},  
                            {.name="m", .address = this->measureValues + 5},  
                            {.name="t", .address = this->measureValues + 6},  
                            {.name="u", .address = this->measureValues + 7}
                        };
    const int numVars = 8; //This refers only to the number of entries in vars!!

    /* This will compile the expression and check for errors. */
    int err;
    te_expr *n = te_compile(expression.c_str(), vars, numVars, &err);

    if (n) {
        return "MODEL OK"; // dont change
    }else{
        return this->interface->DeviceTranslation("model_err_near") + " " + String(err-1);
    }
 }

 // **************************************************************
 // ****************************************************************
 void MATURITY_CLASS::request_measurment_data(){
    time_t timeNow;

    // GBRL start command not received
    if (this->Measurments_EN==false)
        return;
        
    // GBRL New command received
    if (this->Measurments_NEW){
        this->reinitialize_dataset_file(LittleFS);
        this->Measurments_NEW=false;
        this->interface->$espunixtimeStartMeasure=millis();
        this->prevTimeMeasured = millis();
        this->mserial->printStrln(" Measurements started.\n"); 
    }

    if ( ( millis() - this->prevTimeMeasured) < this->config.MEASUREMENT_INTERVAL ){ 
        return;
    }


    if(this->DATASET_NUM_SAMPLES<1){
        digitalWrite(this->interface->EXT_PLUG_PWR_EN ,HIGH);
        this->interface->POWER_PLUG_IS_ON=true;
        delay(1000);
    }
            
    // ToDo: time diff error 
    this->last_measured_time_delta = millis() - this->prevTimeMeasured;
    this->prevTimeMeasured = millis();



    this->mserial->printStrln("Delta Time = " +  addThousandSeparators( std::string( String(this->last_measured_time_delta).c_str() ) ) +" ms");
    
    this->mserial->printStr( this->interface->DeviceTranslation("probing_temp") + "..."); 
    //this->ProbeSensorStatus(mSerial::DEBUG_NONE);
    changeMcuFreq(this->interface, this->interface->WIFI_FREQUENCY);
    this->sensors.requestTemperatures(); // Send the command to get temperature readings 
    this->mserial->printStrln(this->interface->BaseTranslation("completed")); 

    this->DATASET_NUM_SAMPLES = this->DATASET_NUM_SAMPLES+1;
        
    this->last_measured_probe_temp =   this->sensors.getTempCByIndex(0);
    
    changeMcuFreq(this->interface, this->interface->SAMPLING_FREQUENCY);

    // this->sensors.getTempC( this->insideThermometer );
    if( this->last_measured_probe_temp == DEVICE_DISCONNECTED_C) 
    {
        this->mserial->printStrln( this->interface->DeviceTranslation("err_read_temp") + " ?");
    }

    this->onBoardSensors->request_onBoard_Sensor_Measurements();
    // Save data to the dataset file
    this->save_measurment_record(LittleFS);  

    // __________________________________________________________________________

    //Disable PWR on the PLUG
    if (this->interface->config.POWER_PLUG_ALWAYS_ON==false){
        digitalWrite(this->interface->EXT_PLUG_PWR_EN ,LOW);
        this->interface->POWER_PLUG_IS_ON=false;
    }
    this->hasNewMeasurementValues=true;
}

// -------------------------------------------------------------------------
bool MATURITY_CLASS::reinitialize_dataset_file(fs::FS &fs){
    this->readSettings(LittleFS);
    if (fs.exists( "/" +  this->interface->config.SENSOR_DATA_FILENAME ) ){
        if( fs.remove( "/" +  this->interface->config.SENSOR_DATA_FILENAME ) !=true ){
            this->mserial->printStrln( this->interface->DeviceTranslation("err_read_temp") , this->mserial->DEBUG_TYPE_ERRORS);
            return false;
        }  
    }
    File datasetFile = fs.open("/" +  this->interface->config.SENSOR_DATA_FILENAME , FILE_WRITE); 
    if (datasetFile){
        this->mserial->printStrln( this->interface->DeviceTranslation("reinit_dataset") + " [" + String(this->interface->config.SENSOR_DATA_FILENAME) +"]" );
        
        String dataStr="Local DateTime; Start Time(s); Time Delta (s);Probe Temp (*C); Onboard Temp(*C); Onboard Humidity(%); MCU Temp(*C); Onboard Motion X;Onboard Motion Y;Onboard Motion Z;";
        dataStr += "Onboard YAW motion X;Onboard YAW motion Y;Onboard YAW motion Z;";
        dataStr += "Unique FingerPrint ID (UFPID); Previous UFPID; Internet IP addr; GeoLocation Timestamp; Latitude; Longitude;" + String(char(10));
        dataStr+= "Start time(s):;" + String( this->measurement_Start_Time ) + String(char(10));
        datasetFile.print(dataStr);
        
        datasetFile.close();
        this->DATASET_NUM_SAMPLES =0;
        this->hasNewMeasurementValues=false;
        return true;
    }else{
        this->mserial->printStrln( this->interface->BaseTranslation("err_create_file") + " " + this->interface->config.SENSOR_DATA_FILENAME , this->mserial->DEBUG_TYPE_ERRORS);
        this->onboardLED->statusLED( (uint8_t*)(const uint8_t[]){this->onboardLED->LED_RED}, 100,5); 
        return false;
    }
}

// -----------------------------------------------------------------------------------
bool MATURITY_CLASS::save_measurment_record(fs::FS &fs){    
    File datasetFile = fs.open("/" +  this->interface->config.SENSOR_DATA_FILENAME , FILE_APPEND); 
    if (datasetFile){      
        String dataStr = String( interface->rtc.getDateTime(true) ) + ";" +  String( this->interface->$espunixtimeStartMeasure )  + ";";
        dataStr += String( this->last_measured_time_delta ) + ";" +  String(roundFloat(this->last_measured_probe_temp ,2)) + ";";
        dataStr += String(roundFloat(this->onBoardSensors->aht_temp, 2)) + ";" + String(roundFloat(this->onBoardSensors->aht_humidity,2)) + ";";  
        dataStr += String(roundFloat(this->onBoardSensors->LSM6DS3_TEMP, 2)) + ";";
        dataStr += String(roundFloat(this->onBoardSensors->LSM6DS3_Motion_X, 3)) + ";" + String(roundFloat(this->onBoardSensors->LSM6DS3_Motion_Y, 3)) + ";";
        dataStr += String(roundFloat(this->onBoardSensors->LSM6DS3_Motion_Z, 3)) + ";" + String(roundFloat(this->onBoardSensors->LSM6DS3_GYRO_X, 3)) + ";";
        dataStr += String(roundFloat(this->onBoardSensors->LSM6DS3_GYRO_Y, 3)) + ";" + String(roundFloat(this->onBoardSensors->LSM6DS3_GYRO_Z, 3))  + ";";

        String current_UFPID = macChallengeDataAuthenticity(this->interface, dataStr);
        dataStr+=CryptoICserialNumber(this->interface)+"-"+ current_UFPID;
 
         if(this->DATASET_NUM_SAMPLES==1){
            dataStr+=CryptoICserialNumber(this->interface)+"-"+macChallengeDataAuthenticity(this->interface, "begin of measurements") + ";";
        }else{
            dataStr+=this->previous_UFPID + ";";
        }
        this->previous_UFPID= current_UFPID;
        
        dataStr += this->interface->InternetIPaddress + ";";
        dataStr += this->interface->requestGeoLocationDateTime + ";";
        if ( this->interface->geoLocationInfoJson.isNull() == false ){
            if(this->interface->geoLocationInfoJson.containsKey("lat")){
                float lat = this->interface->geoLocationInfoJson["lat"];
                dataStr += String(lat,4) + ";";
            }else{
                dataStr += ";";
            }
            if(this->interface->geoLocationInfoJson.containsKey("lon")){
                float lon = this->interface->geoLocationInfoJson["lon"];
                dataStr += String(lon,4) + ";";
            }else{
                dataStr += ";";
            }
        }
        dataStr += String(char(10));

        // append data record to the datset file
        datasetFile.print(dataStr);
        datasetFile.close();
        return true;
    }else{
      this->mserial->printStrln( this->interface->BaseTranslation("err_open_file")  + "  " + this->interface->config.SENSOR_DATA_FILENAME , this->mserial->DEBUG_TYPE_ERRORS);
      this->onboardLED->statusLED( (uint8_t*)(const uint8_t[]){this->onboardLED->LED_RED}, 100,5); 
      return false;
    }
}


// --------------------------------------------------------------------------

bool MATURITY_CLASS::saveSettings(fs::FS &fs){
    this->mserial->printStrln( this->interface->DeviceTranslation("save_mat_settings")  + "...");

    if (fs.exists("/maturity.cfg") )
        fs.remove("/maturity.cfg");

    File settingsFile = fs.open("/storage/maturity.cfg", FILE_WRITE); 
    if ( !settingsFile ){
        this->mserial->printStrln( this->interface->DeviceTranslation("err_create_mat_settings") + ".");
        return false;
    }

    settingsFile.print( String(this->config.custom_maturity_equation_is_summation) + String(';'));
    settingsFile.print( this->config.custom_maturity_equation + String(';'));
    settingsFile.print( this->config.custom_strenght_equation + String(';'));
    settingsFile.print( String(this->config.MEASUREMENT_INTERVAL) + String(';'));

    settingsFile.close();
    return true;
}

bool MATURITY_CLASS::readSettings(fs::FS &fs){    
    File settingsFile = fs.open("/maturity.cfg", FILE_READ);
    if (!settingsFile){
        this->mserial->printStrln( this->interface->DeviceTranslation("err_notfound_mat_settings")  + ".");
        return false;
    }
    if (settingsFile.size() == 0){
        this->mserial->printStrln( this->interface->DeviceTranslation("err_invalid_mat_settings") + ".");
        return false;    
    }

    File settingsFile2 = fs.open("/maturity.cfg", FILE_READ);

    String temp= settingsFile.readStringUntil(';');
    this->config.custom_maturity_equation_is_summation = *(temp.c_str()) != '0';
  
    this->config.custom_maturity_equation = settingsFile.readStringUntil(';');
    this->config.custom_strenght_equation = settingsFile.readStringUntil(';');
    this->config.MEASUREMENT_INTERVAL = atol(settingsFile.readStringUntil( ';' ).c_str() ); 

    settingsFile.close();
    return true;
}

// -------------------------------------------------------------------------------

 bool MATURITY_CLASS::helpCommands(String $BLE_CMD, uint8_t sendTo){
    if($BLE_CMD != "$?" && $BLE_CMD !="$help" )
        return false;

    String dataStr="Concrete Curing GBRL Commands:\n" \
                    "$pt                 - "+ this->interface->DeviceTranslation("pr") +"\n" \
                    "$probe status       - "+ this->interface->DeviceTranslation("probe_status") +"\n" \
                    "$st                 - "+ this->interface->DeviceTranslation("st") +"\n" \
                    "$ma                 - "+ this->interface->DeviceTranslation("ma") +"\n" \
                    "$ufid               - "+ this->interface->DeviceTranslation("ufid") +" unique fingerprint ID\n" \
                    "$me new             - "+ this->interface->DeviceTranslation("me_new") +"\n" \
                    "$me start           - "+ this->interface->DeviceTranslation("me_start") +"\n" \
                    "$me end             - "+ this->interface->DeviceTranslation("me_end") +"\n" \
                    "$me status          - "+ this->interface->DeviceTranslation("me_status") +"\n" \
                    "$history            - "+ this->interface->DeviceTranslation("history") +"\n" \
                    "$ns                 - "+ this->interface->DeviceTranslation("ns") +"\n" \
                    "$ma model           - "+ this->interface->DeviceTranslation("view_mt_model") +"\n" \
                    "$set ma model       - "+ this->interface->DeviceTranslation("set_ma_model") +"\n" \
                    "$st model           - "+ this->interface->DeviceTranslation("view_st_model") +"\n" \
                    "$set st model       - "+ this->interface->DeviceTranslation("set_st_model") +"\n" \
                    "$mi                 - "+ this->interface->DeviceTranslation("mi") +"\n" \      
                    "$set mi [sec]       - "+ this->interface->DeviceTranslation("set_mi") +"\n\n";

    this->interface->sendBLEstring( dataStr, sendTo);
      
    return false;
 }

// -------------------------------------------------------------------------------


bool MATURITY_CLASS::gbrl_commands(String $BLE_CMD, uint8_t sendTo){
    long int hourT; 
    long int minT; 
    long int secT; 
    long int daysT;
    String dataStr="";
    long int $timedif;

    if ($BLE_CMD == "$ufid"){
        if (this->DATASET_NUM_SAMPLES == 0){
            dataStr = this->interface->DeviceTranslation("no_data_entries") + "." +String(char(10));
            this->interface->sendBLEstring( dataStr, sendTo);
            return true;
        }
        dataStr =  this->interface->DeviceTranslation("calc_ufid") + "..." + this->interface->BaseTranslation("wait_moment")  + "." +String(char(10));
        this->interface->sendBLEstring( dataStr, sendTo);
        
        changeMcuFreq(this->interface, this->interface->MAX_FREQUENCY);
        
        float maturityVal = this->custom_maturity(this->DATASET_NUM_SAMPLES);
        float strenghtVal = this->custom_strenght(this->DATASET_NUM_SAMPLES);

        dataStr =  this->interface->DeviceTranslation("curr_temp") + ": " + String(roundFloat(this->last_measured_probe_temp,2)) + String(char(176))+String("C  ");
        dataStr += this->interface->DeviceTranslation("maturity") + ": "+ String(roundFloat( maturityVal ,2)) + String(char(176)) + String("C.h  ");
        dataStr += this->interface->DeviceTranslation("strength") + ": " + String(roundFloat( strenghtVal,2) ) + String(" MPa") + String(char(10));
        dataStr += "Unique Data Fingerprint ID:"+String(char(10));
        dataStr += CryptoICserialNumber(this->interface)+"-"+macChallengeDataAuthenticity(this->interface, String(this->interface->rtc.getDateTime(true)) + String(roundFloat(this->last_measured_probe_temp,2))+"-"+String(roundFloat(maturityVal ,2))+"-"+String(roundFloat(strenghtVal ,2))  );
        dataStr += String(char(10) + String(char(10)) );
        
        changeMcuFreq(this->interface, this->interface->CURRENT_CLOCK_FREQUENCY);
        this->interface->sendBLEstring( dataStr, sendTo);
        return true;
    }
    if( $BLE_CMD == "$pt"){
        this->ProbeSensorStatus(mSerial::DEBUG_NONE);
        this->sensors.requestTemperatures(); // Send the command to get temperature readings 
        this->last_measured_probe_temp = this->sensors.getTempC( this->insideThermometer );
        dataStr= this->interface->DeviceTranslation("curr_temp") + " "+ this->interface->DeviceTranslation("measured")  +" " + String(roundFloat(this->last_measured_probe_temp ,2))+String(char(176))+String("C") + String(char(10));
        this->interface->sendBLEstring( dataStr, sendTo);
        return true;
    }

    if ($BLE_CMD == "$probe status"){
        this->ProbeSensorStatus(sendTo);
        return true;
    }

    if($BLE_CMD == "$st model"){
        dataStr= this->interface->DeviceTranslation("curr_st_model") + ":\n";
        dataStr +=  this->config.custom_strenght_equation + "\n";
        this->interface->sendBLEstring( dataStr, sendTo);
        return true;
    }
    
    if($BLE_CMD == "$ma model"){
        dataStr= this->interface->DeviceTranslation("curr_mt_model") + ":\n";
        dataStr +=  this->config.custom_maturity_equation + "\n";
        this->interface->sendBLEstring( dataStr, sendTo);
        return true;
    }

    if($BLE_CMD == "$mi"){
        dataStr= this->interface->DeviceTranslation("curr_measure_interval") + " " + String(roundFloat(this->config.MEASUREMENT_INTERVAL/(60*1000) ,2)) + String(" min") + String(char(10));
        this->interface->sendBLEstring( dataStr, sendTo);
        return true;
    }

    if( $BLE_CMD == "$ma"){
        dataStr= this->interface->DeviceTranslation("curr_mt_val") + " " + String(roundFloat(this->custom_maturity(this->DATASET_NUM_SAMPLES),2)) + String(char(176)) + String("C.h  ") + String(char(10));     
        this->interface->sendBLEstring( dataStr, sendTo);
        return true;
    }
    if( $BLE_CMD == "$st"){
        dataStr= this->interface->DeviceTranslation("curr_st_val") +  "  "+ String(roundFloat(this->custom_strenght(this->DATASET_NUM_SAMPLES),2))+" Mpa" + String(char(10));    
        this->interface->sendBLEstring( dataStr, sendTo);
        return true;
    }

    if( $BLE_CMD == "$me status"){
        if( this->Measurments_EN == false){
            dataStr = this->interface->DeviceTranslation("measure_not_started") +  String("\n\n");
        } else{
            dataStr = this->interface->DeviceTranslation("measure_already_started") + String("\n");
            dataStr += this->interface->DeviceTranslation("measure_num_records") + " " + String(this->DATASET_NUM_SAMPLES) + "\n\n";
        }

        this->interface->sendBLEstring( dataStr, sendTo); 
        return true;
    }
    if( $BLE_CMD == "$me new"){
        this->Measurments_NEW=true;
        this->Measurments_EN=false;
        this->DATASET_NUM_SAMPLES=0;
        dataStr= this->interface->DeviceTranslation("new_started") +  String("\n\n");
        this->interface->sendBLEstring( dataStr, sendTo); 
        return true;
    }
    if($BLE_CMD == "$me start"){
        if (this->Measurments_EN){
            dataStr = this->interface->DeviceTranslation("measure_already_started_on") +  " " + String(this->measurement_Start_Time) + String("\n\n");
            this->interface->sendBLEstring( dataStr, sendTo); 
        }else{
            this->DATASET_NUM_SAMPLES=0;
            this->Measurments_NEW=true;
            this->Measurments_EN=true;
            this->measurement_Start_Time = this->interface->rtc.getDateTime(true);

            dataStr = this->interface->DeviceTranslation("measure_started_on") +  " " + String(this->measurement_Start_Time) + String("\n");
            this->interface->sendBLEstring( dataStr, sendTo); 
            
            this->gbrl_summary_measurement_config(sendTo);
        }
        return true;

    }
    if( $BLE_CMD=="$me end"){
        if(this->Measurments_EN==false){
            dataStr= this->interface->DeviceTranslation("measure_already_ended")  + String( char(10));
        }else{
            this->Measurments_EN=false;
            dataStr= this->interface->DeviceTranslation("measure_ended_on") +  " " + String(this->interface->rtc.getDateTime(true)) + String( char(10));
        }
        this->interface->sendBLEstring( dataStr, sendTo); 
        return true;
    }
    if($BLE_CMD=="$ns"){
        dataStr = this->interface->DeviceTranslation("num_data_measure") +  ": " + String(this->DATASET_NUM_SAMPLES+1) + String(char(10));
        this->interface->sendBLEstring( dataStr, sendTo);
        return true;
    }
    bool result =false;
    result = this->helpCommands( $BLE_CMD,  sendTo);
    
    bool result2 =false;
    result2 = this->history($BLE_CMD,  sendTo);
    
    bool result3 =false;
    result3 = this->cfg_commands($BLE_CMD,  sendTo);
    
    bool result4 =false;
    result4 = this->measurementInterval($BLE_CMD,  sendTo);    
    
    //this->gbrl_menu_selection();

    bool result5 =false;
    result5 = this->gbrl_menu_custom_model($BLE_CMD,  sendTo);
   
   return ( result || result2 || result3 || result4 || result5 );

}
// ********************************************************
bool MATURITY_CLASS:: gbrl_summary_measurement_config( uint8_t sendTo){
    String dataStr = this->interface->DeviceTranslation("config_summary") +  ":\n";
    dataStr += this->interface->DeviceTranslation("mt_model") +  ": " + this->config.custom_maturity_equation+ "\n";
    dataStr += this->interface->DeviceTranslation("st_model") +  ": " + this->config.custom_strenght_equation + "\n";
    dataStr += this->interface->DeviceTranslation("mi_interval") +  ": " + String(this->config.MEASUREMENT_INTERVAL/1000) + " sec.\n\n";
    this->interface->sendBLEstring( dataStr , sendTo); 
    return true;
}

// ********************************************************
bool MATURITY_CLASS::gbrl_menu_selection(String $BLE_CMD, uint8_t sendTo){
    String dataStr="";
    // SELECT a MENU ******************************************
    if($BLE_CMD.indexOf("$cfg model")>-1){
        if($BLE_CMD=="$cfg model custom"){
            this->selected_menu="$cfg model custom";
            dataStr= "menu selected";
        }else{
            dataStr="option not found";
        }
        this->interface->sendBLEstring( dataStr + String( char(10) ) , sendTo); 
        return true;
    }
    return false;
}

// *******************************************************
bool MATURITY_CLASS::gbrl_model_help(uint8_t sendTo){
    String dataStr = this->interface->DeviceTranslation("model_vars") +  ":" + String( char(10));
    dataStr +="p  - "+this->interface->DeviceTranslation("probe_temp") + +"( " + String(char(176)) + String("C )") + String( char(10));
    dataStr +="a  - "+this->interface->DeviceTranslation("onboard_temp") + +" ( " + String(char(176)) + String("C )") + String( char(10));
    dataStr +="h  - "+this->interface->DeviceTranslation("onboard_humidity") + +" ( % )" + String( char(10));
    dataStr +="t  - "+this->interface->DeviceTranslation("curr_time") + +" ( h )" + String( char(10));
    dataStr +="pt - "+this->interface->DeviceTranslation("prev_time") + +"( h )" + String( char(10));
    dataStr +="dt - "+this->interface->DeviceTranslation("delta_time") + +" = "+  this->interface->DeviceTranslation("curr_time") +" - "+  this->interface->DeviceTranslation("prev_time") +" ( h )" + String( char(10));  
    dataStr +="m  - "+this->interface->DeviceTranslation("mt_val") + +"Maturity value ( " + String(char(176)) + String("C.h )") + String( char(10) ); 
    
    this->interface->sendBLEstring( dataStr + String( char(10) ) , sendTo);
    return true; 
}

// *********************************************************
bool MATURITY_CLASS::gbrl_menu_custom_model(String $BLE_CMD, uint8_t sendTo){
    String dataStr="";
    // $set maturity model - define a new custom maturity model
    // $set strength model 

    // select cfg menu
    if($BLE_CMD=="$set ma model"){
        this->selected_menu="$set ma model";
        this->gbrl_model_help(sendTo);
        dataStr= this->interface->DeviceTranslation("model_formula") + ":";
        this->interface->sendBLEstring( dataStr + String( char(10) ) , sendTo);
        return true; 
    }else if($BLE_CMD=="$set st model"){
        this->selected_menu=="$set st model";
        this->gbrl_model_help(sendTo);
        dataStr= this->interface->DeviceTranslation("model_formula") + ":";
        this->interface->sendBLEstring( dataStr + String( char(10) ) , sendTo); 
        return true;
    }

    // Sub menu selected   *****************************************************
    if (this->selected_menu=="$set ma model"){    
        // validate sring model expression
        String err = validateExpression($BLE_CMD);
        if (err == "MODEL OK"){ 
            dataStr= this->interface->DeviceTranslation("model_validated");
            this->config.custom_maturity_equation=$BLE_CMD;
            this->saveSettings(LittleFS);
        }else{
            dataStr= "E:" + err;
        }
        this->interface->sendBLEstring( dataStr + String( char(10) ) , sendTo); 
        this->selected_menu = "";
        this->selected_sub_menu="";
        return true;
    } else if (this->selected_menu=="$set st model"){
        // validate sring model expression
        String err = validateExpression($BLE_CMD);
        if (err == "MODEL OK"){ 
            dataStr= this->interface->DeviceTranslation("model_validated");
            this->config.custom_strenght_equation=$BLE_CMD;
            this->saveSettings(LittleFS);
        }else{
            dataStr= "E:" + err;
        }
        this->interface->sendBLEstring( dataStr + String( char(10) ) , sendTo); 
        this->selected_menu = "";
        this->selected_sub_menu= "";
        return true;
    }

    return false;
}

// *******************************************************
bool MATURITY_CLASS::cfg_commands(String $BLE_CMD, uint8_t sendTo){
    String dataStr="";
    long int hourT; 
    long int minT; 
    long int secT; 
    long int daysT;
    long int $timedif;

    if($BLE_CMD.indexOf("$cfg mi ")>-1){
        String value= $BLE_CMD.substring(11, $BLE_CMD.length());
        if (isNumeric(value)){
            long int val= (long int) value.toInt();
            if(val>0){
                this->config.MEASUREMENT_INTERVAL=val*1000; // mili seconds 
                this->saveSettings(LittleFS);

                hourT = (long int) ( this->config.MEASUREMENT_INTERVAL/(3600*1000) );
                minT  = (long int) ( this->config.MEASUREMENT_INTERVAL/(60*1000) - (hourT*60));
                secT  = (long int) ( this->config.MEASUREMENT_INTERVAL/1000 - (hourT*3600) - (minT*60));
                daysT = (long int) (hourT/24);
                hourT = (long int) ( (this->config.MEASUREMENT_INTERVAL/(3600*1000) ) - (daysT*24));
                
                dataStr = this->interface->DeviceTranslation("new_mi_accepted") +  "\r\n\n";        
                dataStr += " ["+String(daysT)+"d "+ String(hourT)+"h "+ String(minT)+"m "+ String(secT)+"s "+ String("]\n\n");
                this->interface->sendBLEstring( dataStr, sendTo);
            }else{
                dataStr= this->interface->BaseTranslation("invalid_input") +  "\r\n";
                this->interface->sendBLEstring( dataStr, sendTo);
            }
            return true;
        }
    }
    return false;
}

// *******************************************************
bool MATURITY_CLASS::measurementInterval(String $BLE_CMD, uint8_t sendTo){
    if($BLE_CMD !="$MEASURE INTERVAL" && $BLE_CMD !="$measure interval")
        return false;

    long int hourT; 
    long int minT; 
    long int secT; 
    long int daysT;
    long int $timedif;
    String dataStr="";

    hourT = (long int) (this->config.MEASUREMENT_INTERVAL/(3600*1000) );
    minT = (long int) (this->config.MEASUREMENT_INTERVAL/(60*1000) - (hourT*60));
    secT =  (long int) (this->config.MEASUREMENT_INTERVAL/1000 - (hourT*3600) - (minT*60));
    daysT = (long int) (hourT/24);
    hourT = (long int) ((this->config.MEASUREMENT_INTERVAL/(3600*1000) ) - (daysT*24));

    dataStr= this->interface->DeviceTranslation("mi_interval")  + String(char(10)) + String(daysT)+"d "+ String(hourT)+"h "+ String(minT)+"m "+ String(secT)+"s "+ String(char(10));
    this->interface->sendBLEstring( dataStr, sendTo);
    return true;
}


// ********************************************************
bool MATURITY_CLASS::history(String $BLE_CMD, uint8_t sendTo){
    if($BLE_CMD != "$history"  )
        return false;

    long int hourT; 
    long int minT; 
    long int secT; 
    long int daysT;
    String dataStr="";
    long int $timedif;
    time_t timeNow;
    time(&timeNow);

    dataStr = this->interface->DeviceTranslation("calc_mi_st_val") +  "..."+ this->interface->BaseTranslation("wait_moment") +"." +String(char(10));
    this->interface->sendBLEstring( dataStr, sendTo);

    changeMcuFreq(this->interface, this->interface->MAX_FREQUENCY);

    dataStr = "\n"+ this->interface->DeviceTranslation("data_history") +String(char(10));

    File file = LittleFS.open("/" + this->interface->config.SENSOR_DATA_FILENAME, "r");
    int counter=0; 

    long int sumTimeDelta=0;

    while (file.available()) {
        this->interface->sendBLEstring( "#", sendTo);

        if (counter == 0 ) {
            // raed the header
            String bin2 = file.readStringUntil( char(10) );
            bin2 = file.readStringUntil( char(10) );
        }

        String bin = file.readStringUntil( ';' ); // RTC Date & Time

        long int timeStart = atol(file.readStringUntil( (char) ';' ).c_str() ); //start time of measure 

        long int timeDelta = atol(file.readStringUntil( (char) ';' ).c_str() ); // delta time since last measure
        sumTimeDelta+=timeDelta; //elapsed time since start time of measure

        float temp = (file.readStringUntil( (char) ';' ).toFloat()); // probe temp.

        String rest = file.readStringUntil( char(10) ); // remaider of data string line

        $timedif = (timeStart+sumTimeDelta) - timeStart;
        hourT = (long int) ($timedif / (3600*1000));
        minT = (long int) ($timedif/ (60*1000) - (hourT*60));
        secT =  (long int) ($timedif/1000 - (hourT*3600) - (minT*60));
        daysT = (long int) (hourT/24);
        hourT = (long int) (($timedif/(3600*1000) ) - (daysT*24));

        dataStr +=  ": ["+String(daysT)+"d "+ String(hourT)+"h "+ String(minT)+"m "+ String(secT)+"s "+ String("]  ");    
        dataStr +=  this->interface->DeviceTranslation("probe_temp") + ": ";
        dataStr += String(roundFloat(temp,2))+String(char(176))+String("C  ");
        dataStr += this->interface->DeviceTranslation("maturity") + ": "+String(roundFloat(this->custom_maturity(counter),2)) + String(char(176)) + String("C.h  ");
        dataStr += this->interface->DeviceTranslation("strength") + ": "+String(roundFloat(this->custom_strenght(counter),2))+String(" MPa") + String(char(10));

        counter++; 
    }

    file.close();
  
    dataStr += "--------------- \n";
    this->interface->sendBLEstring( dataStr, sendTo);
    changeMcuFreq(this->interface, this->interface->CURRENT_CLOCK_FREQUENCY);
  return true;
}

// *********************************************************