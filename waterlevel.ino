#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <cstdlib>
#include <stdlib.h>
WiFiClient espClient;
PubSubClient client(espClient);

#define GPIO5 5   //D1 - LVL5
#define GPIO4 4   //D2 - LVL4
#define GPIO14 14 //D5 - LVL3
#define GPIO12 12 //D6 - LVL2
#define GPIO13 13 //D7 - LVL1

int lvl5=1;
int lvl4=1;
int lvl3=1;
int lvl2=1;
int lvl1=1;

int sensor = 0;

const char* HOTSPOT_WIFI = "zaholy";
const char* HOTSPOT_PWD = "pelusatony";  
const char* MQTT_SERVER = "test.mosquitto.org";
#define MQTT_PORT 1883
#define TOPIC "domotica/nivel-agua"
#define BUFFER_SIZE (50)

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectado a ");
  Serial.println(HOTSPOT_WIFI);
  WiFi.mode(WIFI_STA);
  WiFi.begin(HOTSPOT_WIFI, HOTSPOT_PWD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Llegó el mensaje del tema [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char) payload[i];  // convert *byte to string
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.print(message);
  if(message=="get")
  {
    String data = (String) sensor;
    client.publish(TOPIC, data.c_str());
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando la conexión MQTT...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado");
      client.subscribe(TOPIC);
    } else {
      Serial.print("fallido, rc =");
      Serial.print(client.state());
      Serial.println(" inténtalo de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
 
  pinMode(GPIO5, INPUT_PULLUP);
  pinMode(GPIO4, INPUT_PULLUP);
  pinMode(GPIO14, INPUT_PULLUP);
  pinMode(GPIO12, INPUT_PULLUP);
  pinMode(GPIO13, INPUT_PULLUP);
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  
}
 
void loop() {

   if (!client.connected()) {
    reconnect();
  }

  //Lectura de la sonda tiene en total 5 niveles
  lvl5 = digitalRead(GPIO5);
  lvl4  = digitalRead(GPIO4);
  lvl3  = digitalRead(GPIO14);
  lvl2  = digitalRead(GPIO12);
  lvl1 = digitalRead(GPIO13);

  Serial.println("Nivel 1: " + String(lvl1));
  Serial.println("Nivel 2: " + String(lvl2));
  Serial.println("Nivel 3: " + String(lvl3));
  Serial.println("Nivel 4: " + String(lvl4));
  Serial.println("Nivel 5: " + String(lvl5));
  
  //Lectura 
  if(lvl5 == LOW && lvl4 == LOW && lvl3 == LOW && lvl2 == LOW && lvl1 == LOW){
    sensor = 100;
  }
  if(lvl5 == HIGH && lvl4 == LOW && lvl3 == LOW && lvl2 == LOW && lvl1 == LOW){
    sensor = 80;
  }
  if(lvl5 == HIGH && lvl4 == HIGH && lvl3 == LOW && lvl2 == LOW && lvl1 == LOW){
    sensor = 60;
  }
  if(lvl5 == HIGH && lvl4 == HIGH && lvl3 == HIGH && lvl2 == LOW && lvl1 == LOW){
    sensor = 40;
  }
  if(lvl5 == HIGH && lvl4 == HIGH && lvl3 == HIGH && lvl2 == HIGH && lvl1 == LOW){
    sensor = 20;
  }
  if(lvl5 == HIGH && lvl4 == HIGH && lvl3 == HIGH && lvl2 == HIGH && lvl1 == HIGH){
    sensor = 0;
  }

  Serial.println("SENSOR: " + String(sensor));
  delay(1000);

  client.loop();
  delay(1000);

}
