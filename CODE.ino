#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define SMOKE_PIN A0
#define FLAME_PIN 3
#define RELAY_PIN 7

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(FLAME_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
}

void loop() {
  float temp = dht.readTemperature();
  int smoke = analogRead(SMOKE_PIN);
  int flame = digitalRead(FLAME_PIN);  // LOW means fire

  // Send sensor data to Python (via USB Serial)
  Serial.print(temp); Serial.print(",");
  Serial.print(smoke); Serial.print(",");
  Serial.println(flame);

  // Wait for ML response from Python
  if (Serial.available()) {
    String response = Serial.readStringUntil('\n');
    response.trim();

    if (response == "HIGH_FIRE" || response == "MEDIUM_FIRE") {
      digitalWrite(RELAY_PIN, HIGH);
      delay(5000);
      digitalWrite(RELAY_PIN, LOW);
    }
  }

  delay(2000);
}