#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <aJSON.h>
#include <Wire.h>
#include <Adafruit_TMP006.h>
#include <BMA222.h>

char ssid[] = "MY-SSID";
char password[] = "MY-PASSWORD";
char server[] = "10.0.1.64";

char pubtopic[64];
char subTopic[64];
char deviceId[64];

// system board LED 
int redLedPin          = RED_LED;
int redLedPinStatus    = LOW;
int greenLedPin        = GREEN_LED;
int greenLedPinStatus  = LOW;
int yellowLedPin       = YELLOW_LED;
int yellowLedPinStatus = LOW;

// system board buttons 
const int button2Pin   = PUSH2; 
int button2State       = 0;
const int button1Pin   = PUSH1; 
int button1State       = 0;

// temperature
Adafruit_TMP006 tmp006(0x41);  // start with a diferent i2c address!
bool tempSensorFound = FALSE;

// accelerometer
BMA222 mySensor;   //accel sensor

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received message for topic ");
  Serial.print(topic);
  Serial.print("with length ");
  Serial.println(length);
  Serial.println("Message:");
  Serial.write(payload, length);
  
  if (strstr((char*)payload, "ON") != NULL) redLedPinStatus = HIGH;
  else  if (strstr((char*)payload, "OFF") != NULL) redLedPinStatus = LOW;
  else  Serial.println("Unknow!");
  Serial.println();
}

WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);

void setup()
{
  Serial.begin(115200);
  
   // setup on board leds
  pinMode(redLedPin, OUTPUT);
  digitalWrite(redLedPin, redLedPinStatus);
  pinMode(greenLedPin, OUTPUT);
  digitalWrite(greenLedPin, greenLedPinStatus);
  pinMode(yellowLedPin, OUTPUT);
  digitalWrite(yellowLedPin, yellowLedPinStatus);
    
  // initialize the pushbutton pin as an input:
  pinMode(button1Pin, INPUT_PULLUP); 
  pinMode(button2Pin, INPUT_PULLUP); 
  
  // Start Ethernet with the build in MAC Address
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  // We are connected and have an IP address.
  // Print the WiFi status.
  printWifiStatus();
  
   // Use MAC Address as deviceId
  uint8_t macOctets[6];
  MACAddress mac;
  mac = WiFi.macAddress(macOctets);
  sprintf(deviceId, "%02x%02x%02x%02x%02x%02x", macOctets[0], macOctets[1], macOctets[2], macOctets[3], macOctets[4], macOctets[5]);
  Serial.print("deviceId: ");
  Serial.println(deviceId);
  sprintf(pubtopic, "iot/CC3200/%02x%02x%02x%02x%02x%02x/data", macOctets[0], macOctets[1], macOctets[2], macOctets[3], macOctets[4], macOctets[5]);
  sprintf(subTopic, "iot/CC3200/%02x%02x%02x%02x%02x%02x/command", macOctets[0], macOctets[1], macOctets[2], macOctets[3], macOctets[4], macOctets[5]);
  
  // you can also use tmp006.begin(TMP006_CFG_1SAMPLE) or 2SAMPLE/4SAMPLE/8SAMPLE to have
  // lower precision, higher rate sampling. default is TMP006_CFG_16SAMPLE which takes
  // 4 seconds per reading (16 samples)
  if (! tmp006.begin()) {
  //if (TRUE) {
    Serial.println("NO!! on board temp sensor found");
    tempSensorFound = FALSE;
  } else {
    Serial.println("temp sensor found, good");
    tempSensorFound = TRUE;
  }  
  
  // start the accel sensor
  mySensor.begin();
  uint8_t chipID = mySensor.chipID();
  Serial.print("hipID: ");
  Serial.println(chipID);
}

void loop()
{
  // Reconnect if the connection was lost
  if (!client.connected()) {
    Serial.println("Disconnected. Reconnecting....");

    if(!client.connect(deviceId, "guest", "guest")) {
      Serial.println("Connection failed");
    } else {
      Serial.println("Connection success");
      if(client.subscribe(subTopic)) {
        Serial.println("Subscription successfull");
      }
    }
  }
  
    // read the state of the pushbutton value:
  button1State = digitalRead(button1Pin);

  
   // read the state of the pushbutton value:
  button2State = digitalRead(button2Pin);

  digitalWrite(redLedPin, redLedPinStatus);
  digitalWrite(greenLedPin, greenLedPinStatus);
  digitalWrite(yellowLedPin, yellowLedPinStatus);
  
  
   // Send and receive QoS 0 message
  aJsonObject *json,*data;
  json = aJson.createObject();
  aJson.addItemToObject(json, "d", data = aJson.createObject());
  aJson.addItemToObject(data, "myName", aJson.createItem(deviceId));
  
  float temperature = getTemp();
  aJson.addItemToObject(data, "temp", aJson.createItem(temperature));
  
  float temperature_obj = getTempObj();
  aJson.addItemToObject(data, "tempObj", aJson.createItem(temperature_obj));
  
  int8_t sensorData = mySensor.readXData();
  aJson.addItemToObject(data, "X", aJson.createItem(sensorData));

  sensorData = mySensor.readYData();
   aJson.addItemToObject(data, "Y", aJson.createItem(sensorData));

  sensorData = mySensor.readZData();
  aJson.addItemToObject(data, "Z", aJson.createItem(sensorData));
  
  // get the network received signal strength:
  float RSSI = abs(WiFi.RSSI());
  aJson.addItemToObject(data, "RSSI", aJson.createItem(RSSI));

  int value = (button1State ? 50 :0);
  aJson.addItemToObject(data, "Button1", aJson.createItem(value));
  
  value = (button2State ? 50 :0);
  aJson.addItemToObject(data, "Button2", aJson.createItem(value));

  Serial.print("Publishing: ");
  char* string = aJson.print(json);
  if (string != NULL) {
    Serial.println(string);
  } 
  else {
    Serial.print( "error getting sting from json");
  }
  
  //publish 
  if(client.publish(pubtopic, string)) {
    Serial.println("Publish success");
  } else {
    Serial.println("Publish failed");
  }
  
  aJson.deleteItem(json);
  free(string);
 
  // Check if any message were received
  // on the topic we subsrcived to
  client.loop();
  delay(1000);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

float getTempObj() {
  if (tempSensorFound) {
    float objt = tmp006.readObjTempC();
    return objt;
  } else {
    return 0;
  }  
}


float getTemp() {
  if (tempSensorFound) {
    float objt = tmp006.readDieTempC();
    return objt;
  } else {
    return 0;
  }  
}
