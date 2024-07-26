#define BLYNK_TEMPLATE_ID "TMPL6M3nUbE0Q"
#define BLYNK_TEMPLATE_NAME "TA 2"
#define BLYNK_AUTH_TOKEN "mRG7gbMMkTxXy54oQsehxJNDJymaYa-u"

#include <SPI.h> 
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Pin definitions for NodeMCU
#define CE_PIN D2
#define CSN_PIN D1
#define GREEN_LED_PIN D3
#define YELLOW_LED_PIN D4
#define RED_LED_PIN D8
#define BUZZER_PIN D0

// Blynk credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "DBU 4G";
char pass[] = "25101966";

// NRF24L01
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);

  // Initialize WiFi and Blynk
  WiFi.begin(ssid, pass);
  Blynk.begin(auth, ssid, pass);

  // Initialize NRF24L01
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  Blynk.run();
  
  if (radio.available()) {
    int receivedValue;
    radio.read(&receivedValue, sizeof(receivedValue));

    Serial.print("Received value: ");
    Serial.println(receivedValue);

    // Assume order of reception: MQ-2, MQ-6, Temperature, Humidity
    static int sensorIndex = 0;
    switch(sensorIndex) {
      case 0: // MQ-2
        Serial.print("Received MQ-2 value: ");
        Serial.println(receivedValue);
        Blynk.virtualWrite(V1, receivedValue); // Kirim nilai MQ-2 ke Blynk Virtual Pin V1
        sensorIndex++;
        // Control LEDs and Buzzer based on MQ-2 value
        if (receivedValue < 25) {
          digitalWrite(GREEN_LED_PIN, HIGH);
          digitalWrite(YELLOW_LED_PIN, LOW);
          digitalWrite(RED_LED_PIN, LOW);
          digitalWrite(BUZZER_PIN, LOW);
        } else if (receivedValue <= 39) {
          digitalWrite(GREEN_LED_PIN, LOW);
          digitalWrite(YELLOW_LED_PIN, HIGH);
          digitalWrite(RED_LED_PIN, LOW);
          digitalWrite(BUZZER_PIN, LOW);
        } else {
          digitalWrite(GREEN_LED_PIN, LOW);
          digitalWrite(YELLOW_LED_PIN, LOW);
          digitalWrite(RED_LED_PIN, HIGH);
          digitalWrite(BUZZER_PIN, HIGH);
        }
        break;
      case 1: // MQ-6
        Serial.print("Received MQ-6 value: ");
        Serial.println(receivedValue);
        Blynk.virtualWrite(V2, receivedValue); // Kirim nilai MQ-6 ke Blynk Virtual Pin V2
        sensorIndex++;
        break;
      case 2: // Temperature
        Serial.print("Received Temperature: ");
        Serial.println(receivedValue);
        Blynk.virtualWrite(V3, receivedValue); // Kirim nilai suhu ke Blynk Virtual Pin V3
        sensorIndex++;
        break;
      case 3: // Humidity
        Serial.print("Received Humidity: ");
        Serial.println(receivedValue);
        Blynk.virtualWrite(V4, receivedValue); // Kirim nilai kelembaban ke Blynk Virtual Pin V4
        sensorIndex = 0; // Reset for next cycle
        break;
    }

    delay(5000); // Jeda 10 detik antara pembacaan data berikutnya
  }
}
