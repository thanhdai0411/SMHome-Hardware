#include <Arduino.h>
#include <Wire.h>
#include <ArtronShop_SHT3x.h>
#include <PubSubClient.h>
#include <WiFi.h>

// define mqtt wifi
#define MQTT_USERNAME "sm-home"
#define MQTT_PWD "123123"
#define WIFI_SSID "Dai Nam 2.4G"
#define WIFI_PWD "68686868"
#define MQTT_BROKER_URL "192.168.1.133"

// define pin + relay
#define RELAY_PIN_1 23
#define RELAY_PIN_2 19
#define RELAY_PIN_3 18
#define RELAY_PIN_4 5
#define RELAY_PIN_5 17

#define BTN_PIN_1 16
#define BTN_PIN_2 4
#define BTN_PIN_3 0
#define BTN_PIN_4 2
#define BTN_PIN_5 15


#define SENSOR_ANALOG_PIN_2 34
#define SENSOR_DIGITAL_PIN_2 25

#define SENSOR_ANALOG_PIN_3 35
#define SENSOR_DIGITAL_PIN_3 26

#define SENSOR_ANALOG_PIN_4 32
#define SENSOR_DIGITAL_PIN_4 27

#define SENSOR_ANALOG_PIN_5 33
#define SENSOR_DIGITAL_PIN_5 14


const int sensorAnalogPins[4] = {SENSOR_ANALOG_PIN_2, SENSOR_ANALOG_PIN_3, SENSOR_ANALOG_PIN_4, SENSOR_ANALOG_PIN_5};
const int sensorDigitalPins[4] = {SENSOR_DIGITAL_PIN_2, SENSOR_DIGITAL_PIN_3, SENSOR_DIGITAL_PIN_4, SENSOR_DIGITAL_PIN_5};


#define ON LOW
#define OFF HIGH

#define PRESS LOW
#define RELEASE HIGH

// frame define
#define START_FRAME "<START"
#define END_FRAME "END>"

#define SMH_SENSOR1_ID "01"
#define SMH_SENSOR2_ID "02"
#define SMH_SENSOR3_ID "03"
#define SMH_SENSOR4_ID "04"
#define SMH_SENSOR5_ID "05"

#define SMH_DEVICE1_ID "06"
#define SMH_DEVICE2_ID "07"
#define SMH_DEVICE3_ID "08"
#define SMH_DEVICE4_ID "09"
#define SMH_DEVICE5_ID "10"

// define node
#define SMH_ROOT "SM_HOME"

#define SMH_NODE_ID "SMH_NODE1"

#define SHM_DEVICE_1 "SMH_DEVICE1"
#define SHM_DEVICE_2 "SMH_DEVICE2"
#define SHM_DEVICE_3 "SMH_DEVICE3"
#define SHM_DEVICE_4 "SMH_DEVICE4"
#define SHM_DEVICE_5 "SMH_DEVICE5"

#define SHM_SENSOR_1 "SMH_SENSOR1"
#define SHM_SENSOR_2 "SMH_SENSOR2"
#define SHM_SENSOR_3 "SMH_SENSOR3"
#define SHM_SENSOR_4 "SMH_SENSOR4"
#define SHM_SENSOR_5 "SMH_SENSOR5"

const char *TOPIC_DEVICE_1 = "/" SMH_ROOT "/" SMH_NODE_ID "/" SHM_DEVICE_1 "/" "status";
const char *TOPIC_DEVICE_2 = "/" SMH_ROOT "/" SMH_NODE_ID "/" SHM_DEVICE_2 "/" "status";
const char *TOPIC_DEVICE_3 = "/" SMH_ROOT "/" SMH_NODE_ID "/" SHM_DEVICE_3 "/" "status";
const char *TOPIC_DEVICE_4 = "/" SMH_ROOT "/" SMH_NODE_ID "/" SHM_DEVICE_4 "/" "status";
const char *TOPIC_DEVICE_5 = "/" SMH_ROOT "/" SMH_NODE_ID "/" SHM_DEVICE_5 "/" "status";

const char *TOPIC_SENSOR_1 = "/" SMH_ROOT "/" SMH_NODE_ID "/" SHM_SENSOR_1 "/" "value";
const char *TOPIC_SENSOR_2 = "/" SMH_ROOT "/" SMH_NODE_ID "/" SHM_SENSOR_2 "/" "value";
const char *TOPIC_SENSOR_3 = "/" SMH_ROOT "/" SMH_NODE_ID "/" SHM_SENSOR_3 "/" "value";
const char *TOPIC_SENSOR_4 = "/" SMH_ROOT "/" SMH_NODE_ID "/" SHM_SENSOR_4 "/" "value";
const char *TOPIC_SENSOR_5 = "/" SMH_ROOT "/" SMH_NODE_ID "/" SHM_SENSOR_5 "/" "value";

const int buttonPins[5] = {BTN_PIN_1, BTN_PIN_2, BTN_PIN_3, BTN_PIN_4, BTN_PIN_5};
const int ledPins[5] = {RELAY_PIN_1, RELAY_PIN_2, RELAY_PIN_3, RELAY_PIN_4, RELAY_PIN_5};

int ledStates[5] = {OFF, OFF, OFF, OFF, OFF};
int buttonStates[5];
int lastButtonStates[5] = {RELEASE, RELEASE, RELEASE, RELEASE, RELEASE};

unsigned long lastDebounceTimes[5] = {0, 0, 0, 0, 0};
unsigned long debounceDelay = 50;

// config wifi
const char *ssid = WIFI_SSID;
const char *password = WIFI_PWD;

// mqtt config
const char *mqtt_server = MQTT_BROKER_URL;

// variable init
long digitalDelay = 0;
int previous_MQ135_data = 0;
int previous_SR501_data = 0;
long lastMsg = 0;
float temperature = 0;
float humidity = 0;

// init object
WiFiClient espClient;
PubSubClient client(espClient);
ArtronShop_SHT3x sht3x(0x44, &Wire); // ADDR: 0 => 0x44, ADDR: 1 => 0x45

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

String calculateCRC16(String data) {
  uint16_t crc = 0xFFFF;
  for (int i = 0; i < data.length(); i++) {
    crc ^= (uint8_t)data[i];
    for (int j = 0; j < 8; j++) {
      if (crc & 1) {
        crc = (crc >> 1) ^ 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  char crcStr[5];
  sprintf(crcStr, "%04X", crc); // Chuyển CRC thành chuỗi hex
  return String(crcStr);
}

char* buildTopicPubDevice(int index) {
  // Cấp phát bộ nhớ động để chứa chuỗi
  char* topic = (char*)malloc(100 * sizeof(char));
  if (topic == NULL) {
    return NULL; // Kiểm tra lỗi cấp phát bộ nhớ
  }

  // Tạo chuỗi topic
  snprintf(topic, 100, "/%s/%s/SMH_DEVICE%d/button", SMH_ROOT, SMH_NODE_ID, index + 1);
  return topic;
}

char* buildDeviceIdForDevice(int index) {

  switch (index)
  {
    case 0:
      return SMH_DEVICE1_ID;
    case 1:
      return SMH_DEVICE2_ID;
    case 2:
      return SMH_DEVICE3_ID;
    case 3:
      return SMH_DEVICE4_ID;
    case 4:
      return SMH_DEVICE5_ID;
    default:
      break;
  }
  return SMH_DEVICE1_ID;


}

const char* buildFrameTrans(String deviceID, String dataSent ) {

  String frameData = deviceID + "," + dataSent;
  String crc = calculateCRC16(frameData);
  String frame = (String)START_FRAME +  "," + frameData + "," + crc + "," + (String)END_FRAME;
  return frame.c_str();
}

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

void setup()
{
  Serial.begin(115200);

  Wire.begin();
  while (!sht3x.begin())
  {
    Serial.println("SHT3x not found !");
    delay(1000);
  }

  setup_wifi();
  setup_relay_button();
  setup_sensor() ;

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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
