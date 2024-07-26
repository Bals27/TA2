#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Radio setup
const uint64_t pipeIn = 0xE8E8F0F0E1LL;
RF24 radio(9, 10);

// The sizeof this struct should not exceed 32 bytes
struct PacketData {
  unsigned long sensorValue;
  unsigned long temp;
  unsigned long humid;
};
PacketData data;

// Packet count and average calculations
int packetCounts[10]; // Array for packet counts
int packetCountIndex = 0; // Index for packet count array
int packetCountTotal = 0; // Total packet count
#define AVG_SECONDS 10
int avgs[AVG_SECONDS]; // Array for averages
int avgIndex = 0; // Index for average array
unsigned long avgTotal = 0; // Total for averages

// Variable declarations
unsigned long packetsRead = 0; // Packets read count
unsigned long lastScreenUpdate = 0;
unsigned long lastAvgUpdate = 0;
unsigned long lastRecvTime = 0; // Last received time
unsigned long drops = 0;

void setup() {
  Serial.begin(115200);
  
  // Set up radio module
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);
  radio.openReadingPipe(1, pipeIn);
  radio.startListening();

  memset(&data, 0, sizeof(PacketData));
  memset(packetCounts, 0, sizeof(packetCounts));
  memset(avgs, 0, sizeof(avgs));
}

void recvData() {
  while (radio.available()) {
    radio.read(&data, sizeof(PacketData)); // Radio reads data
    packetsRead++; // Increment packet read count
    lastRecvTime = millis(); // Update last received time
  }
}

void updateScreen() {
  unsigned long now = millis();
  
  // Update screen every 100ms
  if (now - lastScreenUpdate < 100)
    return;

  // Update packet count total
  packetCountTotal -= packetCounts[packetCountIndex];
  packetCounts[packetCountIndex] = packetsRead;
  packetCountTotal += packetsRead;
  packetCountIndex = (packetCountIndex + 1) % 10; // Cycle through packet count array
  packetsRead = 0; // Reset packets read

  // Print packet count per second and average to serial
  Serial.print("PPS: ");
  Serial.println(packetCountTotal);
  Serial.print("AVG: ");
  Serial.println(avgTotal / AVG_SECONDS);
  lastScreenUpdate = millis();

  // Update average every second
  if (now - lastAvgUpdate >= 1000) {
    avgTotal -= avgs[avgIndex];
    avgs[avgIndex] = packetCountTotal;
    avgTotal += packetCountTotal;
    avgIndex = (avgIndex + 1) % AVG_SECONDS;
    lastAvgUpdate = millis();
  }
}

void loop() {
  recvData();
  updateScreen();
}
