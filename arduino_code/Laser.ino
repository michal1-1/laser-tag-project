#include <Arduino.h>
#include "Config.h"
#include "GameManager.h"
#include "IRHandler.h"
#include "Effects.h"
#include "WiFiHandler.h"

// רכיבי מערכת
Effects effects;
GameManager gameManager;
IRHandler irHandler(gameManager);
WiFiHandler wifiHandler(gameManager);

void setup() {
  Serial.begin(115200);
  delay(500);

  // נורות
  effects.begin();

  // IR שלט
  irHandler.begin();

  // WiFi (חיבור חכם עם הדפסה)
  wifiHandler.setupWiFi();

  // מצב התחלה – מחכה למשחק
  gameManager.resetGame();
}

void loop() {
  irHandler.listen();           
  gameManager.update();
  wifiHandler.handleClients(); 
  delay(10);
}
