#ifndef PUZZLE
#define PUZZLE

#include <Arduino.h>

class Puzzle {
  private:
    bool _isSolved = false;

  public:
    Puzzle() {
    }

    bool isSolved(){
      return _isSolved;
    }

    void solve(){
      _isSolved = true;
    }
};
#endif

