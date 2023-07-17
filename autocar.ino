//----------------------------------------------------- Initialization
#define trig 3
#define echo 2
int thresholddistance = 30;

long duration;
float forwarddistance;
float distancefront;
float distanceArr[6];

#include <Servo.h>
#define pinServo 4

//----------------------------------------------------- Initialization: Setting up Servo Angles
int servoArray[6] = {0, 25, 50, 110, 145, 180};
Servo myServo;
//----------------------------------------------------- Initialization: Motors
int motor_left[] = {9, 6};
int motor_right[] = {11, 10};
//----------------------------------------------------- Initialization: Setup Module
void setup() {
  Serial.begin(9600);
  myServo.attach(pinServo);
  //Setup Motors
  for (int i = 0; i < 2; i++)
  {
    pinMode(motor_left[i], OUTPUT);
    pinMode(motor_right[i], OUTPUT);
  }
  //Setup Sensor
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

}
//----------------------------------------------------- Initialization: Motor Module
// The following Module is for the Robot to brake
void brake()
{
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_left[1], LOW);
  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], LOW);
}
// The following Module is for the Robot to go forward
void drive_forward()
{
  digitalWrite(motor_left[0], LOW);
  delayMicroseconds(400);
  digitalWrite(motor_left[1], HIGH);
  delayMicroseconds(1000 - 400);
  digitalWrite(motor_right[0], LOW);
  delayMicroseconds(1000 - 400);
  digitalWrite(motor_right[1], HIGH);
  delayMicroseconds(400);
}
// The following Module is for the Robot to slightly go towards left
void slight_left()
{
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_left[1], HIGH);
  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], LOW);
  delay(600);
}
// The following Module is for the Robot to slightly go towards right
void slight_right()
{
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_left[1], LOW);
  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], HIGH);
  delay(600);
}
// The following Module is for the Robot to turn left
void turn_left()
{
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_left[1], HIGH);
  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], LOW);
  delay(900);
}
// The following Module is for the Robot to turn right
void turn_right()
{
  digitalWrite(motor_left[0], LOW);
  digitalWrite(motor_left[1], LOW);
  digitalWrite(motor_right[0], LOW);
  digitalWrite(motor_right[1], HIGH);
  delay(900);
}
// The following Module is to make a uturn if there are obstacles to the right, to the left and in front
void turn_around()
{
  digitalWrite(motor_left[0], HIGH);
  digitalWrite(motor_left[1], LOW);
  digitalWrite(motor_right[0], HIGH);
  digitalWrite(motor_right[1], LOW);
  delay(600);
}
//----------------------------------------------------- Sight Forward
float distance(int angle) {
  myServo.write(angle);
  delay(500);
  digitalWrite(trig, LOW);
  delayMicroseconds(20);
  digitalWrite(trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  forwarddistance = duration / 58;
  return forwarddistance;
}
//----------------------------------------------------- Sensor Array Module
void scan_around() {
  for (int i = 0; i < 6; i++) {
    myServo.write(servoArray[i]);
    delay(100);
    digitalWrite(trig, LOW);
    delayMicroseconds(20);
    digitalWrite(trig, HIGH);
    delayMicroseconds(20);
    digitalWrite(trig, LOW);
    duration = pulseIn(echo, HIGH);
    distanceArr[i] = duration / 58;
    Serial.print("distanceArr[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(distanceArr[i]);
    delay(100);
  }

}
//----------------------------------------------------- Loop Module 
void loop(){
  myServo.write(75);
  distancefront = distance(75);
  Serial.println(distancefront);
  if ((distancefront > thresholddistance) && (distancefront < 1000))
  {
    drive_forward();
    delay(1000);
    Serial.println("1");
  }
  else
  {
    brake();
    scan_around();
    if ((distanceArr[1] > 1000) && (distanceArr[2] > 1000) && (distanceArr[3] > 1000) && (distanceArr[4] > 1000) && (distanceArr[5] > 1000) && (distanceArr[6] > 1000))
    {
      return;
      Serial.println("Restarted the Loop");
      delay(100);
    }
    else if (((distanceArr[1]) > 30.0) && ((distanceArr[2]) > 30.0))
    {
      slight_right();
      drive_forward();
      delay(1000);
      Serial.println("2");
    }
    else if (((distanceArr[0]) > 30.0) && ((distanceArr[1]) > 30.0))
    {
      turn_right();
      drive_forward();
      delay(1000);
      Serial.println("3");
    }
    else if (((distanceArr[3]) > 30.0) && ((distanceArr[4]) > 30.0))
    {
      slight_left();
      drive_forward();
      delay(1000);
      Serial.println("4");
    }
    else if (((distanceArr[4]) > 30.0) && ((distanceArr[5]) > 30.0))
    {
      turn_left();
      drive_forward();
      delay(1000);
      Serial.println("5");
    }
    else
    {
      turn_around();
      brake();
      delay(1000);
      slight_left();
      drive_forward();
      delay(1000);
      Serial.println("6");
    }
  }
}
