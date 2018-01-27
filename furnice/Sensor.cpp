#ifndef SENSOR
#define SENSOR

#include <Arduino.h>

class Sensor {
  private:
    int _state;
    long _stateTimestamp;
    long _debounceTime;
    int _pin;

    void (*_onDrop)(int);
    int _delay;

    void _act() {
      if (_state == LOW) {
        _drop();
      }
    }

    void _drop() {
      if (_onDrop != 0L) {
        _onDrop(_pin);
      }
    }

    void _updateState() {
      _state = digitalRead(_pin);
      _stateTimestamp = millis();
    }

    bool _stateChanged() {
      return digitalRead(_pin) != _state;
    }
    bool _debounced() {
      return millis() > _stateTimestamp + _debounceTime;
    }

  public:
    Sensor(int pin, long debounceTime) {
      _pin = pin;
      _state = HIGH;
      _stateTimestamp = 0;
      _debounceTime = debounceTime;

      _onDrop = 0L;

      pinMode(pin, INPUT_PULLUP);
    }

    void check() {
      if (_stateChanged()
          && _debounced()) {
        _updateState();
      }
      if(_stateTimestamp >= _delay)
        _act();
    }

    void onDropDelay(int delay, void (*callback)(int)) {
      _onDrop = callback;
      _delay = delay;
    }
};
#endif

