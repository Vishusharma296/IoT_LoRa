/*
  This codes reads DHT11 Temperature and relative humidity data,
  displays it on Onboard OLED display on Heltec LoRa32 board
  Sends a LoRa packet containing: Message number, Temperature and Relative Humidity data
  to another LoRa reciever (can be read by multiple LoRa recievers)
  Logs the transmitted data via serial communication on local PC (Putty is suitable)
  
*/

// Required Libraries:

#include "heltec.h"        // from board manufactures for LoRa, OLED, ESP32
#include <DHT.h>           // From sensor manufacturer

#define Type DHT11         // Sensor type
#define BAND 868E6    //ISM band in EU is 868MHz for LoRa

// Pin assignment

int Pin_DHT11 = 23;     // Signal pin for DHT object attached to MCU

// variable assignment

unsigned int counter = 0;           // For message Number
String rssi = "RSSI --";            // Recieved signal strength
String packSize = "--";             // Data packet size
String packet ;                     // Data packet

int Node_num = 001  ;                // Sensor Node number
int baudrate = 115200;               // serial communication rate
float N1_Temp = 23;                  // Temperature in Celsius
float N1_RH = 55 ;                   // Relative Humidity in percent
int Time_pol = 29500;                // Sample Time for polling the sensor
bool label = true;                   // For labeling in serial communication

DHT DHT_Node_1(Pin_DHT11, DHT11);    // DHT object attached to Node_1


void setup()
{
   // OLED display initialization (Check Heltec documentation for more info)
   
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
 
  delay(2000);
  Heltec.display->clear();
  
  Heltec.display->drawString(0, 0, "Starting Node Nr: ");
  Heltec.display->drawString(90, 0, String(Node_num));
  Heltec.display->display();
  delay(3000);

  // Starting communication with sensor DHT11 on Node_1

  DHT_Node_1.begin();    
  //Serial.begin(baudrate);  Do not use this statement!!!
}

void loop()
{

  // Reading Sensors:
  N1_Temp = DHT_Node_1.readTemperature();
  N1_RH = DHT_Node_1.readHumidity();
  

  // Display info at sensor Node OLED display:
  
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(30,0, "--NODE 1-- ");
  Heltec.display->drawString(0, 10, "Sending Msg Nr: ");
  Heltec.display->drawString(90, 10, String(counter));
  Heltec.display->drawString(0, 20, "Temperature : ");
  Heltec.display->drawString(90, 20, String(N1_Temp));
  Heltec.display->drawString(0, 30, "Rel Hum: ");
  Heltec.display->drawString(90, 30, String(N1_RH));
  Heltec.display->display();


  // Sending data packet via LoRa modulated radio:

 /* Info from manufacturer of board:
 
 * LoRa.setTxPower(txPower,RFOUT_pin);
 * txPower -- 0 ~ 20  // 14 dBm is power efficient
 * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
 *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
 *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
*/

  LoRa.beginPacket();

  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST); // Transmitting at 14dBm
  // This data packet will be displayed on the LoRa reciever and sent 

  LoRa.print("Node -1- Msg Nr: ");
  LoRa.println(counter);
  LoRa.print("Temp_C: ");
  LoRa.println(N1_Temp);
  LoRa.print("RH_%: ");
  LoRa.println(N1_RH);
  
  LoRa.endPacket();

  
  // Polling every 30 seconds
  
  digitalWrite(LED, HIGH);           // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for 2 second
  digitalWrite(LED, LOW);            // turn the LED off by making the voltage LOW
  delay(Time_pol);  

  counter++;


  // Printing and Data logging via serial communication:
  // This data will be recorded in the CSV form when connected to puTTy via USB


  while(label){
    
    Serial.print("Message_Number");
    Serial.print(",");
    Serial.print("Temp_Celsius");
    Serial.print(",");
    Serial.print("RH_Percent");
    Serial.println(" ");
    label = false;
    }

  Serial.print(counter);
  Serial.print(",");
  Serial.print(N1_Temp);
  Serial.print(",");
  Serial.println(N1_RH);
 
}
