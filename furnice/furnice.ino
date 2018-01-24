#include "Puzzle.cpp"
#include "Lock.cpp"
#include "SimpleIndicator.cpp"
#include "Sensor.cpp"

#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

#define SOUND_FILE 1

#define PLAYER_RX 0
#define PLAYER_TX 1

SoftwareSerial mySoftwareSerial(PLAYER_RX, PLAYER_TX); // сюда подключаем плэйер

Puzzle* fursPuzzle;
Lock* furniceLock;
Lock* kettleLock;
SimpleIndicator* fire;
Sensor* furs;

void setup() {
  mySoftwareSerial.begin(9600);

  mp3_set_serial(mySoftwareSerial);
  mp3_set_volume(20);  // громкость крутить здесь
  
  fursPuzzle = new Puzzle();
  furniceLock = new Lock(6);
  fire = new SimpleIndicator(7);
  furs = new Sensor(8, 100);
  kettleLock = new Lock(9);
  furs->onDrop(onFursPlaced);
}

void loop() {
  furs->check();
}

void onFursPlaced(int pin){
  fursPuzzle->solve();
  fire->switchOn();
  furniceLock->open();
  kettleLock->open();
  mp3_play(SOUND_FILE);
}

