#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <Arduino.h>

enum GameState {
  WAITING,
  COUNTDOWN,
  PLAYING,
  GAME_OVER
};

class GameManager {
  public:
    GameManager();
    
    void startCountdown();     
    void startGame();           
    void resetGame();           
    void handleHit();           
    void update();              
    bool isGameActive() const;  
    int getLives() const;       
    GameState getState() const; 

  private:
    int lives;
    unsigned long gameStartTime;
    unsigned long lastHitTime;
    GameState state;
};

#endif
