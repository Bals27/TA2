// Definisikan pin analog yang terhubung ke AOUT dari MQ-6
#define MQ6_PIN A0

void setup() {
  // Memulai komunikasi serial
  Serial.begin(9600);
  Serial.println("MQ-6 Gas Sensor Test");
}

void loop() {
  // Membaca nilai analog dari MQ-6
  int sensorValue = analogRead(MQ6_PIN);

  // Konversi nilai analog ke tegangan (jika diperlukan)
  float voltage = sensorValue * (5.0 / 1023.0);

  // Cetak hasil pembacaan ke Serial Monitor
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print("\t Voltage: ");
  Serial.println(voltage);

  // Tunggu beberapa saat sebelum membaca nilai berikutnya
  delay(1000);
}
