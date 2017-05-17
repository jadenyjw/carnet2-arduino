#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
const char* ssid = "carnet-wifi";
const char* password = "11111111";

WiFiUDP Udp;
unsigned int localUdpPort = 42069;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  
  Udp.begin(localUdpPort);
}


void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {

    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.write(incomingPacket);

  }
}
