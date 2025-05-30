#include <SPI.h>
#include <LoRa.h>

String incomingByte = "";

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // Try to parse incoming packet
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    // Received a packet
    Serial.print("Received packet: ");
    incomingByte = "";  // Clear the buffer

    while (LoRa.available()) {
      char c = (char)LoRa.read();
      incomingByte += c;
    }

    Serial.println(incomingByte);

    // Optionally print signal strength
    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi());
  }
}
