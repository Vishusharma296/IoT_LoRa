/*Lora reciever
* This code tries to parse any available LoRa packets at 868 MHz
* Suitable only for heltec LoRa32 V2 development boards.
*/

#include "heltec.h" 

#define BAND    868E6      //ISM band in EU is 868MHz for LoRa
String Rx_rssi = "RSSI: ";   // signal strength for reciever
String packSize = "--";    // Data size of packet in bytes
String packet ;            // Actual LoRa packet


// For Display on OLED screen on board

void LoRa_OLED(){
  Heltec.display->clear();
  
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0, Rx_rssi);
  Heltec.display->drawString(0 , 10 , packSize);
  Heltec.display->drawString(0 , 20, packet);
    
  Heltec.display->display();
}

// For reading, packets, packet size, RSSI

void Packet_reading(int packetSize) {
  packet = "";
  packSize = String(packetSize,DEC);   // Packet size in decimal
  
  for (int i = 0; i < packetSize; i++) {
    packet += (char) LoRa.read(); 
    }
  Rx_rssi = "RSSI_dBm: " + String(LoRa.packetRssi(), DEC) ;  // Gives the reciever signal strength
  packSize =  "Psize_byte: " + String(packetSize, DEC) ;  // Gives the packet size


  Serial.println(Rx_rssi);
  Serial.println(packSize);
  Serial.println(packet);
  Serial.println("");
  
  LoRa_OLED();
}


// Initial setup

void setup() { 
   
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  
  delay(2000);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Wait for incoming data...");
  Heltec.display->display();
  delay(2000);
  //LoRa.onReceive(cbk);
  LoRa.receive();
}


void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) { Packet_reading(packetSize);  }
  delay(100);
  
}
