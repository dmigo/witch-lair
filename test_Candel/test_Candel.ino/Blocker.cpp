#include <Arduino.h>

class Blocker {
  private:
    bool _blocked;
    
    bool _blockedOnTime;
    long _whenBlocked;
    long _duration;

  public:
    Blocker() {
      _blocked = false;
      _blockedOnTime = false;
    }
    void check() {
      if (!_blockedOnTime)
        return;
        
      if (millis() > _whenBlocked + _duration)
        _blockedOnTime = false;
    }
    
    void block(long milliseconds) {
      _whenBlocked = millis();
      _duration = milliseconds;
      _blockedOnTime = true;
    }
    
    void block() {
      _blocked = true;
    }
    void unblock() {
      _blocked = false;
      _blockedOnTime = false;
    }
    bool isBlocked() {
      return _blocked || _blockedOnTime;
    }
};

