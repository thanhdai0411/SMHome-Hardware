
void actionControlDevice(int ledIndex, int state)
{
  //  ledStates[ledIndex] = !ledStates[ledIndex];
  digitalWrite(ledPins[ledIndex], state);
}

void toggleButton(int i)
{
  int reading = digitalRead(buttonPins[i]);

  if (reading != lastButtonStates[i])
  {
    lastDebounceTimes[i] = millis();
  }

  if ((millis() - lastDebounceTimes[i]) > debounceDelay)
  {
    if (reading != buttonStates[i])
    {
      buttonStates[i] = reading;

      if (buttonStates[i] == RELEASE)
      {

        ledStates[i] = !ledStates[i];
        int state = ledStates[i] == ON ? 1 : 0 ;

        client.publish(buildTopicPubDevice(i), buildFrameTrans(buildDeviceIdForDevice(i), (String)state));
        actionControlDevice(i, ledStates[i]);
      }
    }
  }

  lastButtonStates[i] = reading;
}

void readDataSensorSHT3x() {
  if (sht3x.measure())
  {
    temperature = sht3x.temperature();
    humidity = sht3x.humidity();

    Serial.println();
    Serial.print("Temperature: ");
    Serial.print(temperature, 1);

    Serial.print(" *C\tHumidity: ");
    Serial.print(humidity, 1);
    Serial.print(" %RH");
    Serial.println();

    client.publish(TOPIC_SENSOR_1, buildFrameTrans(SMH_SENSOR1_ID, (String)temperature));
    client.publish(TOPIC_SENSOR_2, buildFrameTrans(SMH_SENSOR2_ID, (String)humidity));
  }
  else
  {
    Serial.println("SHT3x read error");
  }
}

void readDigitalMQ135() {
  // MQ135
  int MQ135_data = digitalRead(SENSOR_DIGITAL_PIN_4);
  if (MQ135_data != previous_MQ135_data) { // Chỉ publish khi có sự thay đổi
    Serial.println();
    Serial.print("MQ135: ");
    Serial.println(MQ135_data);
    client.publish(TOPIC_SENSOR_4, buildFrameTrans( SMH_SENSOR4_ID, (String)MQ135_data));
    previous_MQ135_data = MQ135_data; // Cập nhật trạng thái trước đó
  }
}


void readDigitalMP2() {
  // MP2
  int MP2_data = digitalRead(SENSOR_DIGITAL_PIN_5);
  if (MP2_data != previous_MP2_data) { // Chỉ publish khi có sự thay đổi
    Serial.println();
    Serial.print("MP2: ");
    Serial.println(MP2_data);
    client.publish(TOPIC_SENSOR_5, buildFrameTrans( SMH_SENSOR5_ID, (String)MP2_data));
    previous_MP2_data = MP2_data; // Cập nhật trạng thái trước đó
  }
}


void readDigitalSR501() {
  // SR501
  int SR501_data = digitalRead(SENSOR_DIGITAL_PIN_2);
  if (SR501_data != previous_SR501_data) { // Chỉ publish khi có sự thay đổi
    Serial.println();
    Serial.print("SR501: ");
    Serial.println(SR501_data);
    client.publish(TOPIC_SENSOR_3, buildFrameTrans(SMH_SENSOR3_ID, (String)SR501_data));
    previous_SR501_data = SR501_data; // Cập nhật trạng thái trước đó
  }
}
