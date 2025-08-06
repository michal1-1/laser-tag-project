#ifndef CONFIG_H
#define CONFIG_H

#define PLAYER_ID 2

#if PLAYER_ID == 1
  #define MY_IR_CODE       0x45
  #define OPPONENT_IR_CODE 0x46
#else
  #define MY_IR_CODE       0x46
  #define OPPONENT_IR_CODE 0x45
#endif

#define START_GAME_CODE    0x44
#define RESET_GAME_CODE    0x43
#define RESTART_GAME_CODE  0x42  

#define IR_RECEIVE_PIN   14
#define RED_LED_PIN      27
#define GREEN_LED_PIN    26

#define LED_ON  HIGH
#define LED_OFF LOW

#define MAX_LIVES            3
#define GAME_DURATION_MS     120000
#define HIT_COOLDOWN_MS      3000
#define COUNTDOWN_SECONDS    3

#endif
