/*************************************************** 
CAB 2/2/26
Robot Arm SERVO TEST
5 DOF Robot (base - 0, shoulder - 4, elbow - 3, wrist - 2, gripper - 1)
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

/*called this way, it uses the default address 0x40
you can also call it with a different address you want Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
you can also call it with a different address and I2C interface Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);
*/
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define USMIN 600         // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150 BASE
#define USMID 1500        //base of the robot range is 600 - 2400
#define BUSHome 600      //base home
#define USMAX 1800        // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600 BASE
#define WUSMIN 800        // wristmin
#define WUSHome 1400      //wristhome
#define WUSMAX 2000       // wrist max
#define SUSHome 1000       //shoulder home
#define SUSMIN 1000       //shoulder min
#define SUSMAX 1800       //shoulder max
#define EUSHome 1000      //elbow home
#define EUSMIN 1000        //elbow min
#define EUSMAX 2000       //elbow max
#define GUSMIN 600        //gripper vertical
#define GUSMAX 1300       //gripper horizontal
#define GUSHome 600       //gripper home
#define SERVO_FREQ 50     // Analog servos run at ~50 Hz updates
#define baudrate 9600     //servo communication baudrate
#define oscFreq 27000000  //oscilator frequency
#define pauseTime 3000    //pause between test cycles
#define repeats 1

uint8_t servonum = 0;  // our servo # counter
#define numOfservo 5   //number of servo
#define base 0         // robot base = 0
#define shoulder 4     // robot shoulder = 1
#define elbow 3        // robot elbow = 3
#define wrist 2        // robot wriset = 4
#define gripper 1      // robot gripper = 5

void setup() {
  Serial.begin(baudrate);
  pwm.begin();
  pwm.setOscillatorFrequency(oscFreq);  //set servo motor oscillator frequency
  pwm.setPWMFreq(SERVO_FREQ);           // Analog servos run at ~50 Hz updates
  delay(pauseTime);
  Serial.println("Servo test!");
    delay(pauseTime);

  jogBase();        //pin 0
  jogShoulder();    //pin 4
  jogElbow();       //pin 3
  jogWrist();       //pin 2
  jogGripper();     //pin 1
  robotHoming();    //default start positions - homing
}

void loop() {
}

void robotHoming() {
  Serial.println("Robot homing to default position");
  pwm.writeMicroseconds(base, BUSHome);       //pin 0 - base
  pwm.writeMicroseconds(shoulder, SUSHome);   //pin 4 - shoulder
  pwm.writeMicroseconds(elbow, EUSHome);      //pin 3 - elbow
  pwm.writeMicroseconds(wrist, WUSHome);      //pin 2 - wrist
  pwm.writeMicroseconds(gripper, GUSHome);    //pin 1 - gripper
}

void jogBase() {  //pin0 - base
  Serial.println("base ccw test");
  /*
  for (int i = 0; i < repeats; i++) {
    for (uint16_t microsec = USMIN; microsec < USMAX; microsec++) {
      pwm.writeMicroseconds(base, microsec);
    }
    delay(pauseTime);

    Serial.println("base cw test");
    for (uint16_t microsec = USMAX; microsec > USMIN; microsec--) {
      pwm.writeMicroseconds(base, microsec);
    }
    delay(pauseTime);
  }
  pwm.writeMicroseconds(base, BUSHome);  //pin 0
  */
      Serial.println("base test");

  pwm.writeMicroseconds(base, USMAX);
      delay(pauseTime);

  pwm.writeMicroseconds(base, USMIN);
      delay(pauseTime);

}

void jogShoulder() {  //pin 4 - shoulder
/*
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
  Serial.println("base cw test");
*/
  Serial.println("shoulder test");

  pwm.writeMicroseconds(shoulder, SUSMAX);
      delay(pauseTime);

  pwm.writeMicroseconds(shoulder, SUSMIN);
      delay(pauseTime);
}

void jogElbow() {  //pin 3 - elbow
/*
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

        Serial.println("base cw test");
*/
      Serial.println("elbow test");

  pwm.writeMicroseconds(elbow, EUSMAX);
      delay(pauseTime);

  pwm.writeMicroseconds(elbow, EUSMIN);
      delay(pauseTime);
}

void jogWrist() {  //pin 2 - wrist
/*
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
        Serial.println("base cw test");
*/
  pwm.writeMicroseconds(wrist, WUSMAX);
      delay(pauseTime);

  pwm.writeMicroseconds(wrist, WUSMIN);
      delay(pauseTime);
}

void jogGripper() {  //pin 1 - gripper
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
