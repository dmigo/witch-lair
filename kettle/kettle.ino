#include "Scales.cpp"
#include "ScorePuzzle.cpp"
#include "TriggerLock.cpp"
#include "Sensor.cpp"
#include "Puzzle.cpp"

#define TARGET_WEIGHT_MIN 160
#define TARGET_WEIGHT_MAX 190

#define DOUT 3
#define SCK 2

#define LAST_RELAY_PIN 5
#define FURNICE_PIN 9

Scales* scales;
ScorePuzzle* weightPuzzle;
Puzzle* furnicePuzzle;
TriggerLock* lastLock;
Sensor* furnice;

void setup() {
  Serial.begin(9600);
  Serial.println("Version 1.0.0");
  Serial.println("Starting...");
  
  scales = new Scales(DOUT, SCK);
  scales->onCheck(onWeightCheck);
  weightPuzzle = new ScorePuzzle(10);
  lastLock = new TriggerLock(LAST_RELAY_PIN, isTheWeightPuzzleSolved);
  furnice = new Sensor(FURNICE_PIN, 100);
  furnice->onDrop(fireIsOn);
  furnicePuzzle = new Puzzle();
  
  Serial.println("Started");
}

void loop() {
  scales->check();
  lastLock->check();
  furnice->check();
}

void fireIsOn(){
  Serial.println("This girl is on fire!!!");
  furnicePuzzle->solve();
}

bool isTheWeightPuzzleSolved(){
  return weightPuzzle->isSolved();
}

void onWeightCheck(float weight) {
  if (!furnicePuzzle->isSolved() || isTheWeightPuzzleSolved())
    return;
    
  Serial.print("The weight is: ");
  Serial.println(weight);
  if (weight >= TARGET_WEIGHT_MIN
      && weight <= TARGET_WEIGHT_MAX)
    weightPuzzle->hit();
  else
    weightPuzzle->fail();
}

