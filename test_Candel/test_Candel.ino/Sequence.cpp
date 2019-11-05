
class Sequence {
  private:
    int _sequenceSize;
    int *_sequence;

    int _currentIndex;
    int *_attempt;

    bool _isSolved = false;
    void (*_onSuccess)();
    void (*_onFailure)();

    void _solve() {
      _isSolved = true;
      if (_onSuccess != 0L) {
        _onSuccess();
      }
    }

    void _fail() {
      _currentIndex = 0;
      if (_onFailure != 0L) {
        _onFailure();
      }
    }

    bool _attemptSuccessful() {
      bool result = true;

      for (int i = 0; i < _sequenceSize; i++)
        result = result && _attempt[i] == _sequence[i];

      return result;
    }

  public:
    Sequence(int sequenceSize, int* sequence) {
      _isSolved = false;
      _currentIndex = 0;

      _sequenceSize = sequenceSize;
      _sequence = new int[sequenceSize];
      _attempt = new int[sequenceSize];
      for (int i = 0; i < sequenceSize; i++) {
        _sequence[i] = sequence[i];
        _attempt[i] = -1;
      }
    }

    void add(int element) {
      if (_isSolved)
        return;

      _attempt[_currentIndex] = element;
      _currentIndex++;

      if (_currentIndex == _sequenceSize)
        if (_attemptSuccessful())
          _solve();
        else
          _fail();
    }

    bool isSolved() {
      return _isSolved;
    }

    void onSuccess(void (*callback)()) {
      _onSuccess = callback;
    }
    void onFailure(void (*callback)()) {
      _onFailure = callback;
    }
};

