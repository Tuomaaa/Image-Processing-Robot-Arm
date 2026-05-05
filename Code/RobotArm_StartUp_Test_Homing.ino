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
#define USMID 1500        //base of the robot
#define USMAX 2400        // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600 BASE
#define WUSMIN 800        // wristmin
#define WUSHome 1400      //wristhome
#define WUSMAX 2000       // wrist max
#define SUSHome 600       //shoulder home
#define SUSMIN 1000       //shoulder min
#define SUSMAX 1800       //shoulder max
#define EUSHome 2400      //elbow home
#define EUSMIN 800        //elbow minManage Submissions
#define EUSMAX 2000       //elbow max
#define GUSMIN 600        //gripper vertical
#define GUSMAX 1300       //gripper horizontal
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
  jogBase();      //pin 0
  //jogShoulder();  //pin 4
  //jogElbow();       //pin 3
  jogWrist();       //pin 2
  jogGripper();     //pin 1
  robotHoming();  //default start positions - homing
}

void loop() {
}

void robotHoming() {
  Serial.println("Robot homing to default position");
  pwm.writeMicroseconds(base, USMID);        //pin 0
  pwm.writeMicroseconds(shoulder, SUSHome);  //pin 4
  pwm.writeMicroseconds(elbow, EUSHome);     //pin 3
  pwm.writeMicroseconds(wrist, WUSHome);     //pin 2
  pwm.writeMicroseconds(gripper, GUSMIN);    //pin 1
}

void jogBase() {  //pin0
  Serial.println("base ccw test");
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
  pwm.writeMicroseconds(base, USMID);  //pin 0
}

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
