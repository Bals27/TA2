#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "DHT.h"

// Definisikan pin CE dan CSN
#define CE_PIN 9
#define CSN_PIN 10

// Buat objek radio
RF24 radio(CE_PIN, CSN_PIN);

// Alamat pipe untuk pengiriman
const byte address[6] = "00001";

// Pin sensor gas
#define MQ2_PIN A0 // Butana
#define MQ6_PIN A1 // Metana

// Pin DHT11
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Pin Kipas
#define FAN1_PIN 6
#define FAN2_PIN 5

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();

  // Set pin kipas sebagai output
  pinMode(FAN1_PIN, OUTPUT);
  pinMode(FAN2_PIN, OUTPUT);

  // Initialize DHT sensor
  dht.begin();
}

void loop() {
  int butaneValue = analogRead(MQ2_PIN);
  int methaneValue = analogRead(MQ6_PIN);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Mengirim data butana
  radio.write(&butaneValue, sizeof(butaneValue));
  Serial.print("Butane Value Sent: ");
  Serial.println(butaneValue);
  delay(5000); // Jeda 5 detik

  // Mengirim data metana
  radio.write(&methaneValue, sizeof(methaneValue));
  Serial.print("Methane Value Sent: ");
  Serial.println(methaneValue);
  delay(5000); // Jeda 5 detik

  // Mengirim data suhu
  int temperatureInt = (int)temperature;
  radio.write(&temperatureInt, sizeof(temperatureInt));
  Serial.print("Temperature Sent: ");
  Serial.println(temperatureInt);
  delay(5000); // Jeda 5 detik

  // Mengirim data kelembaban
  int humidityInt = (int)humidity;
  radio.write(&humidityInt, sizeof(humidityInt));
  Serial.print("Humidity Sent: ");
  Serial.println(humidityInt);
  delay(5000); // Jeda 5 detik

  // Kontrol kipas berdasarkan nilai gas metana
  if (butaneValue < 150) {
    digitalWrite(FAN1_PIN, HIGH); // Kipas 1 hidup
    digitalWrite(FAN2_PIN, LOW);  // Kipas 2 mati
  } else {
    digitalWrite(FAN1_PIN, LOW);  // Kipas 1 mati
    digitalWrite(FAN2_PIN, HIGH); // Kipas 2 hidup
  }
}
