#include "GameManager.h"
#include "Config.h"
#include "Effects.h"

extern Effects effects;

GameManager::GameManager() {
  resetGame();
}

void GameManager::resetGame() {
  lives = MAX_LIVES;
  gameStartTime = 0;
  lastHitTime = 0;
  state = WAITING;
  Serial.println("🔄 Game reset. Waiting to start.");
  effects.setWaitingMode();
}

void GameManager::startCountdown() {
  if (state != WAITING && state != GAME_OVER) return;

  state = COUNTDOWN;
  Serial.println("⏳ Countdown started...");

  for (int i = COUNTDOWN_SECONDS; i > 0; i--) {
    Serial.printf("🔢 %d...\n", i);
    effects.flashLED(GREEN_LED_PIN, 1, 500);
    delay(1000);
  }

  startGame();
}

void GameManager::startGame() {
  lives = MAX_LIVES;
  gameStartTime = millis();
  lastHitTime = 0;
  state = PLAYING;
  Serial.println("🎮 Game started!");
  effects.setPlayingMode();
}

void GameManager::handleHit() {
  if (state != PLAYING) return;

  unsigned long now = millis();
  if (now - lastHitTime < HIT_COOLDOWN_MS) {
    Serial.println("⛔ Cooldown – immune");
    return;
  }

  lastHitTime = now;
  if (lives > 0) {
    lives--;
    Serial.printf("💥 Hit! Lives left: %d\n", lives);
    effects.showHit();
  }

  if (lives == 0) {
    Serial.println("💀 Out of lives. Game Over!");
    state = GAME_OVER;
    effects.showGameOver();
  }
}

void GameManager::update() {
  if (state == PLAYING && (millis() - gameStartTime > GAME_DURATION_MS)) {
    Serial.println("⏱️ Time up! Game Over.");
    state = GAME_OVER;
    effects.showGameOver();
  }
}


bool GameManager::isGameActive() const {
  return state == PLAYING;
}

int GameManager::getLives() const {
  return lives;
}

GameState GameManager::getState() const {
  return state;
}
