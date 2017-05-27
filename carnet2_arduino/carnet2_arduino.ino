
#include <SoftwareSerial.h>
#define INPUT_SIZE 8

//Declare variables for L293D 
int enableLeft = 10;
int enableRight = 11;
int leftA = 8;
int leftB = 9;
int rightA = 12;
int rightB = 13;

int potentiometer = A2;

int trigPin = 4;
int echoPin = 5;    

double potValue = 0.0;

double initialDistance;
double duration, cm;

//Setup software serial
SoftwareSerial mySerial(2, 3);
void setup() {
  
    //Start serial interface
    Serial.begin(115200);
    Serial.setTimeout(5);
    mySerial.begin(115200);
    mySerial.setTimeout(1);
    
    //Enable pins for motors
    pinMode(enableLeft, OUTPUT);
    pinMode(enableRight, OUTPUT);
    
    pinMode(leftA, OUTPUT);
    pinMode(leftB, OUTPUT);
    pinMode(rightA, OUTPUT);
    pinMode(rightB, OUTPUT);

    //Ultrasonic Sensor Setup
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    //Turn on motors
    digitalWrite(leftA, HIGH);
    digitalWrite(leftB, LOW);
    digitalWrite(rightA, HIGH);
    digitalWrite(rightB, LOW);

    //Calibrates an initial distance from front to itself
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    initialDistance = (duration/2) / 29.1;

    potValue = (analogRead(potentiometer) - 512) / 2048.0;

}

void loop() {

 
  //digitalWrite(trigPin, LOW);
  //delayMicroseconds(1);
  //digitalWrite(trigPin, HIGH);
  //delayMicroseconds(10);
  //digitalWrite(trigPin, LOW);
  
  //duration = pulseIn(echoPin, HIGH);
  //cm = (duration/2) / 29.1;

  
  //Forever listen to serial from the ESP8266 for actions
  boolean noWallorStairs = (initialDistance - cm <= 5 && initialDistance - cm >= -10);
 
  
  if (mySerial.available()) {
    char input[INPUT_SIZE + 1];
    byte size = mySerial.readBytes(input, INPUT_SIZE);
    // Add the final 0 to end the C string
    input[size] = 0;
    char* command = strtok(input, "&");
    char* separator = strchr(command, ':');
    if (separator != 0)
    {
        // Actually split the string in 2: replace ':' with 0
        *separator = '0';
        int speed = atoi(command);
        Serial.println(speed);
        ++separator;
        int angle = atoi(separator);
        Serial.println(angle);
        drive(speed, angle);
    } 
  }
  
}

//Move the two corresponding motors forward
void drive(int speed, int angle){
  //Serial.println(speed - angle* (1.16 - potValue));
  //Serial.println(speed + angle* (1.16 + potValue));
  analogWrite(enableLeft, speed + speed/128 * (angle * (1.16 + potValue)));
  analogWrite(enableRight, speed - speed/128 * (angle* (1.16 - potValue)));
}
