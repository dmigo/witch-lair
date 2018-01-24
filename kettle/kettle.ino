#include "Scales.cpp"
#include "ScorePuzzle.cpp"
#include "TriggerLock.cpp"
#include "Sensor.cpp"
#include "Puzzle.cpp"

#define TARGET_WEIGHT_MIN 570
#define TARGET_WEIGHT_MAX 595

Scales* scales;
ScorePuzzle* weightPuzzle;
Puzzle* furnicePuzzle;
TriggerLock* lastLock;
Sensor* furnice;

void setup() {
  Serial.begin(9600);
  Serial.println("Version 1.0.0");
  Serial.println("Starting...");
  
  scales = new Scales(3, 2);
  scales->onCheck(onWeightCheck);
  weightPuzzle = new ScorePuzzle(10);
  lastLock = new TriggerLock(5, isTheWeightPuzzleSolved);
  furnice = new Sensor(9, 100);
  furnice->onDrop(fireIsOn);
  furnicePuzzle = new Puzzle();
  
  Serial.println("Started");
}

void loop() {
  scales->check();
  lastLock->check();
}

void fireIsOn(){
  furnicePuzzle->solve();
}

bool isTheWeightPuzzleSolved(){
  return weightPuzzle->isSolved();
}

void onWeightCheck(float weight) {
  if (isTheWeightPuzzleSolved())
    return;
    
  Serial.print("The weight is: ");
  Serial.println(weight);
  if (weight >= TARGET_WEIGHT_MIN
      && weight <= TARGET_WEIGHT_MAX)
    weightPuzzle->hit();
  else
    weightPuzzle->fail();
}

