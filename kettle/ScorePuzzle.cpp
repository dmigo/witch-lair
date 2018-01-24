#ifndef SCORE_PUZZLE
#define SCORE_PUZZLE

#include <Arduino.h>

class ScorePuzzle {
  private:
    bool _isSolved = false;
    int _targetHits = 0;
    int _hits = 0;

  public:
    ScorePuzzle(int targetHits) {
      _targetHits = targetHits;
    }

    bool isSolved() {
      return _isSolved;
    }

    void hit() {
      _hits++;
      if (_hits >= _targetHits)
        _isSolved = true;
    }
    void fail(){
      _hits = 0;
    }
};
#endif
