#ifndef AMBIENT
#define AMBIENT

#include <Arduino.h>
#include "SoftwareSerial.h"

class Player {
  private:
    int _busy;
    SoftwareSerial* _serial;
    // --- Excecute the DFPlayer command with two parameters (folder and file numbers)
    void _execute(byte CMD, byte Par1, byte Par2)
    {
# define Start_Byte   0x7E
# define Version_Byte  0xFF
# define Command_Length 0x06
# define Acknowledge  0x00
# define End_Byte    0xEF
      // Calculate the checksum (2 bytes)
      uint16_t checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
      // Build the command line
      uint8_t Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
                                   Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte
                                 };
      // Send the command line to DFPlayer
      for (byte i = 0; i < 10; i++) _serial->write( Command_line[i]);
    }

  public:
    Player(int rx, int tx, int busy) {
      _busy = busy;
      pinMode(_busy, INPUT);
      _serial = new SoftwareSerial(rx, tx); // сюда подключаем плэйер
      _serial->begin(9600);
      _execute(0x3F, 0x00, 0x00);  // Send request for initialization parameters
      while (_serial->available() < 10) // Wait until initialization parameters are received (10 bytes)
        delay(30);            // have >20ms delays between commands
    }

    void play(int file) {
      _execute(0x0F, 0x01, file); // play (0x0F) file [ifile] from folder "01"
    }

    bool isBusy() {
      return digitalRead(_busy) == HIGH;
    }

    void setVolume(int volume) {
      _execute(0x06, 0x00, volume);
      delay(30);
    }
};

class Ambient {
  private:
    bool _isOn = false;
    int _fileName;
    Player* _player;


  public:
    Ambient(Player* player, int volume, int fileName) {
      _player = player;
      _fileName = fileName;

      _player->setVolume(volume);
    }


    void check() {
      if (_isOn) {
        if (!_player->isBusy())
          _player->play(_fileName);
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


