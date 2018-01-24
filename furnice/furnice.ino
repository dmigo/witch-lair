#include "Puzzle.cpp"
#include "Lock.cpp"
#include "SimpleIndicator.cpp"
#include "Sensor.cpp"
#include "Ambient.cpp"

#define SOUND_FILE 1
#define PLAYER_RX_PIN 10
#define PLAYER_TX_PIN 11
#define PLAYER_BUSY_PIN 4
#define PLAYER_VOLUME 20

#define FURNICE_DOOR_PIN 6
#define FIRE_PIN 7
#define KETTLE_PIN 8
#define FURS_PIN 12

Puzzle* fursPuzzle;
Lock* furniceLock;
Lock* kettleLock;
SimpleIndicator* fire;
Sensor* furs;
Player* player;
Ambient* fireAmbient;

void setup() {
  fursPuzzle = new Puzzle();
  furniceLock = new Lock(FURNICE_DOOR_PIN);
  player = new Player(PLAYER_RX_PIN, PLAYER_TX_PIN, PLAYER_BUSY_PIN);
  fireAmbient = new Ambient(player, PLAYER_VOLUME, SOUND_FILE);
  fire = new SimpleIndicator(FIRE_PIN);
  furs = new Sensor(FURS_PIN, 100);
  kettleLock = new Lock(KETTLE_PIN);
  
  furs->onDrop(onFursPlaced);
}

void loop() {
  furs->check();
  fireAmbient->check();
}

void onFursPlaced(int pin){
  fursPuzzle->solve();
  fire->switchOn();
  fireAmbient->switchOn();
  furniceLock->open();
  kettleLock->open();
}

