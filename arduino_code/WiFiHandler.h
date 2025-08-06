// WiFiHandler.h – ממשק חכם לניהול WiFi
#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include "GameManager.h"

class WiFiHandler {
  public:
    WiFiHandler(GameManager& gameManager);
    void setupWiFi();      
    void handleClients();  

  private:
    static void sendGameStatus(); 
};

#endif
