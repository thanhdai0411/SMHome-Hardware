
void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  Serial.println();

  if (String(topic) == String(TOPIC_DEVICE_1))
  {
    int state = messageTemp == "1" ? ON : OFF;
    ledStates[0] = state;
    actionControlDevice(0, state);
  }
  if (String(topic) == String(TOPIC_DEVICE_2))
  {
    int state = messageTemp == "1" ? ON : OFF;
    ledStates[1] = state;
    actionControlDevice(1, state);
  }
  if (String(topic) == String(TOPIC_DEVICE_3))
  {
    int state = messageTemp == "1" ? ON : OFF;
    ledStates[2] = state;
    actionControlDevice(2, state);
  }
  if (String(topic) == String(TOPIC_DEVICE_4))
  {
    int state = messageTemp == "1" ? ON : OFF;
    ledStates[3] = state;
    actionControlDevice(3, state);
  }
  if (String(topic) == String(TOPIC_DEVICE_5))
  {
    int state = messageTemp == "1" ? ON : OFF;
    ledStates[4] = state;
    actionControlDevice(4, state);
  }
}

void reconnect()
{

  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client", MQTT_USERNAME, MQTT_PWD))
    {
      Serial.println("connected");
      // Subscribe
      client.subscribe(TOPIC_DEVICE_1);
      client.subscribe(TOPIC_DEVICE_2);
      client.subscribe(TOPIC_DEVICE_3);
      client.subscribe(TOPIC_DEVICE_4);
      client.subscribe(TOPIC_DEVICE_5);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
