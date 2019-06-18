#include <ESP8266WiFi.h>
#include <PubSubClient.h>
void callback(char* topic, byte* payload, unsigned int payloadLength);
const char* ssid = "Redmi";
const char* password = "Kamal123";
#define ORG "ti62vq"
#define DEVICE_TYPE "nodemcu555"
#define DEVICE_ID "5555"
#define TOKEN "55555555"
String data;
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char subtopic[] = "iot-2/cmd/home/fmt/String";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
//Serial.println(clientID);

WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);
#define led D1
void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(led,OUTPUT);
  wifiConnect();
  mqttConnect();
}

void loop() {
  if (!client.loop()) {
    mqttConnect();
  }
delay(100);
}

void wifiConnect() {
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

void mqttConnect() {
  if (!client.connected())
  {
    Serial.print("Reconnecting MQTT client to ");
    Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    Serial.println();
  }
}
void initManagedDevice() {
  if (client.subscribe(subtopic)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}

void callback(char* topic, byte* payload, unsigned int payloadLength) {
  Serial.print("callback invoked for topic: "); 
  Serial.println(subtopic);

  for (int i = 0; i < payloadLength; i++) {
    //Serial.println((char)payload[i]);
    data += (char)payload[i];
  }
Serial.println(data);//durgaguitare1
if(data == "light_ON"){
  digitalWrite(led,HIGH);
  Serial.println("Light is Switched ON");
}
else if(data == "light_OFF"){
  digitalWrite(led,LOW);
  Serial.println("Light is Switched OFF");
}
data ="";
}
