#ifndef SENSOR
#define SENSOR

#include <Arduino.h>

class ThresholdSensor {
  private:
    int _state;
    long _stateTimestamp;
    long _threshold;
    int _pin;
    bool _fired;

    void (*_onDrop)(int);

    void _act(){
        if(millis() >= _stateTimestamp +_threshold
            && _state == LOW
            && !_fired){
                _drop();
                _fired =true;
        }
    }

    void _drop() {
      if (_onDrop != 0L) {
        _onDrop(_pin);
      }
    }

    void _setState(int state) {
        if(state!=_state){
            _state = state;
            _stateTimestamp = millis();
            _fired = false;
        }
    }

  public:
    ThresholdSensor(int pin, long  threshold) {
      _pin = pin;
      _state = HIGH;
      _threshold = threshold;
      _stateTimestamp = 0;
      _fired = false;

      _onDrop = 0L;

      pinMode(pin, INPUT_PULLUP);
    }

    void check() {
        int state = digitalRead(_pin);
        _setState(state);
         _act();
    }

    void onDrop(void (*callback)(int)) {
      _onDrop = callback;
    }
};
#endif
