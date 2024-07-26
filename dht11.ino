#include <DHT.h>

// DHT11 sensor pins
#define DHTPIN 2     // Data pin connected to digital pin 2
#define DHTTYPE DHT11   // DHT11 sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float temperatureF = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature) || isnan(temperatureF)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float heatIndexF = dht.computeHeatIndex(temperatureF, humidity);
  // Compute heat index in Celsius (isFahreheit = false)
  float heatIndexC = dht.computeHeatIndex(temperature, humidity, false);

  // Print the results
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *C ");
  Serial.print(temperatureF);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(heatIndexC);
  Serial.print(" *C ");
  Serial.print(heatIndexF);
  Serial.println(" *F");
}
