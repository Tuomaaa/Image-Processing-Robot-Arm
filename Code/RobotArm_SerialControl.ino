/*************************************************** 
CAB 2/2/26 - Modified for Serial Control
Robot Arm SERVO CONTROL
5 DOF Robot (base - 0, shoulder - 4, elbow - 3, wrist - 2, gripper - 1)
Input format: servo_pin microseconds (e.g. "0 1500")
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_FREQ 50
#define baudrate 9600
#define oscFreq 27000000
#define numOfservo 5

// servo pin mapping
#define base 0
#define shoulder 4
#define elbow 3
#define wrist 2
#define gripper 1

// min/max limits (microseconds)
const uint16_t servoMin[] = { 600, 600, 800, 1000, 1000 };   // pin 0,1,2,3,4
const uint16_t servoMax[] = { 1800, 1300, 2000, 2000, 1800 }; // pin 0,1,2,3,4

void setup() {
  Serial.begin(baudrate);
  pwm.begin();
  pwm.setOscillatorFrequency(oscFreq);
  pwm.setPWMFreq(SERVO_FREQ);
  delay(1000);

  Serial.println("=== Robot Arm Serial Control ===");
  Serial.println("Format: pin microseconds");
  Serial.println("Example: 0 1500");
  Serial.println("Servo mapping:");
  Serial.println("  0 = base     (600-1800)");
  Serial.println("  1 = gripper  (600-1300)");
  Serial.println("  2 = wrist    (800-2000)");
  Serial.println("  3 = elbow    (1000-2000)");
  Serial.println("  4 = shoulder (1000-1800)");
  Serial.println("Ready.");
}

void loop() { 
  if (Serial.available()) {
    int pin = Serial.parseInt();
    int us = Serial.parseInt();

    // clear leftover newline
    while (Serial.available() && Serial.read() != '\n');

    if (pin < 0 || pin > 4) {
      Serial.print("Invalid pin: ");
      Serial.println(pin);
      return;
    }



    pwm.writeMicroseconds(pin, us);
    Serial.print("Pin ");
    Serial.print(pin);
    Serial.print(" -> ");
    Serial.print(us);
    Serial.println(" us");
  }
}
