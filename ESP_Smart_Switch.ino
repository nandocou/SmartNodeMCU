#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>


//const char* ssid = "SuperWiFi 2.4GHz";
//const char* pass = "villegas";

//const char* ssid = "MotoG";
//const char* pass = "nandoman";

int ledPin = 14; // GPIO14

WiFiClient WiFiclient;
MQTTClient client;
WiFiManager wifiManager;

unsigned long lastMillis = 0;

int val = 1;

void setup() {
  Serial.begin(9600);
  //  WiFi.begin(ssid, pass);

  //wifiManager.setCustomHeadElement("<script>if (window.location.href.indexOf(/wifi)<0) {window.location=/wifi};</script>");

  wifiManager.autoConnect("AutoConnectAP");

  WiFi.mode(WIFI_STA);

  //  Enable light sleep
  wifi_set_sleep_type(LIGHT_SLEEP_T);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  client.begin("m16.cloudmqtt.com", 12807, WiFiclient);
  client.onMessage(messageReceived);

  //void setOptions(int keepAlive, bool cleanSession, int timeout);
  //default setOptions(10,true, 1000);
  client.setOptions(10, true, 1000);


  connect();
}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    //   bool publish(const String &topic, const String &payload, bool retained, int qos)
    client.publish("data", "world", false, 0);
  }

  delay(500);
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }



  Serial.print("\nconnecting...");
  while (!client.connect("nodemcu", "qnjmmect", "skrZqVQ6-DTk")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("switch");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);

  if (payload.toInt() > 0)  {
    digitalWrite(ledPin, HIGH);

  }
  else if (payload.toInt() < 1)  {
    digitalWrite(ledPin, LOW);

  }

}
