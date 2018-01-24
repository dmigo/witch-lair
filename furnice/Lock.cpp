#ifndef LOCK
#define LOCK

#include <Arduino.h>

class Lock {
  private:
    bool _isOpen = false;
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
    Lock(int pin) {
      _pin = pin;
      pinMode(_pin, OUTPUT);
      _close();
    }

    void open() {
      _setState(true);
    }
    void close() {
      _setState(false);
    }
};
#endif

