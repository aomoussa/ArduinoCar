#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <L3G4200D.h>;
#include <Wire.h>
/* initiate compass sensor with unique id: 12345 */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
L3G4200D gyro;

#include<Servo.h>

Servo myServo;
int switchPin = 22;
int servoPWMPin = 9;
int motorPWMPin = 12;
int blinkingLEDPin = 13;
int motorPower = 10;
int numberOfLoops = 0;

//--------------- -------------- SENSOR STUFF ---------- ---------- begins
int cntr = 0;
double VELOCITY = 1;//define me;
//--------------- -------------- SENSOR STUFF ---------- ---------- ends

void setup() {
  //--------------- -------------- CAR STUFF ---------- ---------- begins
    myServo.attach(servoPWMPin);
    pinMode(motorPWMPin, OUTPUT);
  //--------------- -------------- CAR STUFF ---------- ---------- ends
  
  //--------------- -------------- SENSOR STUFF ---------- ---------- begins
  Serial.begin(9600);
   if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
  Wire.begin();
  gyro.enableDefault();
  //--------------- -------------- SENSOR STUFF ---------- ---------- ends
  fullCircle(2.0);
}

void loop() {
  // put your main code here, to run repeatedly:

}
double getHeading() //you know what it iiiiiiiissssss
{
  sensors_event_t event; 
  mag.getEvent(&event);
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  /*
   //declination in tempe = 10 degrees positive east 
  float declinationAngle = 0.19;
  heading += declinationAngle;
  */
  //ignore for now

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

void fullCircle(double radius)
{   
    double initialHeadingDegrees = getHeading();  //this gets our initial value with the compass  
    double angVel = -(VELOCITY/radius)*(180/PI); //defines our angular velocity for the circle
    double carAngle = 100;
    // we need to define carAngle to an approximate value
    writeCarServo(carAngle);
    //using radius here it doesnt have to be exact, you know why.
  
    analogWrite(motorPWMPin, motorPower);
    delay(2000);
 
    
    while(1)// 
    {
      bool servoHasInitialHeading = (getHeading() < initialHeadingDegrees+5 && getHeading() > initialHeadingDegrees-5);
      if(servoHasInitialHeading){
        //break;
      }
      gyro.read(); 
      double feedAngVel = gyro.g.z*(8.75/1000); //reading angular velocity
  
  Serial.print("Servo angle input: ");Serial.println(carAngle);
           Serial.print("feedAngVel: ");Serial.println(feedAngVel);
           Serial.print("angVel: ");Serial.println(angVel);
      if (feedAngVel > angVel) //if the angular velocity is too high adjust
        { //honestly just run the car with these and see which way it needs to be adjusted
          if(carAngle < 125)
          {
            carAngle++;
            writeCarServo(carAngle);
          }
        }
        else if (feedAngVel < angVel) //if the angular velocity is too low adjust
        {
           if(!(carAngle < 70))
          {
            carAngle--;
            writeCarServo(carAngle);
            
          }
     
        }       
      }   
  analogWrite(motorPWMPin, 0);
  
}
