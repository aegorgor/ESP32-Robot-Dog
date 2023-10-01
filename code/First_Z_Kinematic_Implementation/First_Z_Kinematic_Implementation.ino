// Include Wire Library for I2C
#include <Wire.h>

// Include Adafruit PCA9685 Servo Library
#include <Adafruit_PWMServoDriver.h>

// Creat object to represent PCA9685 at default I2C address
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  700  // Minimum value
#define SERVOMAX  120  // Maximum value

//3 joints -> abductor, hip, knee
// Define servo motor connections (expand as required)


#define aVertPos 103 //103 degrees corredponds to a vertical abductor position

#define aSER0  0   //Servo Motor 0 on connector 0
#define hSER0  1   //Servo Motor 0 on connector 0
#define kSER0  2   //Servo Motor 0 on connector 0

#define aSER1  4   //Servo Motor 0 on connector 0
#define hSER1  5   //Servo Motor 0 on connector 0
#define kSER1  6   //Servo Motor 0 on connector 0

#define aSER2  8   //Servo Motor 0 on connector 0
#define hSER2  9   //Servo Motor 0 on connector 0
#define kSER2  10  //Servo Motor 0 on connector 0

#define aSER3  12  //Servo Motor 0 on connector 0
#define hSER13  13 //Servo Motor 0 on connector 0
#define kSER3  14 //Servo Motor 0 on connector 0

// Variables for Servo Motor positions (expand as required)
int pwm0;
int pwm1;

void setup() {

  // Serial monitor setup
  Serial.begin(115200);

  // Print to monitor
  Serial.println("PCA9685 Servo Test");

  // Initialize PCA9685
  pca9685.begin();

  // Set PWM Frequency to 50Hz
  pca9685.setPWMFreq(80);

}

void loop() {
    pwm0 = map(180, 0, 180, SERVOMIN, SERVOMAX);
    // Write to PCA9685
    pca9685.setPWM(aSER0, 0, 600);
/*
  // Move Motor 0 from 0 to 180 degrees
  for (int posDegrees = 0; posDegrees <= 180; posDegrees++) {

    // Determine PWM pulse width
    pwm0 = map(posDegrees, 0, 180, SERVOMIN, SERVOMAX);
    // Write to PCA9685
    pca9685.setPWM(aSER0, 0, pwm0);
    pwm1 = map(posDegrees, 0, 180, SERVOMAX, SERVOMIN);

    pca9685.setPWM(aSER1, 0, pwm1);
    pca9685.setPWM(aSER2, 0, pwm0);
    pca9685.setPWM(aSER3, 0, pwm1);


    // Print to serial monitor
    Serial.print("Motor 0 = ");
    Serial.println(posDegrees);
    delay(40);
  }
  delay(50000);
  */
/*
  for (int posDegrees = 20; posDegrees >= 0; posDegrees--) {

    // Determine PWM pulse width
    pwm0 = map(posDegrees, 0, 180, SERVOMIN-210, SERVOMAX);
    // Write to PCA9685
    pca9685.setPWM(hSER0, 0, pwm0);
    // Print to serial monitor
    Serial.print("Motor 0 = ");
    Serial.println(posDegrees);
    delay(15);
  }/*
  // Move Motor 0 from 180 to 0 degrees
  for (int posDegrees = 20; posDegrees >= 0; posDegrees--) {

    // Determine PWM pulse width
    pwm0 = map(posDegrees, 0, 180, SERVOMIN-180, SERVOMAX);
    // Write to PCA9685
    pca9685.setPWM(hSER0, 0, pwm0);
    // Print to serial monitor
    Serial.print("Motor 0 = ");
    Serial.println(posDegrees);
    delay(15);
  }

  for (int posDegrees = 20; posDegrees >= 0; posDegrees--) {

    // Determine PWM pulse width
    pwm1 = map(posDegrees, 0, 180, SERVOMIN-210, SERVOMAX);
    // Write to PCA9685
    pca9685.setPWM(SER1, 0, pwm1);
    // Print to serial monitor
    Serial.print("Motor 0 = ");
    Serial.println(posDegrees);
    delay(15);
  }*/

}
