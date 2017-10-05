#pragma once

class TimedAction {
  private:
    unsigned long lastMils;
    unsigned int delay;
    void ( *callback )( void );
  public:
    TimedAction();
    void setDelay(unsigned int);
    void setCallback(void ( * callback )( void ));
    void update();
};
