/*LEDBlinkArduinoCode_storePosition.ino
  CAB, MS - 4.16.26
  use volatile memory to store the data from MATLAB on location of red, green and blue cubes
  3 values received from MATLAB
    cube locations are
      1st number - location of red cube goes to place 1 (robot's right)
      2nd number - location of green cube goes to place 2 (robot's front)
      3rd number - location of blue cube goes to place 3 (robot's left)

    Pick location
      1 - top (furthest from robot), 
      2 - middle - pick 2, 
      3 - bottom (closest to robot) 

  */

#include <EEPROM.h>

struct Data {
  int color1;
  int color2;
  int color3;
};

Data myData;

int EEPROM_ADDR = 0;
int positions[3];


void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);

  // Restore last saved value
  EEPROM.get(EEPROM_ADDR, myData);

  Serial.print("Restored value: ");
  Serial.print(myData.color1);
  Serial.print(myData.color2);
  Serial.println(myData.color3);
}

void loop() {
  matlabSequence();
  LEDBlink();
  printPositions();
}

void matlabSequence() {
  if (Serial.available() >= 3) {
    myData.color1 = Serial.parseInt();
    myData.color2 = Serial.parseInt();
    myData.color3 = Serial.parseInt();
    EEPROM.put(EEPROM_ADDR, myData);
  }
}


void printPositions() {
  String colors[3] = { " red: ", " green: ", " blue: " };
  for (int i = 0; i < 3; i++) {
    Serial.print(colors[i]);
    Serial.print(positions[i]);
  }
  Serial.println();
}

// Simple function to cause LED to blink at amount based on sequence
void LEDBlink() {
  int pos[3] = { myData.color1, myData.color2, myData.color3 };
  for (int i = 0; i < 3; i++) {
    for (int k = 0; k < pos[i]; k++) {
      positions[i] = pos[i];
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
    delay(400);
  }
  delay(1000);
}