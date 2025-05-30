#include <SPI.h>
#include <LoRa.h>
#include <DFRobot_DHT11.h>

DFRobot_DHT11 DHT;

#define DHT11_PIN 5
#define SOIL_PH_PIN A3
#define SOIL_MOISTURE_PIN A4
#define RELAY1 3

int moist_read, ph_read, temp, humi;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  pinMode(RELAY1, OUTPUT);
}

void loop() {
  // Read from DHT11
  DHT.read(DHT11_PIN);
  temp = DHT.temperature;
  humi = DHT.humidity;

  // Read soil moisture and pH
  moist_read = analogRead(SOIL_MOISTURE_PIN);
  ph_read = analogRead(SOIL_PH_PIN);
  ph_read = map(ph_read, 1023, 0, 0, 7);  // adjust based on calibration

  // Send packet via LoRa
  LoRa.beginPacket();

  Serial.println("Humidity:");
  Serial.println(humi);
  Serial.println("Temperature:");
  Serial.println(temp);
  Serial.println("Moisture:");
  Serial.println(moist_read);
  Serial.println("pH:");
  Serial.println(ph_read);

  LoRa.print("A"); LoRa.print(temp); LoRa.print("#");
  LoRa.print("B"); LoRa.print(humi); LoRa.print("#");
  LoRa.print("C"); LoRa.print(ph_read); LoRa.print("#");
  LoRa.print("D"); LoRa.print(moist_read); LoRa.print("#");

  LoRa.endPacket();

  // Control relay based on moisture level
  if (moist_read > 500) {
    digitalWrite(RELAY1, LOW);  // turn OFF pump
  } else {
    digitalWrite(RELAY1, HIGH); // turn ON pump
  }

  delay(1000); // 1 second delay
}
