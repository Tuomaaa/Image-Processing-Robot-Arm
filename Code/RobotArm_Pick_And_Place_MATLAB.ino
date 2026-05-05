/*************************************************** 
CAB 4/1/26
RobotArm-Pick-And-Place_MATLAB.ino
This code moves the 3d printed 5DOF robot to pick up a red, green and blue cube from one of three pick up locations
to a place location in front of the robot
5 DOF Robot (base - 0, shoulder - 4, elbow - 3, wrist - 2, gripper - 1)
  3 values received from MATLAB
    cube locations are
      1st number - location of red cube goes to place 1 (robot's right), place1() function
      2nd number - location of green cube goes to place 2 (robot's front),place2() function
      3rd number - location of blue cube goes to place 3 (robot's left), place3() function

    Pick location
      1 - top (furthest from robot), pick1() function 
      2 - middle - pick 2, pick2() function
      3 - bottom (closest to robot), pick3() function 

  */

#include <Wire.h>                                         //this library allows I2C communicaton for the servo driver board and arducam
#include <Adafruit_PWMServoDriver.h>                      //servo motor driver library
#include <EEPROM.h>                                       //voltatile memory library
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();  //instance of servo motor driver

struct Data {  //data structure to hold color position data from MATLAB
  int color1;
  int color2;
  int color3;
};

Data myData;          //instance of data structure current data from matLab
Data newValue;        //new data from MATLAB
int EEPROM_ADDR = 0;  //address to store color position data in volatile memory
int positions[3];     //variable to hold color cube position data

//servo motor and other global variables
#define SERVO_FREQ 50     // Analog servos run at ~50 Hz updates
#define baudrate 115200   //servo communication baudrate
#define oscFreq 27000000  //oscilator frequency
#define pauseTime 1000    //pause between test cycles
#define maxSpeed 10       //maximum robot speed
int speed = 6;            //speed is an integer between 0 and 10, increase to speed up robot
#define blocks 3          //number of blocks to sort

//hardware connections
#define base 0      // robot base = 0
#define shoulder 4  // robot shoulder = 1
#define elbow 3     // robot elbow = 3
#define wrist 2     // robot wriset = 4
#define gripper 1   // robot gripper = 5

/**********MOTOR POSITIONS FOR HOMING AND PICK AND PLACE POSITIONS - can eventyally move to tab or header 
calibrate these values to work best for your robot*/
#define BUSMIN 600    // base minimum position (far right)
#define BUSMAX 2400   // base maximum position (far left)
#define BUSHome 1500  //base home position (middle)
int BPos = BUSHome;   //current base position

#define BPlace1a 1200  //base place 1 position a (right)
#define BPlace2a 1500  //base place 2 position a (center)
#define BPlace3a 1900  //base place 3 position a (lefy)

#define SUSMIN 800    //shoulder min (back)
#define SUSMAX 2000   //shoulder max (forward)
#define SUSHome 1000  //shoulder home (middle)
int SPos = SUSHome;   //current shoulder position

#define SPick3a 1800  //shoulder pick 3a (in)
#define SPick3b 2100  //shoulder pick 3b (way in)

#define SPick2a 1800  //shoulder pick 2a (in)
#define SPick2b 2200  //shoulder pick 2b ( way in)

#define SPick1a 2000  //shoulder pick 1a (in)
#define SPick1b 2200  //shoulder pick 1b (way in)

#define SPlace1a 1300  //shoulder place 1a (in)
#define SPlace1b 1700  //shoulder place 1b (way in)

#define SPlace2a 1300  //shoulder place 2a (in)
#define SPlace2b 1700  //shoulder place 2b (way in)

#define SPlace3a 1300  //Shoulder place 3a (in)
#define SPlace3b 1800  //Shoulder place 3a (way in)

#define EUSMIN 800    //elbow min (out)
#define EUSMAX 2000   //elbow max (in)
#define EUSHome 1500  //elbow home (middle)
int EPos = EUSHome;   //current elbow position

#define EPick1a 1000  //shoulder pick 1a (in)
#define EPick2a 1200  //shoulder pick 2a (in)
#define EPick3a 1300  //shoulder pick 3a (in)

#define EPlace1a 1600  //shoulder place 1a (in)
#define EPlace2a 1600  //shoulder place 2a (in)
#define EPlace3a 1600  //shoulder place 3a (in)

#define WUSMIN 600    // wristmin (far in)
#define WUSMAX 2000   // wrist max (far out)
#define WUSHome 1000  //wristhome (mid)
int WPos = WUSHome;   //current wrist position

#define WPick1 1000  //wrist (mid)
#define WPick2 1000  //wrist (mid)
#define WPick3 1000  //wrist (mid)

#define WPlace1 1000  //wrist (mid)
#define WPlace2 1000  //wrist (mid)
#define WPlace3 1000  //wrist (mid)

#define GUSMIN 600   //gripper vertical
#define GUSMAX 1300  //gripper horizontal
#define GUSHome 600  //gripper home
int GPos = GUSHome;  //current gripper positiion

#define GOpen 1200  //gripper open
#define GClose 600  //gripper close

//code that runs only once
void setup() {
  Serial.begin(baudrate);               //start serial communication
  pinMode(LED_BUILTIN, OUTPUT);         //LED on pin 13 on Arduino to blink with position data
  pwm.begin();                          //start servo motor driver board
  pwm.setOscillatorFrequency(oscFreq);  //set servo motor oscillator frequency
  pwm.setPWMFreq(SERVO_FREQ);           // Analog servos run at ~50 Hz updates
  delay(1000);

  // Restore last saved value
  EEPROM.get(EEPROM_ADDR, myData);
  Serial.print("color position data: ");
  Serial.print(myData.color1);
  Serial.print(myData.color2);
  Serial.println(myData.color3);

  /************** UNCOMMENT FUNCTIONS FOR TESTING EACH MOTOR DURING CALIBRATIONT*/
  // Serial.println("Move to Home");
  // robotHoming();
  // delay(pauseTime);
  // Serial.println("Move to Pick 1");
  // delay(pauseTime);
  // pick1();
  // delay(pauseTime);
  // Serial.println("Move to Place 1");
  // place1();
  // delay(pauseTime);
  // Serial.println("Move to Pick 2");
  // pick2();
  // delay(pauseTime);
  // Serial.println("Move to Place 2");
  // place2();
  // delay(pauseTime);
  // Serial.println("Move to Pick 3");
  // pick3();
  // delay(pauseTime);
  // Serial.println("Move to Place 3");
  // place3();
}

//code that runs indefinitely
void loop() {
  matlabSequence();  //read position data from MATLAB
  printPositions();  //print position data to serial monitor
  LEDBlink();        //blink position data on LED
  delay(100);
  if (positions[0] != 0 && positions[1] != 0 && positions[2] != 0) {
    for (int i = 0; i < blocks; i++) {
      Serial.println("start moving cubes");
      switch (positions[i]) {
        //base - 0, shoulder - 4, elbow - 3, wrist - 2, gripper - 1
        case 1:  //base
          pick1();
          break;
        case 2:  //gripper
          pick2();
          break;
        case 3:  //wrist
          pick3();
          break;
      }
      switch (i) {
        //base - 0, shoulder - 4, elbow - 3, wrist - 2, gripper - 1
        case 0:  //base
          place1();
          break;
        case 1:  //gripper
          place2();
          break;
        case 2:  //wrist
          place3();
          break;
      }
    }
    //finished sorting, stoop loop
    while (1) {};
  }  //end if condition
  else {
    Serial.println("no cube position data received");
  }
}

//read cube position data from MATLAB

void matlabSequence() {
  Serial.println("MATLAB SEQUENCE");
  if (Serial.available() >= 3) {
    Serial.print("Incoming: ");
    newValue.color1 = Serial.parseInt();
    newValue.color2 = Serial.parseInt();
    newValue.color3 = Serial.parseInt();
    Serial.print(newValue.color1);
    Serial.print(newValue.color2);
    Serial.println(newValue.color3);
    if (isDifferent(newValue, myData) && isNonZero(newValue)) {
      myData = newValue;
      Serial.print("Stored");
      Serial.print(myData.color1);
      Serial.print(myData.color2);
      Serial.println(myData.color3);
      EEPROM.put(EEPROM_ADDR, myData);
    } else {
      Serial.println("no new data received");
    }
  }  //serial available
}

bool isDifferent(Data a, Data b) {
  return (a.color1 != b.color1) || (a.color2 != b.color2) || (a.color3 != b.color3);
}

bool isNonZero(Data a) {
  return (a.color1 > 0) && (a.color2 > 0) && (a.color3 > 0);
}

//print cube position data to serial monitor
void printPositions() {
  String colors[3] = { " red: ", " green: ", " blue: " };
  positions[0] = myData.color1;
  positions[1] = myData.color2;
  positions[2] = myData.color3;
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
    if (pos[i] != 0) {
      for (int k = 0; k < pos[i]; k++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
      }
      delay(400);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  delay(1000);
}

/***********************************************************************************/
//move the robot arm to a location at a given speed
void moveSpeed(int joint, int location, int spd) {
  int steps = spd;  //movement steps, make this value 1 to 10 to modify speed, increase to speed up robot
  int current;      //holds the current position of the joint passed to the function
  int start;        //holds i loop variable start value
  // Serial.print("joint: ");
  // Serial.print(joint);
  // Serial.print(" location: ");
  // Serial.print(location);
  // Serial.print(" speed: ");
  // Serial.print(spd);
  // Serial.print(" delay ");
  // Serial.println(10 * (maxSpeed - spd));

  switch (joint) {
    //base - 0, shoulder - 4, elbow - 3, wrist - 2, gripper - 1
    case base:  //base
      current = BPos;
      break;
    case gripper:  //gripper
      current = GPos;
      break;
    case wrist:  //wrist
      current = WPos;
      break;
    case elbow:  //elbow
      current = EPos;
      break;
    case shoulder:  //shoulder
      current = SPos;
      break;
  }

  start = current;  //set the i start value to current joint position
  if (spd > 0) {
    if (location > start) {
      for (int i = start; i <= location; i = i + steps) {
        pwm.writeMicroseconds(joint, i);  //pin 0
        delay(maxSpeed - spd);            //delay movements by robot speed
        //Serial.println(i);
      }
    } else {
      for (int i = start; i >= location; i = i - steps) {
        pwm.writeMicroseconds(joint, i);  //pin 0
        delay(maxSpeed - spd);            //delay movements by robot speed
        //Serial.println(i);
      }
    }
    setCurrent(joint, location);  //set joint current positionto final value
  }
}

//set robot joint current position
void setCurrent(int joint, int pos) {
  switch (joint) {
    //base - 0, shoulder - 4, elbow - 3, wrist - 2, gripper - 1
    case base:  //base
      BPos = pos;
      break;
    case gripper:  //gripper
      GPos = pos;
      break;
    case wrist:  //wrist
      WPos = pos;
      break;
    case elbow:  //elbow
      EPos = pos;
      break;
    case shoulder:  //shoulder
      SPos = pos;
      break;
  }
}

//set robot to start position
void robotHoming() {
  Serial.println("homing: move base home");
  moveSpeed(base, BUSHome, speed);
  setCurrent(base, BUSHome);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("homing: move shoulder home");
  moveSpeed(shoulder, SUSHome, speed);
  setCurrent(shoulder, SUSHome);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("homing: move elbow home");
  moveSpeed(elbow, EUSHome, speed);
  setCurrent(elbow, EUSHome);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("homing: move wrist home");
  moveSpeed(wrist, WUSHome, speed);
  setCurrent(wrist, WUSHome);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("homing: move gripper home");
  moveSpeed(gripper, GUSHome, speed);
  setCurrent(gripper, GUSHome);  //set joint current positionto final value
}

//pick up object from position 1
void pick1() {
  Serial.println("pick up from position1");
  //Serial.println("pick1: move base");
  delay(pauseTime);
  moveSpeed(base, BUSHome, speed);
  setCurrent(base, BUSHome);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick1: move elbow");
  moveSpeed(elbow, EPick1a, speed);
  setCurrent(elbow, EPick1a);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick1: move shoulder");
  moveSpeed(shoulder, SPick1a, speed);
  setCurrent(shoulder, SPick1a);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick1: move gripper");
  moveSpeed(gripper, GOpen, speed);
  setCurrent(gripper, GOpen);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick1: move wrist");
  moveSpeed(wrist, WPick1, speed);
  setCurrent(wrist, WPick1);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick1: move shoulder");
  moveSpeed(shoulder, SPick1b, speed);
  setCurrent(shoulder, SPick1b);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick1: move gripper");
  moveSpeed(gripper, GClose, speed);
  setCurrent(gripper, GClose);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick1: move shoulder");
  moveSpeed(shoulder, SUSHome, speed);
  setCurrent(shoulder, SUSHome);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick1: move elbow");
  moveSpeed(elbow, EUSHome, speed);
  setCurrent(elbow, EUSHome);  //set joint current positionto final value
}

//pick up object from position 2
void pick2() {
  Serial.println("pick up from position2");
  //Serial.println("pick2: move base");
  delay(pauseTime);
  moveSpeed(base, BUSHome, speed);
  setCurrent(base, BUSHome);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick2: move elbow");
  moveSpeed(elbow, EPick2a, speed);
  setCurrent(elbow, EPick2a);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick2: move shoulder");
  moveSpeed(shoulder, SPick2a, speed);
  setCurrent(shoulder, SPick2a);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick2: move gripper");
  moveSpeed(gripper, GOpen, speed);
  setCurrent(gripper, GOpen);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick2: move wrist");
  moveSpeed(wrist, WPick1, speed);
  setCurrent(wrist, WPick1);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick2: move shoulder");
  moveSpeed(shoulder, SPick2b, speed);
  setCurrent(shoulder, SPick2b);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick2: move gripper");
  moveSpeed(gripper, GClose, speed);
  setCurrent(gripper, GClose);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick2: move shoulder");
  moveSpeed(shoulder, SUSHome, speed);
  setCurrent(shoulder, SUSHome);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick2: move elbow");
  moveSpeed(elbow, EUSHome, speed);
  setCurrent(elbow, EUSHome);  //set joint current positionto final value
}

//pick up object from position 3
void pick3() {
  Serial.println("pick up from position3");
  //Serial.println("pick3: move base");
  delay(pauseTime);
  moveSpeed(base, BUSHome, speed);
  setCurrent(base, BUSHome);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick3: move elbow");
  moveSpeed(elbow, EPick3a, speed);
  setCurrent(elbow, EPick3a);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick3: move shoulder");
  moveSpeed(shoulder, SPick3a, speed);
  setCurrent(shoulder, SPick3a);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick3: move gripper");
  moveSpeed(gripper, GOpen, speed);
  setCurrent(gripper, GOpen);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick3: move wrist");
  moveSpeed(wrist, WPick3, speed);
  setCurrent(wrist, WPick3);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick3: move shoulder");
  moveSpeed(shoulder, SPick3b, speed);
  setCurrent(shoulder, SPick3b);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick3: move gripper");
  moveSpeed(gripper, GClose, speed);
  setCurrent(gripper, GClose);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick3: move shoulder");
  moveSpeed(shoulder, SUSHome, speed);
  setCurrent(shoulder, SUSHome);  //set joint current positionto final value
  delay(pauseTime);
  //Serial.println("pick3: move elbow");
  moveSpeed(elbow, EUSHome, speed);
  setCurrent(elbow, EUSHome);  //set joint current positionto final va
}

//drop off object at position 1
void place1() {
  Serial.println("drop at place 1");
  moveSpeed(base, BPlace1a, speed);  //pin 0
  delay(pauseTime);
  moveSpeed(shoulder, SPlace1a, speed);  //pin 4
  delay(pauseTime);
  moveSpeed(elbow, EPlace1a, speed);  //pin 4
  delay(pauseTime);
  moveSpeed(shoulder, SPlace1b, speed);  //pin 4
  delay(pauseTime);
  moveSpeed(shoulder, SPlace1b, speed);  //pin 4
  delay(pauseTime);
  moveSpeed(gripper, GOpen, speed);  //pin 1
  delay(pauseTime);
  moveSpeed(shoulder, SPlace1a, speed);  //pin 4
  delay(pauseTime);
  moveSpeed(base, BPlace1a, speed);  //pin 0
  delay(pauseTime);
  robotHoming();
}

//drop off object at position 2
void place2() {
  Serial.println("drop at place 2");
  moveSpeed(base, BPlace2a, speed);      //pin 0
  moveSpeed(shoulder, SPlace1a, speed);  //pin 4
  moveSpeed(elbow, EPlace2a, speed);     //pin 4
  moveSpeed(shoulder, SPlace2b, speed);  //pin 4
  moveSpeed(gripper, GOpen, speed);      //pin 1
  moveSpeed(shoulder, SPlace2b, speed);  //pin 4
  moveSpeed(gripper, GClose, speed);     //pin 1
  moveSpeed(shoulder, SPlace2a, speed);  //pin 4
  moveSpeed(base, BPlace2a, speed);      //pin 0
  robotHoming();
}

//drop off object at position 3
void place3() {
  Serial.println("drop at place 3");
  moveSpeed(base, BPlace3a, speed);  //pin 0
  delay(pauseTime);
  moveSpeed(shoulder, SPlace3a, speed);  //pin 4
  delay(pauseTime);
  moveSpeed(elbow, EPlace3a, speed);  //pin 4
  delay(pauseTime);
  moveSpeed(shoulder, SPlace3a, speed);  //pin 4
  delay(pauseTime);
  moveSpeed(shoulder, SPlace3b, speed);  //pin 4
  delay(pauseTime);
  moveSpeed(gripper, GOpen, speed);  //pin 1
  delay(pauseTime);
  moveSpeed(shoulder, SUSHome, speed);  //pin 4
  delay(pauseTime);
  moveSpeed(base, BUSHome, speed);  //pin 0
  delay(pauseTime);
  robotHoming();
}
