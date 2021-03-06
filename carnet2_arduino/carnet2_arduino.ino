
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
int savedAngle;
int savedSpeed;  
double potValue = 0.0;

//Setup software serial
SoftwareSerial mySerial(2, 3);
void setup() {
  
    //Start serial interface
    Serial.begin(115200);
    Serial.setTimeout(1);
    mySerial.begin(115200);
    mySerial.setTimeout(1);
    
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

  if(mySerial.available()){
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
        if(speed >= 1000){
          speed = speed / 10;
        }
        ++separator;
        int angle = atoi(separator);
        if(angle > 90 || angle < -90){
          angle = angle/10;
        }
       
        drive(speed, angle);

     }
  }
}

//Move the two corresponding motors forward
void drive(int speed, int angle){
  
  if(savedSpeed != speed || savedAngle != angle){
  
    if (angle == 0 && speed > 0)
    {
      analogWrite(enableLeft, 255);
      analogWrite(enableRight, 255);
    }
    else if (angle < 0)
    {
      analogWrite(enableLeft, 10);
      analogWrite(enableRight, speed - speed/255 * angle* (2.0 - potValue));
    }
    else
    {
      analogWrite(enableLeft, speed + speed/255 * angle * (2.0 + potValue));
      analogWrite(enableRight, 10);
    }
    savedSpeed = speed;
    savedAngle = angle;
 
  }
  
}
