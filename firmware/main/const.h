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

#define SMH_START_ID "99"

// define node
#define SMH_ROOT "SM_HOME"

// ID of node
#define SMH_NODE_ID "SMH_NODE2"

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

// VD topic relay : "/SM_HOME/SMH_NODE1/SMH_DEVICE1/status"
// VD topic sensor : "/SM_HOME/SMH_NODE1/SMH_SENSOR1/status"

const char *TOPIC_START_NODE = "/" SMH_ROOT "/" SMH_NODE_ID "/" "start";

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
