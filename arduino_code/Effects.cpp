#include "Effects.h"
#include "Config.h"
#include <Arduino.h>

void Effects::begin() {
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, LED_OFF);
  digitalWrite(GREEN_LED_PIN, LED_OFF);
}

void Effects::showHit() {
  flashLED(RED_LED_PIN, 2, 200);
}

void Effects::showGameOver() {
  flashLED(RED_LED_PIN, 3, 300);
  flashLED(GREEN_LED_PIN, 3, 300);
}

void Effects::setWaitingMode() {
  turnOff(RED_LED_PIN);
  turnOn(GREEN_LED_PIN);
}

void Effects::setPlayingMode() {
  turnOff(RED_LED_PIN);
  turnOff(GREEN_LED_PIN);
}

void Effects::flashLED(int pin, int times, int duration) {
  for (int i = 0; i < times; i++) {
    turnOn(pin);
    delay(duration);
    turnOff(pin);
    delay(duration);
  }
}

void Effects::turnOn(int pin) {
  digitalWrite(pin, LED_ON);
}

void Effects::turnOff(int pin) {
  digitalWrite(pin, LED_OFF);
}
