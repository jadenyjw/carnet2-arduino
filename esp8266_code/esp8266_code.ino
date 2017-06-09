#include <ESP8266WiFi.h>//package for our specific wifi module
#include <WiFiUdp.h>//package for wifi transmission
const char* ssid = "carnet-wifi";//id for wireless server
const char* password = "11111111";//pass for wireless server

WiFiUDP Udp;//instance of wireless UDP class that can requires 
unsigned int localUdpPort = 42069;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets

void setup()
{
  //starts serial cycles
  Serial.begin(115200);
  Serial.println();
  //connects to the wifi server setup
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");//shows that there is a connection in serial monitor
  }
  Udp.begin(localUdpPort);//begins the UDP for our wireless module to transfer information
}

void loop()
{
  int packetSize = Udp.parsePacket();//reads packet
  if (packetSize)//checks if such packet sizes exist
  {
    int len = Udp.read(incomingPacket, 255);//length of packet stored
    if (len > 0)//if packet is not empty, runs
    {
      incomingPacket[len] = 0;//resets said variable 
    }
    Serial.write(incomingPacket);//Sends contents through serial out to arduino
  }
}
