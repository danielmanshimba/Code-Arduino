
#include <SPI.h>


const int CS_PIN = 10;  


const byte POT_Z1 = 0x00;  
const byte POT_Z2 = 0x01;  
const byte POT_Z3 = 0x02;  
const byte POT_Z4 = 0x03;  /

void setup() {
  Serial.begin(9600);
  

  SPI.begin();
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);  
  
 
  setPotentiometer(POT_Z1, 255);
  setPotentiometer(POT_Z2, 255);
  setPotentiometer(POT_Z3, 255);
  setPotentiometer(POT_Z4, 255);
  
  Serial.println("AD5204 Controller Ready");
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    data.trim();
    
   
    if (data.startsWith("Z1:") && data.length() > 10) {
      processZoneData(data);
    }
  }
}

void setPotentiometer(byte channel, byte value) {
  digitalWrite(CS_PIN, LOW);  
  SPI.transfer(channel);      
  SPI.transfer(value);         
  digitalWrite(CS_PIN, HIGH); 
}

void processZoneData(String data) {
  int values[4] = {255};
  int index = 0;
  int startPos = 0;
  
  for (int i = 0; i < data.length(); i++) {
    if (data[i] == ',' || i == data.length() - 1) {
      String segment = data.substring(startPos, i);
      if (segment.startsWith("Z")) {
        int colonPos = segment.indexOf(':');
        if (colonPos != -1) {
          String valueStr = segment.substring(colonPos + 1);
          values[index] = valueStr.toInt();
          index++;
        }
      }
      startPos = i + 1;
    }
  }
  
  // Appliquer les valeurs aux potentiomètres
  if (index == 4) {
    setPotentiometer(POT_Z1, constrain(values[0], 0, 255));
    setPotentiometer(POT_Z2, constrain(values[1], 0, 255));
    setPotentiometer(POT_Z3, constrain(values[2], 0, 255));
    setPotentiometer(POT_Z4, constrain(values[3], 0, 255));
    
    Serial.print("Potentiometers updated: ");
    Serial.print(values[0]);
    Serial.print(", ");
    Serial.print(values[1]);
    Serial.print(", ");
    Serial.print(values[2]);
    Serial.print(", ");
    Serial.println(values[3]);
  }
}