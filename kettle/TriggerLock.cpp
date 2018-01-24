#ifndef TRIGGER_LOCK
#define TRIGGER_LOCK

#include <Arduino.h>

class TriggerLock {
  private:
    bool _isOpen = false;
    bool (*_check)();
    int _pin;

    _setState(bool newState) {
      if (newState != _isOpen) {
        _open();
      }
    }
    void _open() {
      _isOpen = true;
      digitalWrite(_pin, LOW);
    }
    void _close() {
      _isOpen = false;
      digitalWrite(_pin, HIGH);
    }

  public:
    TriggerLock(int pin, bool (*check)()) {
      _check = check;
      _pin = pin;
      pinMode(_pin, OUTPUT);
      _close();
    }

    void check() {
      bool newState = _check();
      _setState(newState);
    }
};
#endif

