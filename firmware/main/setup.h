
void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup_relay_button()
{
  for (int i = 0; i < 5; i++)
  {
    pinMode(buttonPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], OFF); // Đặt trạng thái ban đầu cho các đèn LED
  }
}

void setup_sensor() {
  for (int i = 0; i < 4; i++)
  {
    pinMode(sensorAnalogPins[i], INPUT);
    pinMode(sensorDigitalPins[i], INPUT);
  }

}
