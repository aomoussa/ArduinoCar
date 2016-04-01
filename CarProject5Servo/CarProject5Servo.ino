#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Wire.h>
#include<Servo.h>

#include <L3G4200D.h>;
Servo myServo;
int switchPin = 22;
int servoPWMPin = 9;
int motorPWMPin = 12;
int blinkingLEDPin = 13;
int motorPower = 15;
int numberOfLoops = 0;
double VELOCITY = 0.595;//0.495;
L3G4200D gyro;
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);


void setup() {
    //exit(0);
    myServo.attach(servoPWMPin);
    Serial.begin(9600);
    
    if(!mag.begin())
    {
        /* There was a problem detecting the HMC5883 ... check your connections */
        Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
        while(1);
    }
    gyro.enableDefault();
    delay(10000);
    circles(1,2);
}

void loop() {
}

void circles(double radius1, double radius2){
  fullCirclecw(radius1);
  fullCircleccw(radius2);

}
void fullCirclecw(double radius)
{
    double startPoint = getHeading();  //this gets our initial value with the compass
    double angVel = -(VELOCITY/radius)*(180/PI); //defines our angular velocity for the circle
    int carAngle = 100;
    double feedAngVel = 0;
    int delay1 = 50;
    double degreesSoFar = 0;
    // we need to define carAngle to an approximate value
    writeCarServo(carAngle);
    //using radius here it doesnt have to be exact, you know why.
    // Serial.print(angVel);    Serial.println(" : our angVel");
    delay(5000);
    analogWrite(motorPWMPin, motorPower);
    
    Serial.println("right before loops");
    while(1)//
    {
        delay(delay1);
        gyro.read();
        feedAngVel = (gyro.g.z)*(8.75/1000); //reading angular velocity
        degreesSoFar += feedAngVel*(delay1)/1000;
        Serial.print(degreesSoFar);    Serial.println(" : dso");
        
        if(degreesSoFar <= -360){
            break;
        }
        if (feedAngVel < angVel) //if the angular velocity is too high adjust
        { //honestly just run the car with these and see which way it needs to be adjusted
            //too sharp cw
            if(carAngle > 90)
            {
                carAngle--;
                writeCarServo(carAngle);
            }
        }
        else if (feedAngVel > angVel) //if the angular velocity is too low adjust
        {
            if(carAngle < 165)
            {
                carAngle++;
                writeCarServo(carAngle);
            }
            
        }
    }
    analogWrite(motorPWMPin, 0);
    
    
}
void fullCircleccw(double radius)
{
    double startPoint = getHeading();  //this gets our initial value with the compass
    double angVel = (VELOCITY/radius)*(180/PI); //defines our angular velocity for the circle
    int carAngle = 80;
    double feedAngVel = 0;
    int delay1 = 50;
    double degreesSoFar = 0;
    // we need to define carAngle to an approximate value
    writeCarServo(carAngle);
    //using radius here it doesnt have to be exact, you know why.
    // Serial.print(angVel);    Serial.println(" : our angVel");
    delay(1000);
    analogWrite(motorPWMPin, motorPower);
    
    Serial.println("right before loops");
    while(1)//
    {
        delay(delay1);
        gyro.read();
        feedAngVel = (gyro.g.z)*(8.75/1000); //reading angular velocity
        degreesSoFar += feedAngVel*delay1/1000;
        Serial.print(degreesSoFar);    Serial.println(" : dso");
        if(degreesSoFar >= 360){
            break;
        }
        if (feedAngVel<angVel) //if the angular velocity is too high adjust
        { //honestly just run the car with these and see which way it needs to be adjusted
            //too not sharp ccw
            if(carAngle > 15)
            {
                carAngle--;
                writeCarServo(carAngle);
            }
        }
        else if (feedAngVel > angVel) //if the angular velocity is too low adjust
        {
            if(carAngle < 90)
            {
                carAngle++;
                writeCarServo(carAngle);
            }
            
        }
    }
    analogWrite(motorPWMPin, 0);
    
    
}
double getHeading() //you know what it iiiiiiiissssss
{
    sensors_event_t event;
    mag.getEvent(&event);
    float heading = atan2(event.magnetic.y, event.magnetic.x);
    //declination in tempe = 10 degrees positive east
    float declinationAngle = 0.19;
    heading += declinationAngle;
    
    if(heading < 0)
        heading += 2*PI;
    
    if(heading > 2*PI)
        heading -= 2*PI;
    
    return (heading * 180/M_PI);
}

void writeCarServo(int servoValue) // classic servo function
{
    myServo.write(servoValue);
}

void straightGUNNIN()
{
    myServo.write(90);
    delay(2000);
    analogWrite(motorPWMPin, motorPower);
    delay(6000);
    analogWrite(motorPWMPin, 0);
}
