/*************************************************** 
CAB 2/2/26
Robot Arm SERVO TEST
5 DOF Robot (base - 0, shoulder - 4, elbow - 3, wrist - 2, gripper - 1)
Thhis code is used to jog the 5 robot motors to calibrate for the max, min and home positions
/*called this way, it uses the default address 0x40
you can also call it with a different address you want Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
you can also call it with a different address and I2C interface Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);
added moveSpeed function 4/10/26 CAB
 ****************************************************/

#include <Wire.h> //this library allows I2C communicaton for the servo driver board and arducam
#include <Adafruit_PWMServoDriver.h>  //servo motor driver library
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //instance of servo motor driver

/**********MOTOR POSITIONS FOR HOMING AND STARTUP MIN MAX VALUES - can eventually move to tab or header files 
calibrate these values to work best for your robot*/
#define BUSMIN 600      // base minimum position
#define BUSMAX 2400     // base maximum position
#define BUSHome 1500    //base home position
int BPos;              //current base position

#define SUSMIN 800     //shoulder min
#define SUSMAX 2000    //shoulder max
#define SUSHome 1000   //shoulder home
int SPos;              //current shoulder position

#define EUSMIN 800     //elbow min
#define EUSMAX 2000    //elbow max
#define EUSHome 1500   //elbow home
int EPos;              //current elbow position

#define WUSMIN 600   // wristmin
#define WUSMAX 2000  // wrist max
#define WUSHome 800  //wristhome
int WPos;            //current wrist position

#define GUSMIN 600   //gripper vertical
#define GUSMAX 1300  //gripper horizontal
#define GUSHome 600  //gripper home
#define GOpen 1200   //gripper open
#define GClose 600   //gripper close
int GPos;            //current gripper position

#define SERVO_FREQ 50     // Analog servos run at ~50 Hz updates
#define baudrate 9600     //servo communication baudrate
#define oscFreq 27000000  //oscilator frequency
#define pauseTime 1000    //pause between test cycles
#define repeats 1         //number of times to jog motor

uint8_t servonum = 0;  // our servo # counter
#define numOfservo 5   //number of servo
#define base 0         // robot base = 0
#define shoulder 4     // robot shoulder = 1
#define elbow 3        // robot elbow = 3
#define wrist 2        // robot wriset = 4
#define gripper 1      // robot gripper = 5

#define maxSpeed 10  //maximum robot speed
int speed = 2;       //speed is an integer between 0 and 10, increase to speed up robot

void setup() {
  Serial.begin(baudrate);
  pwm.begin();
  pwm.setOscillatorFrequency(oscFreq);  //set servo motor oscillator frequency
  pwm.setPWMFreq(SERVO_FREQ);           // Analog servos run at ~50 Hz updates
  Serial.println("Servo test!");
  /************** UNCOMMENT FUNCTIONS FOR TESTING EACH MOTOR DURING CALIBRATIONT*/
  // jogBase();  //pin 0
  // delay(pauseTime);
  // jogShoulder();  //pin 4
  // delay(pauseTime);
  // jogElbow();  //pin 3
  // delay(pauseTime);
  // jogWrist();  //pin 2
  // delay(pauseTime);
  // jogGripper();  //pin 1
  // delay(pauseTime);
  // robotHoming();  //default start positions - homing
}

void loop() {
}


//move the robot arm to a location at a given speed
void moveSpeed(int joint, int location, int spd) {
  int steps = spd;  //movement steps, make this value 1 to 10 to modify speed, increase to speed up robot
  int current;      //holds the current position of the joint passed to the function
  int start;        //holds i loop variable start value
  Serial.print("joint: ");
  Serial.print(joint);
  Serial.print(" location: ");
  Serial.print(location);
  Serial.print(" speed: ");
  Serial.print(spd);
  Serial.print(" delay ");
  Serial.println(10 * (maxSpeed - spd));

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
        Serial.println(i);
      }
    } else {
      for (int i = start; i >= location; i = i - steps) {
        pwm.writeMicroseconds(joint, i);  //pin 0
        delay(maxSpeed - spd);            //delay movements by robot speed
        Serial.println(i);
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
  Serial.println("Robot homing to default position");
  moveSpeed(base, BUSHome, speed);
  setCurrent(base, BUSHome);  //set joint current positionto final value
  delay(pauseTime);
  moveSpeed(shoulder, SUSHome, speed);
  setCurrent(shoulder, SUSHome);  //set joint current positionto final value
  delay(pauseTime);
  moveSpeed(elbow, EUSHome, speed);
  setCurrent(elbow, EUSHome);  //set joint current positionto final value
  delay(pauseTime);
  moveSpeed(wrist, WUSHome, speed);
  setCurrent(wrist, WUSHome);  //set joint current positionto final value
  delay(pauseTime);
  moveSpeed(gripper, GUSHome, speed);
  setCurrent(gripper, GUSHome);  //set joint current positionto final value
}


//job robot base 0
void jogBase() {  //pin0
  Serial.println("base ccw test");
  for (int i = 0; i < repeats; i++) {
    for (uint16_t microsec = BUSMIN; microsec < BUSMAX; microsec++) {
      pwm.writeMicroseconds(base, microsec);
    }
    delay(pauseTime);

    Serial.println("base cw test");
    for (uint16_t microsec = BUSMAX; microsec > BUSMIN; microsec--) {
      pwm.writeMicroseconds(base, microsec);
    }
    delay(pauseTime);
  }
  pwm.writeMicroseconds(base, BUSHome);  //pin 0
}

//jog robot shoulder 4
void jogShoulder() {  //pin 4
  Serial.println("shoulder cw test");
  for (int i = 0; i < repeats; i++) {
    for (uint16_t microsec = SUSMIN; microsec < SUSMAX; microsec++) {
      pwm.writeMicroseconds(shoulder, microsec);
    }
    delay(pauseTime);

    Serial.println("shoulder ccw test");
    for (uint16_t microsec = SUSMAX; microsec > SUSMIN; microsec--) {
      pwm.writeMicroseconds(shoulder, microsec);
    }
    delay(pauseTime);
  }
  pwm.writeMicroseconds(shoulder, SUSHome);  //pin 4
}

//job robot elbow 3
void jogElbow() {  //pin 3
  Serial.println("elbow cw test");
  for (int i = 0; i < repeats; i++) {
    for (uint16_t microsec = EUSMIN; microsec < EUSMAX; microsec++) {
      pwm.writeMicroseconds(elbow, microsec);
    }
    delay(pauseTime);

    Serial.println("elbow ccw test");
    for (uint16_t microsec = EUSMAX; microsec > EUSMIN; microsec--) {
      pwm.writeMicroseconds(elbow, microsec);
    }
    delay(pauseTime);
  }
  pwm.writeMicroseconds(elbow, EUSHome);  //pin 3
}


//jog robot wrist 2
void jogWrist() {  //pin 2
  Serial.println("wrist cw test");
  for (int i = 0; i < repeats; i++) {
    for (uint16_t microsec = WUSMIN; microsec < WUSMAX; microsec++) {
      pwm.writeMicroseconds(wrist, microsec);
    }
    delay(pauseTime);

    Serial.println("wrist ccw test");
    for (uint16_t microsec = WUSMAX; microsec > WUSMIN; microsec--) {
      pwm.writeMicroseconds(wrist, microsec);
    }
    delay(pauseTime);
  }
  pwm.writeMicroseconds(wrist, WUSHome);  //pin 2
}

//jog robot gripper 1
void jogGripper() {  //pin 1
  Serial.println("gripper cw test");
  for (int i = 0; i < repeats; i++) {
    for (uint16_t microsec = GUSMIN; microsec <= GUSMAX; microsec += 200) {
      pwm.writeMicroseconds(gripper, microsec);
    }
    delay(pauseTime);
    Serial.println("gripper ccw test");

    for (uint16_t microsec = GUSMAX; microsec >= GUSMIN; microsec -= 200) {
      pwm.writeMicroseconds(gripper, microsec);
    }
    delay(pauseTime);
  }
  pwm.writeMicroseconds(gripper, GUSMIN);  //pin 1
}