#include <DHT.h>

#define DHTPIN A3
#define DHTTYPE DHT11

int temp_val = 0, water_lvl = 0, temp_threshold = 30;

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(A3, INPUT);
  pinMode(3, INPUT);
  Serial.begin(9600);

  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
}

void loop()
{
  temp_val = dht.readTemperature();;
  water_lvl = digitalRead(3);
  Serial.print("temp: ");
  Serial.println(temp_val);
  Serial.print("water lvl: ");
  Serial.println(water_lvl);
  if (water_lvl) {
    digitalWrite(7, HIGH);
  } else {
    digitalWrite(7, LOW);
    if (temp_val < temp_threshold) {
      digitalWrite(8, HIGH);
      Serial.println("Coil ON");
    } else {
      digitalWrite(8, LOW);
      Serial.println("coil OFF");
    }
  }
  delay(2000); // Wait for 2000 millisecond(s)
}
