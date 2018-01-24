#ifndef SIMPLE_INDICATOR
#define SIMPLE_INDICATOR

#include <Arduino.h>

class SimpleIndicator{
  private:
  int _pin;
  bool _state;

  void _setState(int newState){
    if(_state != newState){
      _state = newState;
      digitalWrite(_pin, _state);
    }
  }
  
  public:
  SimpleIndicator(int pin){
    _pin = pin;
    pinMode(_pin, OUTPUT);
  }

  void switchOn(){
    _setState(HIGH);
  }
  void switchOff(){
    _setState(LOW);
  }
};

#endif


