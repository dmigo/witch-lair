#include "ScorePuzzle.cpp"
#include "TriggerLock.cpp"
#include "Sensor.cpp"
#include "Puzzle.cpp"
#include "Mnemonic.cpp"

#define TARGET_WEIGHT_MIN_ADDRESS 0
#define TARGET_WEIGHT_MAX_ADDRESS 2
#define TARGET_WEIGHT_DISPERSION 5// разброс веса в процентах
#define STABILISING_TIME 2000 // время на стабилизацию весов (чем больше - тем точнее тарирование)
const float CALIBRATION_FACTOR = 696.0; // калибровочный фактор

#define DOUT 3
#define SCK 2

#define LAST_RELAY_PIN 4
#define FURNICE_PIN 9

#define SET_WEIGHT_PIN 8

#include <HX711_ADC.h>

//HX711 constructor (dout pin, sck pin)
HX711_ADC LoadCell(DOUT, SCK);
long t;

ScorePuzzle* weightPuzzle;
Puzzle* furnicePuzzle;
TriggerLock* lastLock;
Sensor* furnice;
Sensor* resetWeight;

Mnemonic minWeightStorage(TARGET_WEIGHT_MIN_ADDRESS);
Mnemonic maxWeightStorage(TARGET_WEIGHT_MAX_ADDRESS);

int minWeight = 0;
int maxWeight = 0;
float weight;

void setup() {
  Serial.begin(9600);
  Serial.println("Version 1.0.0");
  Serial.println("Starting...");

  LoadCell.begin();
  LoadCell.start(STABILISING_TIME);
  LoadCell.setCalFactor(CALIBRATION_FACTOR);

  weightPuzzle = new ScorePuzzle(10);
  lastLock = new TriggerLock(LAST_RELAY_PIN, isTheWeightPuzzleSolved);
  furnice = new Sensor(FURNICE_PIN, 100);
  furnice->onDrop(fireIsOn);
  furnicePuzzle = new Puzzle();

  resetWeight = new Sensor(SET_WEIGHT_PIN, 100);
  resetWeight->onRise(setTargetWeight);
  minWeight = minWeightStorage.recall();
  maxWeight = maxWeightStorage.recall();

  Serial.println("Started");
  reportTargetWeight();
}

void loop() {
  updateWeight();
  lastLock->check();
  furnice->check();
  resetWeight->check();
}

void updateWeight() {
  LoadCell.update();
  if (millis() > t + 250) {
    weight = LoadCell.getData();
    t = millis();
    checkWeight();
  }
}

void fireIsOn() {
  Serial.println("This girl is on fire!!!");
  furnicePuzzle->solve();
}

bool isTheWeightPuzzleSolved() {
  return weightPuzzle->isSolved();
}

void checkWeight() {
  if (!furnicePuzzle->isSolved() || isTheWeightPuzzleSolved())
    return;

  Serial.print("The weight is: ");
  Serial.println(weight);
  if (weight >= minWeight
      && weight <= maxWeight)
    weightPuzzle->hit();
  else
    weightPuzzle->fail();
}

void setTargetWeight() {
  Serial.print("The current weight is: ");
  Serial.println(weight);

  minWeight = weight - (weight * TARGET_WEIGHT_DISPERSION / 100);
  maxWeight = weight + (weight * TARGET_WEIGHT_DISPERSION / 100);

  minWeightStorage.memorize(minWeight);
  maxWeightStorage.memorize(maxWeight);

  reportTargetWeight();
}

void reportTargetWeight() {
  Serial.print("Target weight: [");
  Serial.print(minWeight);
  Serial.print(", ");
  Serial.print(maxWeight);
  Serial.println("]");
}

