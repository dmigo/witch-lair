#ifndef MNEMONIC
#define MNEMONIC

#include <EEPROM.h>
#include <Arduino.h>

class Mnemonic {
  private:
    int _address;
    int _size;

    int _read(int addr, int size) {
      byte raw[2];
      for (byte i = 0; i < size; i++) raw[i] = EEPROM.read(addr + i);
      int &num = (int&)raw;
      return num;
    }
    void _write(int addr, int size, int num) {
      byte raw[size];
      (int&)raw = num;
      for (byte i = 0; i < size; i++)
        EEPROM.write(addr + i, raw[i]);
    }

  public:
    Mnemonic(int address) {
      _address = address;
      _size = 2;
    }

    int recall(){
      return _read(_address, _size);
    }

    void memorize(int value){
      _write(_address, _size, value);
    }
};
#endif
