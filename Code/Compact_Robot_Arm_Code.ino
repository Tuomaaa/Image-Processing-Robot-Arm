/*Modified CompactRobotArmCode.ino
CABerry 01072026
  pulselength = map(degrees, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWMFreq(1000) //freq: A number representing the frequency in Hz, between 40 and 1600
  setPWM(channel, on, off) //channel: The channel that should be updated with the new values (0..15), Adafruit 16-Channel 12-bit PWM/Servo Driver with I2C interface
Adafruit 16-Channel 12-bit PWM/Servo Driver - I2C interface
$14.95

  on: The tick (between 0..4095) when the signal should transition from low to high
  off:the tick (between 0..4095) when the signal should transition from high to low
  pwm.setPWM(15, 1024, 3072)
*/


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define MIN_PULSE_WIDTH 650
#define MAX_PULSE_WIDTH 2350
#define FREQUENCY 50
#define MAXFREQUENCY 1600
#define baudrate 9600

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

 //Assign Motors to pins on Servo Driver Board
int base = 0;
int hand = 1;
int wrist = 2;
int elbow = 3; 
int shoulder = 4;

void setup() {
  Serial.begin(baudrate);
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
  //pwm.setPWM(base, 4096, 0);
  //delay(1000);
  //pwm.setPWM(base, 0, 4096);
  //delay(1000);
  pwm.setPWM(base, 0, 90);  //Set Gripper to 90 degrees (Close Gripper)
}

void loop() {
  //   for(int i = 0; i <= 100; i=i+10) {
  //     moveMotor(i, base);
  //     delay(100);
  //   }

  // moveMotor(position, wrist);
  // moveMotor(position, elbow);
  // moveMotor(position, shoulder);
  // int pushButton = digitalRead(13);
  // if(pushButton == LOW)
  // {
  //   pwm.setPWM(hand, 0, 180);                             //Keep Gripper closed when button is not pressed
  //   Serial.println("Grab");
  // }
  // else
  // {
  //   pwm.setPWM(hand, 0, 90);                              //Open Gripper when button is pressed
  //   Serial.println("Release");
  // }
}

void moveMotor(int pos, int motorOut) {
  int pulse_wide, pulse_width;

  pulse_wide = map(pos, 0, 100, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  pulse_width = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);  //Map Potentiometer position to Motor

  pwm.setPWM(motorOut, 0, pulse_width);
}