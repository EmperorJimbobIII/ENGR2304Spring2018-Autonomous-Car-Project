//Most of this program was taken from example code provided by the instructor on CANVAS

#include <NewPing.h>  // This is using a update to the ping 
// See this website.  Add the library to your librarys folder C:\Program Files (x86)\Arduino\libraries
// http://playground.arduino.cc/Code/NewPing



class Drive 
{
 private:
      int E1;  // Passengers Side
      int M1; // Forward
      int M1_R; // Reverse 
      int E2;    //Drivers Side                  
      int M2; // Forward
      int M2_R; // Reverse
      int M1_PWM;  
      int M2_PWM;
 public:
      void Forward(void); // Taken from existing program provided by instructor 
      void TurnLeft(void); // Annie
      void TurnRight(void); //Annie
      void Backward(void); // Dominic
      void Stop(void); // James
       Drive(void); // Group
};

//Constructor for the Ultrasonic distance finder to create an instance sonar as a global
NewPing sonarF(5, 6, 200);
NewPing sonarR(4, 3, 200);
NewPing sonarL(2, 11, 200);

             
bool stopped = 1; //Trigger that the motors are stopped.

void setup() 
{ 
Serial.begin(9600);
Drive setupDrive();
} 

void loop() //Note, loop repeats infinitely
{ 
  Drive MyDrive; //set up default constructor and initialize motor names and values
  // Variables to store the ping distance.  Distance is in cm
  int pingF_dist;
  int pingL_dist;
  int pingR_dist;
  if (stopped)  //If it is not stopped and the sonar is not triggered, it is moving forward and will stay moving forward
  {
    MyDrive.Forward();
    stopped=0;
  }
  // Both engines are moving forward at this point
  delay(50); // 50 ms delay --- This is a delay to make sure that the sonar has had time to reset.  Some delay here is typically needed
  
  pingF_dist=sonarF.ping_cm(); // Get a ping value for the forward sensor
  Serial.println(pingF_dist);


  /*
   * Add logic using if statements to determine which direction the vehicle should turn
   */
  while (pingF_dist>0&&pingF_dist<30) // If ping is greater than 0 (that is non-zero), but less than 30, trigger a change angle
  {
    MyDrive.Stop();
    pingL_dist=sonarL.ping_cm(); // Get a ping value for the left sensor 
    if (pingL_dist>0&&pingL_dist<15) //if the left side is blocked
    {
      pingR_dist=sonarR.ping_cm(); //Get a ping value for the right sensor

      if (pingR_dist>0&&pingR_dist<15) //if the right side is blocked
      {
        MyDrive.Backward(); // backup and turn right
        delay(200); //wait to be clear of obstacle
        MyDrive.TurnRight();
        }
    
      }
    else //if the left side is clear
    {
      MyDrive.TurnLeft(); //turn left, we don't care about what's going on on the right
      MyDrive.Stop();
      delay(1000);
      }      
    
    MyDrive.Backward(); //Just for testing purposes, remove or change location later  
    delay(100); //This delay of 0.1 second is in here just to make it easier to see what the motor is doing.
    //This delay is completely unecessary.
    MyDrive.Forward();
    MyDrive.TurnRight();
    MyDrive.Stop();
    stopped=1; // motor was stopped
    pingF_dist=sonarF.ping_cm(); // Get a new ping value for the front 
    pingR_dist=sonarR.ping_cm(); // Get a new ping value for the passengers side
    pingL_dist=sonarL.ping_cm(); // Get a new ping value for the drivers side
    Serial.println(pingF_dist); 
  }  


}

 void Drive::Forward(void)
{
    Serial.println("Forward");
    analogWrite(E1,0); //Speed 0 Motor 1
    analogWrite(E2,0); //Speed 0 Motor 2
    digitalWrite(M1_R,LOW); //Turn off reverse, to make sure
    digitalWrite(M2_R,LOW); //Turn off reverse, to make sure
    digitalWrite(M1,HIGH);  //Motor 1 forward on 
    digitalWrite(M2, HIGH); //Motor 2 forward on
    //You could replace the for loop with just analogWrite(E1,M1_PWM); analogWrite(E2,M2_PWM) 
    for (int i(0);i<101;i++) //This is not really necessary for this system.  But, for a large machine, you would need to ramp up speed gradually
    {
      analogWrite(E1,M1_PWM*i/100); //Ramp motor 1 speed up
      analogWrite(E2,M2_PWM*i/100); //Ramp motor 2 speed up
      delay(10);
    }
  return;
}

void Drive::Backward(void) //Dominic
{
    Serial.println("Backwards");
    analogWrite(E1,0); //Speed 0 Motor 1
    analogWrite(E2,0); //Speed 0 Motor 2
    digitalWrite(M1,LOW); //Turn off forward, to make sure
    digitalWrite(M2,LOW); //Turn off forward, to make sure
    digitalWrite(M1_R,HIGH);  //Motor 1 backward on 
    digitalWrite(M2_R,HIGH); //Motor 2 backward on
    //You could replace the for loop with just analogWrite(E1,M1_PWM); analogWrite(E2,M2_PWM) 
    for (int i(0);i<101;i++) //This is not really necessary for this system.  But, for a large machine, you would need to ramp up speed gradually
    {
      analogWrite(E1,M1_PWM*i/100); //Ramp motor 1 speed up
      analogWrite(E2,M2_PWM*i/100); //Ramp motor 2 speed up
      delay(10);
    }
  return; // added for backward motion
}
void Drive::Stop(void)
{
    Serial.println("Stop");
      for (int i(100);i<0;i--) // Again, you could just set speed to zero for this small system, but for a large system, this ramp down would be needed
    {
      analogWrite(E1,M1_PWM*i/100); //Ramp motor 1 speed down
      analogWrite(E2,M2_PWM*i/100); //Ramp motor 2 speed down
      delay(10);
    }
    digitalWrite(M1_R,LOW); //Turn off reverse
    digitalWrite(M2_R,LOW); //Turn off reverse
    digitalWrite(M1,LOW);  //Turn off forward
    digitalWrite(M2, LOW); //Turn off forward
  return;
}

void Drive::TurnRight(void)
{
    digitalWrite(M1_R,HIGH); //turn (Motor 1 in reverse motor 2 forward
    digitalWrite(M2,HIGH);
    
    
        for (int i(0);i<101;i++) //Ramp up speed
    {
      analogWrite(E1,M1_PWM*i/100); //Ramp motor 1 speed down
      analogWrite(E2,M2_PWM*i/100); //Ramp motor 2 speed down
      delay(10);
    }

}


void Drive::TurnLeft(void)
{
    digitalWrite(M1,HIGH); //turn (Motor 1 in reverse motor 2 forward
    digitalWrite(M2_R,HIGH);
    
    
        for (int i(0);i<101;i++) //Ramp up speed

    {
      analogWrite(E1,M1_PWM*i/100); //Ramp motor 1 speed down
      analogWrite(E2,M2_PWM*i/100); //Ramp motor 2 speed down
      delay(10);
    }

    delay(50); //This is the time for turning at full speed.  Note that as your batteries run out, the turns will get shorter.
  }

 Drive::Drive()
  {
      //This is for the L298N motor controller.  The values are the pin locations
      //that you use to connect the ENA(E1), IN1(M1), IN2(M1_R), ENA(E2), IN4(M2), and IN3(M2_R)
      //ENA and ENB are the variable power for the motor
      //IN1 and IN2 are on/off for forward or reverse for motor A
      //IN4 and IN3 are on/off for forward or reverse for motor B
      //If one or both motors are running backwards, you can switch the power to the motor (OUT1 and OUT2 or OUT3 and OUT4)
      //Or, you can switch the pin connections
      //Or, in the code below, you could switch the pin numbers.
      //Try it all and see what works best for you.
      
      E1 = 10;  // Passengers Side
      M1 = 13; // Forward
      M1_R = 12; // Rev 
      E2 = 9;    //Drivers Side                  
      M2 = 8; // Forward
      M2_R = 7; // Rev

      //Motor speed calibration.  Run it on the floor with both values at 255
      //It will curve left or right.  Curving left means that the right motor is going faster.
      //Curving to the right means the left motor is going faster.
      //Leave your slower motor at 255.  Change the value down for you faster motor.
      //Repeat until it runs straight (unless you wanted it to run in big circles).
      M1_PWM=255; // Passenger side with respect to the ultrasonic sensor
      M2_PWM=255; // Drivers side with respect to the ultrasonic sensor
    }
