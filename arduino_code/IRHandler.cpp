// #include "IRHandler.h"
// #include "Config.h"
// #include <IRremote.hpp>
// #include <Arduino.h>

// IRHandler::IRHandler(GameManager& manager) : gameManager(manager) {}

// void IRHandler::begin() {
//   IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
//   Serial.println("🛰️ IR Receiver ready");
// }

// void IRHandler::listen() {
//   if (IrReceiver.decode()) {
//     uint16_t code = IrReceiver.decodedIRData.command;
//     IrReceiver.resume();

//     if (code == START_GAME_CODE && gameManager.getState() == WAITING) {
//       Serial.println("▶️ Start command received");
//       gameManager.startCountdown();
//     }
//     else if (code == RESTART_GAME_CODE && gameManager.getState() == GAME_OVER) {
//       Serial.println("🔁 Restart after Game Over");
//       gameManager.startCountdown();
//     }
//     else if (code == RESET_GAME_CODE) {
//       Serial.println("🔁 Full Reset");
//       gameManager.resetGame();
//     }
//     else if (code == OPPONENT_IR_CODE) {
//       Serial.println("🎯 Hit detected from opponent");
//       gameManager.handleHit();
//     }
//     else {
//       Serial.printf("👀 Ignored code: 0x%02X\n", code);
//     }
//   }
// }
















#include "IRHandler.h"
#include "Config.h"
#include <IRremote.hpp>
#include <Arduino.h>

IRHandler::IRHandler(GameManager& manager) : gameManager(manager) {}

void IRHandler::begin() {
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("🛰️ IR Receiver ready");
}

void IRHandler::listen() {
  if (IrReceiver.decode()) {
    uint16_t code = IrReceiver.decodedIRData.command;
    IrReceiver.resume();

    if (code == START_GAME_CODE && gameManager.getState() == WAITING) {
      Serial.println("▶️ Start command received");
      gameManager.startCountdown();
    }
    else if (code == RESTART_GAME_CODE && gameManager.getState() == GAME_OVER) {
      Serial.println("🔁 Restart after Game Over");
      gameManager.startCountdown();
    }
    else if (code == RESET_GAME_CODE) {
      Serial.println("🔁 Full Reset");
      gameManager.resetGame();
    }
    // כאן התיקון - רק אם זה באמת הקוד של היריב ולא שלי!
    else if (code == OPPONENT_IR_CODE && gameManager.getState() == PLAYING) {
      Serial.println("🎯 Hit detected from opponent");
      gameManager.handleHit();
    }
    else if (code == MY_IR_CODE) {
      Serial.println("🚫 Ignoring my own shot");
      // לא עושים כלום
    }
    else {
      Serial.printf("👀 Ignored code: 0x%02X\n", code);
    }
  }
}