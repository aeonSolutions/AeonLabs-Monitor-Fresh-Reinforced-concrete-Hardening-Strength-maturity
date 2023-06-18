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

//---------------------------------------------------------------------------------
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

#ifndef MATURITY_CLASS_DEF
  #define MATURITY_CLASS_DEF

  //-------------------------------------------------------------------
  //External DS18b20 sensor
  #include <OneWire.h> 
  #include <DallasTemperature.h>
  #include "mserial.h"
  #include "interface_class.h"
  #include "onboard_sensors.h"
   #include "FS.h"
   #include <LittleFS.h>
  #include "onboard_led.h"
  #include "tinyexpr.h"
  #include "m_wifi.h"

  class MATURITY_CLASS {
  private:
    OneWire onewire; 
    String validateExpression(String expression);
    String previous_UFPID;
    uint8_t  get_number_model_vars(String equation);
    double measureValues[8]; // {"a","h","pt","dt","p","m","t","u"};
    int currentDatasetPos;    
    String var[8];
    String signs[9];

    long int prevTimeMeasured;
    
   // GBRL commands  *********************************************
    bool gbrl_menu_selection(String $BLE_CMD, uint8_t sendTo);
    bool gbrl_model_help(uint8_t sendTo);
    bool gbrl_menu_custom_model(String $BLE_CMD, uint8_t sendTo);
    bool cfg_commands(String $BLE_CMD, uint8_t sendTo);
    bool measurementInterval(uint8_t sendTo);
    bool history(String $BLE_CMD, uint8_t sendTo);
    bool helpCommands(String $BLE_CMD, uint8_t sendTo);
    bool measurementInterval(String $BLE_CMD, uint8_t sendTo);
    bool gbrl_summary_measurement_config(uint8_t sendTo);

  public:
      typedef struct{
          bool custom_maturity_equation_is_summation;
          String custom_maturity_equation;
          String custom_strenght_equation;
          unsigned long MEASUREMENT_INTERVAL; // IN SEC

      } config_strut;
      config_strut config;

      String selected_menu;
      String selected_sub_menu;
      
   DallasTemperature sensors;
   DeviceAddress insideThermometer;

    uint8_t EXT_PLUG_DI_IO;

    bool hasNewMeasurementValues;
    float last_measured_probe_temp;
    float last_measured_time_delta;     
    int DATASET_NUM_SAMPLES;
    
    bool Measurments_EN;
    bool Measurments_NEW;
    String measurement_Start_Time;
    
    mSerial* mserial=nullptr;
    INTERFACE_CLASS* interface=nullptr;
    ONBOARD_SENSORS* onBoardSensors=nullptr;
    M_WIFI_CLASS* mWifi;

    MATURITY_CLASS();

    void init(INTERFACE_CLASS* interface, ONBOARD_SENSORS* onBoardSensors, mSerial* mserial, M_WIFI_CLASS* mWifi);
    void ProbeSensorStatus(uint8_t sendTo);

    int get_dataset_size();
    int get_dataset_memory_availability();   
    bool reinitialize_dataset_file( fs::FS &fs = LittleFS );
    void get_dataset_values(int pos);

    void request_measurment_data();
    bool save_measurment_record( fs::FS &fs = LittleFS );

    double custom_strenght(int pos);
    double custom_maturity(int pos);

    // Setup configuration and settings *******************************************
    bool readSettings( fs::FS &fs = LittleFS );
    bool saveSettings( fs::FS &fs = LittleFS  );


   // GBRL commands  *********************************************
    bool gbrl_commands(String $BLE_CMD, uint8_t sendTo);

};
#endif
