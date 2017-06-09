
#include <SoftwareSerial.h>//needed for software serial interface
#define INPUT_SIZE 9//reserve space in memory for input(over wireless module).

//Declare variables for L293D, var name corresponds to pins on the integrated circuit. 
int enableLeft = 10;
int enableRight = 11;
int leftA = 8;
int leftB = 9;
int rightA = 12;
int rightB = 13;

//This pin is reserved for reading the turning calibration device.
int potentiometer = A2;
//This var keeps the potential metre that acts as a calibration device.
double potValue = 0.0;

//This keeps track of movement of vehicle in the last command cycle.
int savedAngle;
int savedSpeed;  

//Setup software serial
SoftwareSerial mySerial(2, 3);

void setup() {
    //Start serial inside arduino
    Serial.begin(115200);//the # of bits per second transfered
    Serial.setTimeout(1);//the time between command cycles
  
    //Start the PC end's serial monitor, monitors wireless module
    mySerial.begin(115200);//the # of bits per second transfered
    mySerial.setTimeout(1);//the time between command cycles
    
    //Enable pins for motors
    pinMode(enableLeft, OUTPUT);
    pinMode(enableRight, OUTPUT);
    
  //Enable pins controlling turning
    pinMode(leftA, OUTPUT);
    pinMode(leftB, OUTPUT);
    pinMode(rightA, OUTPUT);
    pinMode(rightB, OUTPUT);

    //Turn on motors
    digitalWrite(leftA, HIGH);
    digitalWrite(rightA, HIGH);
    
    //Takes calibration value upon starting
    potValue = (analogRead(potentiometer) - 512) / 2048.0;
}//end of setup

void loop() {
  
  //For receiving commands from the wireless module
  if(mySerial.available())//if the wireless module is detected
  {
    //declears size of input on software serial
    char input[INPUT_SIZE];
    byte size = mySerial.readBytes(input, INPUT_SIZE);
    // Add the final 0 to end the C string
    input[size] = 0;
    
    // Read each command pair and stores it
    char* command = strtok(input, "&");
    
    // Split the command into two values, speed and angle
    char* separator = strchr(input, ':');
 
    //Reading the input
    if (separator != 0)
    {
        // Actually split the string in 2: replace ':' with 0
        *separator = 0;
        int speed = atoi(command);
        if(speed >= 1000)//This avoids inputs having extra 0
        {
          speed = speed / 10;
        }
        ++separator;
        int angle = atoi(separator);
        if(angle > 90 || angle < -90)//This avoids inputs having extra 0
        {
          angle = angle/10;
        }  
        drive(speed, angle);//Runes a method that gives movement commands to the robot. 
     }//end if
  }//end if
}//end void loop


void drive(int speed, int angle)//This method gives power to each motor according to the determined speed and angle
{
  //this if avoids executing repeated commands
  if(savedSpeed != speed || savedAngle != angle)
  {
    if (angle == 0 && speed > 0)// moves forward if no turning input
    {
      analogWrite(enableLeft, 255);
      analogWrite(enableRight, 255);
    }
    else if (angle < 0)//turns left
    {
      analogWrite(enableLeft, 10);
      analogWrite(enableRight, speed - speed/255 * angle* (2.0 - potValue));
    }
    else//turn right
    {
      analogWrite(enableLeft, speed + speed/255 * angle * (2.0 + potValue));
      analogWrite(enableRight, 10);
    }//end if
    //stores this cycle's speed and angle values
    savedSpeed = speed;
    savedAngle = angle;
  }//end if
}//end drive
