
#include "WiFiHandler.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "Config.h"

const char* ssid = "chaim1_2.4GHz";
const char* password = "0504188883";

IPAddress local_IP;
IPAddress gateway(192, 168, 76, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

GameManager* gm = nullptr;

// פונקציה להוספת CORS headers
void setCorsHeaders() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

void WiFiHandler::sendGameStatus() {
  if (!gm) return;

  setCorsHeaders(); // הוספת CORS headers

  DynamicJsonDocument doc(256);
  doc["player_id"] = PLAYER_ID;

  switch (gm->getState()) {
    case WAITING:    doc["state"] = "WAITING";    break;
    case COUNTDOWN:  doc["state"] = "COUNTDOWN";  break;
    case PLAYING:    doc["state"] = "PLAYING";    break;
    case GAME_OVER:  doc["state"] = "GAME_OVER";  break;
  }

  doc["lives"] = gm->getLives();

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleStart() {
  if (!gm) return;
  
  setCorsHeaders(); // הוספת CORS headers
  
  GameState state = gm->getState();

  if (state == WAITING || state == GAME_OVER) {
    gm->startCountdown();
    server.send(200, "text/plain", "Game started");
  } else {
    server.send(409, "text/plain", "Game already running");
  }
}

void handleReset() {
  if (!gm) return;
  
  setCorsHeaders(); // הוספת CORS headers
  
  gm->resetGame();
  server.send(200, "text/plain", "Game reset");
}

// פונקציה לטיפול ב-OPTIONS requests (preflight)
void handleOptions() {
  setCorsHeaders();
  server.send(200, "text/plain", "");
}

WiFiHandler::WiFiHandler(GameManager& gameManager) {
  gm = &gameManager;
}

void WiFiHandler::setupWiFi() {
  // כתובת IP לפי שחקן
  if (PLAYER_ID == 1) {
    local_IP = IPAddress(192, 168, 76, 198);
  } else {
    local_IP = IPAddress(192, 168, 76, 199);
  }

  Serial.print(" Connecting to WiFi: ");
  Serial.println(ssid);

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println(" Failed to configure static IP");
  }

  WiFi.begin(ssid, password);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi connected!");
    Serial.print("🌐 IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ Failed to connect to WiFi");
    return;
  }

  // הגדרת routes עם CORS
  server.on("/status", HTTP_GET, sendGameStatus);
  server.on("/start",  HTTP_GET, handleStart);
  server.on("/reset",  HTTP_GET, handleReset);
  
  // הוספת support ל-OPTIONS requests (preflight)
  server.on("/status", HTTP_OPTIONS, handleOptions);
  server.on("/start",  HTTP_OPTIONS, handleOptions);
  server.on("/reset",  HTTP_OPTIONS, handleOptions);

  server.begin();
  Serial.println("🚀 HTTP server started on port 80");
}

void WiFiHandler::handleClients() {
  server.handleClient();
}