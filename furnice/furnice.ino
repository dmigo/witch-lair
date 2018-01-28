#include "Puzzle.cpp"
#include "Lock.cpp"
#include "SimpleIndicator.cpp"
#include "ThresholdSensor.cpp"

#include "SoftwareSerial.h"
#include <DFMiniMp3.h>

#define SOUND_FILE 1
#define PLAYER_RX_PIN 10
#define PLAYER_TX_PIN 11
#define PLAYER_VOLUME 20

#define FURNICE_DOOR_PIN 6
#define FIRE_PIN 7
#define KETTLE_PIN 8

#define FURS_PIN 9
#define FURS_DELAY 1500 // задержка перед срабатыванием мехов

Puzzle* fursPuzzle;
Lock* furniceLock;
Lock* kettleLock;
SimpleIndicator* fire;
ThresholdSensor* furs;

class Mp3Notify
{
  public:
    static void OnError(uint16_t errorCode)
    {
    }

    static void OnPlayFinished(uint16_t globalTrack)
    {
    }

    static void OnCardOnline(uint16_t code)
    {
    }

    static void OnCardInserted(uint16_t code)
    {
    }

    static void OnCardRemoved(uint16_t code)
    {
    }
};

SoftwareSerial serial(PLAYER_RX_PIN, PLAYER_TX_PIN);
DFMiniMp3<SoftwareSerial, Mp3Notify> player(serial);

void play() {
  player.playMp3FolderTrack(SOUND_FILE);
}


void setup() {
  serial.begin(9600);
  player.begin();
  player.setVolume(PLAYER_VOLUME);

  fursPuzzle = new Puzzle();
  furniceLock = new Lock(FURNICE_DOOR_PIN);
  furniceLock->close();
  
  fire = new SimpleIndicator(FIRE_PIN);
  kettleLock = new Lock(KETTLE_PIN);
  furs = new ThresholdSensor(FURS_PIN, FURS_DELAY);
  furs->onDrop(onFursPlaced);
}

void loop() {
  furs->check();
  player.loop();
}

void onFursPlaced(int pin) {
  fursPuzzle->solve();
  fire->switchOn();
  play();
  furniceLock->open();
  kettleLock->open();
}

