
#include <SoftwareSerial.h>
#define INPUT_SIZE 9

//Declare variables for L293D 
int enableLeft = 10;
int enableRight = 11;
int leftA = 8;
int leftB = 9;
int rightA = 12;
int rightB = 13;

int potentiometer = A2;
  
double potValue = 0.0;

//Setup software serial
SoftwareSerial mySerial(2, 3);
void setup() {
  
    //Start serial interface
    Serial.begin(115200);
    Serial.setTimeout(10);
    mySerial.begin(115200);
    mySerial.setTimeout(10);
    
    //Enable pins for motors
    pinMode(enableLeft, OUTPUT);
    pinMode(enableRight, OUTPUT);
    
    pinMode(leftA, OUTPUT);
    pinMode(leftB, OUTPUT);
    pinMode(rightA, OUTPUT);
    pinMode(rightB, OUTPUT);

    //Turn on motors
    digitalWrite(leftA, HIGH);
    digitalWrite(leftB, LOW);
    digitalWrite(rightA, HIGH);
    digitalWrite(rightB, LOW);

    potValue = (analogRead(potentiometer) - 512) / 2048.0;

}

void loop() {

    char input[INPUT_SIZE];
    byte size = mySerial.readBytes(input, INPUT_SIZE);
    // Add the final 0 to end the C string
    input[size] = 0;
    
    // Read each command pair 
    char* command = strtok(input, "&");
    
    // Split the command in two values
    char* separator = strchr(input, ':');
 
    if (separator != 0)
    {
        // Actually split the string in 2: replace ':' with 0
        *separator = 0;
        int speed = atoi(command);
        ++separator;
        int angle = atoi(separator);
        
        drive(speed, angle);

     }
}

//Move the two corresponding motors forward
void drive(int speed, int angle){
  
  if (angle == 0)
  {
     analogWrite(enableLeft, speed);
     analogWrite(enableRight, speed);
  }
  else if (angle > 0)
  {
     analogWrite(enableLeft, speed);
     analogWrite(enableRight, speed - speed/250 * angle* (1.4 - potValue));
  }
  else
  {
    analogWrite(enableLeft, speed + speed/250 * angle * (1.4 + potValue));
    analogWrite(enableRight, speed);
  }
 
  //analogWrite(enableRight, 255);
  //analogWrite(enableLeft, 255);
}
