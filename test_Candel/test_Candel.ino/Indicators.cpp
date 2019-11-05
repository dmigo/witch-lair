#ifndef INDICATORS_H
#define INDICATORS_H
#include <Arduino.h>

class SimpleIndicator {
  private:
    int _pin;
    bool _state;

    void _setState(int newState) {
      if (_state != newState) {
        _state = newState;
        digitalWrite(_pin, _state);
      }
    }

  public:
    SimpleIndicator(int pin) {
      _pin = pin;
      pinMode(_pin, OUTPUT);
    }

    void switchOn() {
      _setState(HIGH);
    }
    void switchOff() {
      _setState(LOW);
    }
};


class BlinkingIndicator {
  private:
    SimpleIndicator *_indicator;
    int _delayOn;
    int _delayOff;

    int _timesToBlink;
    long _startBlinking;

  public:
    BlinkingIndicator(SimpleIndicator* indicator, int delayOn, int delayOff) {
      _indicator = indicator;

      _delayOn = delayOn;
      _delayOff = delayOff;

      _timesToBlink = 0;
      _startBlinking = 0;
    }

    bool _isOn(int offset, int times) {
      for (int i = 0; i < times; i++)
        if (offset >= 0 && offset < _delayOn)
          return true;
        else
          offset -= _delayOn + _delayOff;
      return false;
    }

    bool _isOff(int offset, int times) {
      for (int i = 0; i < times; i++)
        if (offset >= _delayOn && offset < _delayOn + _delayOff)
          return true;
        else
          offset -= _delayOn + _delayOff;
      return false;
    }

    void check() {
      if (_timesToBlink < 1)
        return;

      long current = millis();
      long offset = current - _startBlinking;

      bool isOn = _isOn(offset, _timesToBlink);
      bool isOff = _isOff(offset, _timesToBlink);

      if (isOn)
        _indicator->switchOn();
      else if (isOff)
        _indicator->switchOff();
      else
        _timesToBlink = 0;
    }

    void blinkNTimes(int n) {
      _timesToBlink = n;
      _startBlinking = millis();
    }
};
#endif


