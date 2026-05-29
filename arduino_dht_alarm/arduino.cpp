#include "arduino.h"

unsigned long buzzerStart = 0;
unsigned long ledStart = 0;

bool buzzerActive = false;
bool ledActive = false;

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(ledRed, OUTPUT);

}

void loop() {
  // HC-SR04
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  int distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Mouvement → buzzer 1.5 s
  bool motion = (distance > 0 && distance <= 10);

  if (motion && !buzzerActive) {
    buzzerActive = true;
    buzzerStart = millis();
    digitalWrite(buzzer, HIGH);
  }

  if (buzzerActive && millis() - buzzerStart >= 1500) {
    digitalWrite(buzzer, LOW);
    buzzerActive = false;
  }

  // Presence detectee via distance -> LED rouge 1.5 s
  if (motion && !ledActive) {
      ledActive = true;
      ledStart = millis();
      digitalWrite(ledRed, HIGH);
  }

  if (ledActive && millis() - ledStart >= 1500) {
      digitalWrite(ledRed, LOW);
      ledActive = false;
  }

  delay(100);
}
