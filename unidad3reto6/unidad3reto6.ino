#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "MAURICIO";
const char* password = "43667111";
const char* host = "192.168.0.5";
WiFiUDP udpDevice;
uint16_t localUdpPort = 70170;
uint16_t UDPPort = 5051;
#define BME_CS 5
#define ALIVE 1000
#define D0 4

Adafruit_BME280 bme(BME_CS);


void setup() {
  Serial.begin(115200);

  bool status;
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

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
  static uint32_t previousMillis = 0;
  static uint32_t currentMillis;
  byte bufferTx[8] = {0}; // humedad y temperatura
  float t = 0;
  float h = 0;


  t = bme.readTemperature();
  h = bme.readHumidity();

  memcpy(bufferTx, (uint8_t *)&t, 4);
  memcpy(bufferTx + 4, (uint8_t *)&h, 4);

  currentMillis  = millis();

  if ((currentMillis - previousMillis) >= ALIVE) {
    previousMillis = currentMillis;
    udpDevice.beginPacket(host, UDPPort);
    udpDevice.write(bufferTx, 8);
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
