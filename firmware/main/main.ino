#include <Arduino.h>
#include <Wire.h>
#include <ArtronShop_SHT3x.h>
#include <PubSubClient.h>
#include <WiFi.h>


// init object
WiFiClient espClient;
PubSubClient client(espClient);
ArtronShop_SHT3x sht3x(0x44, &Wire); // ADDR: 0 => 0x44, ADDR: 1 => 0x45


// variable init
long digitalDelay = 0;
int previous_MQ135_data = 0;
int previous_SR501_data = 0;
long lastMsg = 0;
float temperature = 0;
float humidity = 0;

#include "const.h"
#include "setup.h"
#include "utils.h"
#include "control.h"
#include "cwifi.h"


void setup()
{
  Serial.begin(115200);

  Wire.begin();
//  while (!sht3x.begin())
//  {
//    Serial.println("SHT3x not found !");
//    delay(1000);
//  }

  setup_wifi();
  setup_sensor() ;

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  setup_relay_button();
}

void loop()
{

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  // set up button relay
  for (int i = 0; i < 5; i++)
  {
    toggleButton(i);
  }

  long now = millis();

  if (now - digitalDelay > 500) {
    digitalDelay = now;

    readDigitalMQ135();
    readDigitalSR501();

  }

  if (now - lastMsg > 5000)
  {
    lastMsg = now;
    readDataSensorSHT3x();

  }
}
