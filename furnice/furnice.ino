#include "Puzzle.cpp"
#include "Lock.cpp"
#include "SimpleIndicator.cpp"
#include "Sensor.cpp"

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

Puzzle* fursPuzzle;
Lock* furniceLock;
Lock* kettleLock;
SimpleIndicator* fire;
Sensor* furs;

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

SoftwareSerial serialo(PLAYER_RX_PIN, PLAYER_TX_PIN);
DFMiniMp3<SoftwareSerial, Mp3Notify> player(serialo);

void play() {
  player.playMp3FolderTrack(SOUND_FILE);
}


void setup() {
  serialo.begin(9600);
  player.begin();
  player.setVolume(PLAYER_VOLUME);

  furs = new Sensor(FURS_PIN, 100);
  fursPuzzle = new Puzzle();
  furniceLock = new Lock(FURNICE_DOOR_PIN);
  furniceLock->close();
  
  fire = new SimpleIndicator(FIRE_PIN);
  kettleLock = new Lock(KETTLE_PIN);
  furs->onChange(onFursPlaced);
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

