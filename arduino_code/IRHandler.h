#ifndef IR_HANDLER_H
#define IR_HANDLER_H

#include "GameManager.h"

class IRHandler {
  public:
    IRHandler(GameManager& manager);
    void begin();    
    void listen();    

  private:
    GameManager& gameManager;
};

#endif
