#include <Arduino.h>
#include "Sensor.cpp"

#define DEBOUNCE_TIME 1

class Button {
  private:
    Sensor* _sensor;
  public:
    Button(int pin) {
      _sensor = new Sensor(pin, DEBOUNCE_TIME);
    }

    void check(){
      _sensor->check();
    }

    void onPress(void (*callback)(int)) {
      _sensor->onDrop(callback);
    }
    void onRelease(void (*callback)(int)) {
      _sensor->onRise(callback);
    }

    void deactivate(long milliseconds) {
      _sensor->deactivate(milliseconds);
    }
    void deactivate() {
      _sensor->deactivate();
    }
    void activate() {
      _sensor->activate();
    }
};


