#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10);
const byte address[] = "00001"; // Alamat pipe untuk komunikasi

void setup() {
  Serial.begin(115200);

  radio.begin();
  radio.openWritingPipe(address);
  radio.openReadingPipe(1, address); // Pipe 1 untuk menerima balasan
  radio.setPALevel(RF24_PA_MIN); // Set power amplifier level ke minimum
}

void loop() {
  unsigned long startTime = millis();
  unsigned long timeTaken;

  // Mengirimkan pesan ping
  radio.stopListening();
  if (radio.write(&startTime, sizeof(startTime))) {
    radio.startListening();

    // Menunggu balasan pong
    unsigned long startWaiting = millis();
    bool timeout = false;
    while (!radio.available()) {
      if (millis() - startWaiting > 200) { // timeout 200 ms
        timeout = true;
        break;
      }
    }

    if (!timeout) {
      unsigned long receivedTime;
      radio.read(&receivedTime, sizeof(receivedTime));
      timeTaken = millis() - startTime;
      Serial.print("Ping: ");
      Serial.print(timeTaken);
      Serial.println(" ms");
    } else {
      Serial.println("Ping: Timeout");
    }
  } else {
    Serial.println("Ping: Failed to send");
  }

  delay(1000);
}
