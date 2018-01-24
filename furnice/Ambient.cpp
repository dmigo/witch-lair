#ifndef AMBIENT
#define AMBIENT

#include <Arduino.h>

class Ambient {
  private:
    bool _isOn = false;
    void (*_play)();
    bool (*_isBusy)();

  public:
    Ambient(void (*play)(), bool (*isBusy)()) {
      _play = play;
      _isBusy = isBusy;
    }

    void check() {
      if (_isOn) {
        if (!_isBusy()) {
          _play();
        }
      }
    }

    void switchOn() {
      _isOn = true;
    }
    void switchOff() {
      _isOn = false;
    }
};
#endif


