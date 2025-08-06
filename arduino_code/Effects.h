#ifndef EFFECTS_H
#define EFFECTS_H

class Effects {
  public:
    void begin();
    void showHit();
    void showGameOver();
    void flashLED(int pin, int times, int duration);

    void setWaitingMode();  
    void setPlayingMode();  

  private:
    void turnOn(int pin);
    void turnOff(int pin);
};

#endif