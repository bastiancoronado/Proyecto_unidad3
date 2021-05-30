#include "DS1307.h"
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "MAURICIO";
const char* password = "43667111";
const char* host = "192.168.0.5";
WiFiUDP udpDevice;
uint16_t localUdpPort = 70171;
uint16_t UDPPort = 5052;
#define ALIVE 1000
#define D0 4

DS1307 clk;

void setup() {
  Serial.begin(115200);
  clk.setAddress();
  clk.setAllDate(50, 59, 11, 1, 2, 25, 5, 21);

  pinMode(D0, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(D0, HIGH);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());
  udpDevice.begin(localUdpPort);


}



void loop() {
  task();
  aliveTask();
}

void task() {
  byte bufferTx[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  static uint32_t previousMillis = 0;
  static uint32_t currentMillis;
  currentMillis  = millis();




  if ((currentMillis - previousMillis) >= ALIVE) {

    if (clk.onBus()) {
      clk.getAllDate(bufferTx);


    }
    else {
      for (uint8_t i = 0; i < sizeof(bufferTx); i++) bufferTx[i] = 0;
    }
    //Serial.write(bufferTx, sizeof(bufferTx));
    previousMillis = currentMillis;

    udpDevice.beginPacket(host, UDPPort);
    udpDevice.write(bufferTx, sizeof(bufferTx));
    udpDevice.endPacket();

  }

}

void aliveTask() {
  static uint32_t previousMillis = 0;
  static uint32_t currentMillis;
  static uint8_t ledState = 0;
  currentMillis  = millis();
  if ((currentMillis - previousMillis) >= ALIVE) {
    previousMillis = currentMillis;
    if (ledState == 0) {
      digitalWrite(D0, HIGH);
      ledState = 1;
    }
    else {
      digitalWrite(D0, LOW);
      ledState = 0;
    }
  }
}
