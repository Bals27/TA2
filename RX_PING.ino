#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10);
const byte address[] = "00001"; // Alamat pipe untuk komunikasi

void setup() {
  Serial.begin(115200);

  radio.begin();
  radio.openReadingPipe(1, address);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening(); // Mulai mendengarkan untuk menerima pesan ping
}

void loop() {
  if (radio.available()) {
    unsigned long receivedTime;
    radio.read(&receivedTime, sizeof(receivedTime));

    // Mengirim balasan pong
    radio.stopListening();
    radio.write(&receivedTime, sizeof(receivedTime));
    radio.startListening();

    Serial.println("Ping request received and responded");
  }
}
