#include <SoftwareSerial.h>
#define light A2
#define Ecoh1 13
#define Trig1 12
#define Ecoh2 A1
#define Trig2 A0
#define Ecoh3 17
#define Trig3 16
#define button 14
#define LeftupMotor1 7   
#define LeftupMotor2 8    
#define RightupMotor1 5   
#define RightupMotor2 6   
#define LeftdownMotor1 2  
#define LeftdownMotor2 9  
#define RightdownMotor1 3  
#define RightdownMotor2 4 
#define distance 30
#define v 90
#define turn_v 100
SoftwareSerial blue(10, 11);  // RX, TX
float cm1 = 0.0;
float cm2 = 0.0;
float cm3 = 0.0;
int buttonstate = 1;
int buttonswitch = false;
void turn_Left() {
  digitalWrite(LeftupMotor1, LOW);
  analogWrite(LeftupMotor2, turn_v);
  analogWrite(RightupMotor1, turn_v);
  digitalWrite(RightupMotor2, LOW);
  digitalWrite(LeftdownMotor1, LOW);
  analogWrite(LeftdownMotor2, turn_v);
  analogWrite(RightdownMotor1, turn_v);
  digitalWrite(RightdownMotor2, LOW);
  digitalWrite(light, HIGH);
  delay(65);
}

void turn_Right() {
  analogWrite(LeftupMotor1, turn_v);
  digitalWrite(LeftupMotor2, LOW);
  digitalWrite(RightupMotor1, LOW);
  analogWrite(RightupMotor2, turn_v);
  analogWrite(LeftdownMotor1, turn_v);
  digitalWrite(LeftdownMotor2, LOW);
  digitalWrite(RightdownMotor1, LOW);
  analogWrite(RightdownMotor2, turn_v);
  digitalWrite(light, HIGH);
  delay(65);
}

void go() {
  analogWrite(LeftupMotor1, v);
  digitalWrite(LeftupMotor2, LOW);
  analogWrite(RightupMotor1, v);
  digitalWrite(RightupMotor2, LOW);
  analogWrite(LeftdownMotor1, v);
  digitalWrite(LeftdownMotor2, LOW);
  analogWrite(RightdownMotor1, v);
  digitalWrite(RightdownMotor2, LOW);
}

void stop() {
  digitalWrite(LeftupMotor1, LOW);
  digitalWrite(LeftupMotor2, LOW);
  digitalWrite(RightupMotor1, LOW);
  digitalWrite(RightupMotor2, LOW);
  digitalWrite(LeftdownMotor1, LOW);
  digitalWrite(LeftdownMotor2, LOW);
  digitalWrite(RightdownMotor1, LOW);
  digitalWrite(RightdownMotor2, LOW);
  digitalWrite(light, HIGH);
}

void back() {
  digitalWrite(LeftupMotor1, LOW);
  analogWrite(LeftupMotor2, v);
  digitalWrite(RightupMotor1, LOW);
  analogWrite(RightupMotor2, v);
  digitalWrite(LeftdownMotor1, LOW);
  analogWrite(LeftdownMotor2, v);
  digitalWrite(RightdownMotor1, LOW);
  analogWrite(RightdownMotor2, v);
}
void setup() {
  // put your setup code here, to run once:

  // Set pin mode to output
  pinMode(LeftupMotor1, OUTPUT);
  pinMode(LeftupMotor2, OUTPUT);
  pinMode(RightupMotor1, OUTPUT);
  pinMode(RightupMotor2, OUTPUT);
  pinMode(LeftdownMotor1, OUTPUT);
  pinMode(LeftdownMotor2, OUTPUT);
  pinMode(RightdownMotor1, OUTPUT);
  pinMode(RightdownMotor2, OUTPUT);
  pinMode(Ecoh1, INPUT);
  pinMode(Trig1, OUTPUT);
  pinMode(Ecoh2, INPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Ecoh3, INPUT);
  pinMode(Trig3, OUTPUT);
  pinMode(button, INPUT);
  Serial.begin(9600);  //Initialize Arduino's hard serial port
  blue.begin(9600);    // Initialize communication between soft serial port and Bluetooth module
}
void loop() {

  // Trigger ultrasonic sensor 1
  digitalWrite(light, LOW);
  digitalWrite(Trig1, LOW);  // Low level 2 subtle to trigger foot
  delayMicroseconds(2);
  digitalWrite(Trig1, HIGH);  // Give the trigger pin a high level of 10 microseconds
  delayMicroseconds(10);
  digitalWrite(Trig1, LOW);  // Continuously low level to trigger pin
  cm1 = pulseIn(Ecoh1, HIGH) / 58.0;

  // Trigger ultrasonic sensor 2
  digitalWrite(Trig2, LOW);  
  delayMicroseconds(2);
  digitalWrite(Trig2, HIGH);  
  delayMicroseconds(10);
  digitalWrite(Trig2, LOW);  
  cm2 = pulseIn(Ecoh2, HIGH) / 58.0;

  // Trigger ultrasonic sensor 3
  digitalWrite(Trig3, LOW);  
  delayMicroseconds(2);
  digitalWrite(Trig3, HIGH);  
  delayMicroseconds(10);
  digitalWrite(Trig3, LOW);  
  cm3 = pulseIn(Ecoh3, HIGH) / 58.0;


  String distanceData = String(cm1) + "," + String(cm2) + "," + String(cm3);
  blue.println(distanceData);
  buttonstate = digitalRead(button);
  delay(100);
  if(buttonstate == LOW){
    buttonswitch = !buttonswitch;
  }
  if(buttonswitch){
    delay(100);
  // Eight scenarios in total
  // Scenario 1: Only right turns allowed
  if (cm1 <= distance && cm2 <= distance && cm3 > distance) {
    turn_Right();
  }
  // Direct route
  else if (cm1 > distance && cm2 <= distance && cm3 <= distance) {
    go();
  }
  // Only allow left turns
  else if (cm1 <= distance && cm2 > distance && cm3 <= distance) {
    turn_Left();
  }
  // Enter the end of the maze and make a left turn
  else if (cm1 <= distance && cm2 <= distance && cm3 <= distance) {
    stop();
    turn_Left();
  }
  // Left T-junction, go straight
  else if (cm1 > distance && cm2 > distance && cm3 <= distance) {
    go();
  }
  // Right T-junction, go straight
  else if (cm1 > distance && cm2 <= distance && cm3 > distance) {
    go();
  }
  // T-junction, turn right
  else if (cm1 <= distance && cm2 > distance && cm3 > distance) {
    turn_Right();
  } else {
    go();
  }
  }
  else{
    stop();
  }
}
