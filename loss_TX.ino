#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipeOut = 0xE8E8F0F0E1LL;

// Struktur data yang akan dikirim, ukuran tidak boleh melebihi 32 byte
struct PacketData {
  unsigned long sensorValue;
  unsigned long temp;
  unsigned long humid;
};

PacketData data;
RF24 radio(9, 10); // CE pin, CSN pin

void setup() {
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
}

void loop() {
  radio.write(&data, sizeof(PacketData)); // Radio mengirim paket ke receiver
}
