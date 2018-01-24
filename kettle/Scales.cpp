#ifndef SCALES
#define SCALES

#define CHECK_INTERVAL 250

#include <HX711_ADC.h>

class Scales {
  private:
    void (*_onCheck)(float weight);
    int _dout;
    int _sck;
    float _weight = 0;
    HX711_ADC* _loadCell;
    long _lastCheck = 0;

    bool _isItTimeToCheck() {
      return millis() > _lastCheck + CHECK_INTERVAL;
    }

    void _check(float weight) {
      if (_onCheck != 0L) {
        _onCheck(weight);
        _lastCheck = millis();
      }
    }

  public:
    Scales(int dout, int sck) {
      _onCheck = 0L;
      _dout = dout;
      _sck = sck;
      _loadCell = new HX711_ADC(dout, sck);

      _loadCell->begin();
      long stabilisingtime = 5000; // tare preciscion can be improved by adding a few seconds of stabilising time
      _loadCell->start(stabilisingtime);
      _loadCell->setCalFactor(696.0); // user set calibration factor (float)
    }

    void onCheck(void (*onCheck)(float weight)) {
      _onCheck = onCheck;
    }

    void check() {
      _loadCell->update();
      if (_isItTimeToCheck()) {
        _weight = _loadCell->getData();
        _check(_weight);
      }
    }

    float getWeight(){
      return _weight;
    }
};
#endif


