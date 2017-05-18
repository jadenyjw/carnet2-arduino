#include <SoftwareSerial.h>
#define INPUT_SIZE 8

//Declare variables for L293D 
int enableLeft = 10;
int enableRight = 11;
int leftA = 8;
int leftB = 9;
int rightA = 12;
int rightB = 13;

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

}

void loop() {
 //Forever listen to serial from the ESP8266 for actions
 if (mySerial.available()) {
    char input[INPUT_SIZE + 1];
    byte size = Serial.readBytes(input, INPUT_SIZE);
    // Add the final 0 to end the C string
    input[size] = 0;
    char* command = strtok(input, "&");
    char* separator = strchr(command, ':');
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
}

//Drive forward

void drive(int speed, int angle){
  analogWrite(enableLeft, (int) (speed - angle*1.41));
  analogWrite(enableRight, (int) (speed + angle*1.41));
}
