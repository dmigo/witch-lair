//E+ - красный
//E- - черный
//A- - белый
//A+ - зеленый

  #define TARGET_WEIGHT_DISPERSION 7// разброс веса в процентах
#define TARGET_WEIGHT_MIN_ADDRESS 0
#define TARGET_WEIGHT_MAX_ADDRESS 2

#include "Mnemonic.cpp"

#include <HX711.h>
HX711 scale(A4, A5);    // parameter "gain" is ommited; the default value 128 is used by the library
enum State {SAVE, WORK_1, WORK_2, FINAL, CLOSED, ALL, ORDER1, ORDER2, ORDER3, ORDER_OPEN};
State state;
//Электромагнитный замок
#define LOCK 4
//Пин мехов
#define FURNICE_PIN 9
//Кнопка для сохранения значения
#define SAVE_BTN_PIN 8

//Вес на весах
float w;
float old_w = 444;
int val;
int minimum;
int maximum;
unsigned long previousMillis = 0;
unsigned long saveMillis = 0;
unsigned long closeMillis = 0;
const long interval = 6000;

Mnemonic minWeightStorage(TARGET_WEIGHT_MIN_ADDRESS);
Mnemonic maxWeightStorage(TARGET_WEIGHT_MAX_ADDRESS);

void setup() {
  Serial.begin(9600);
  Serial.println("Version 1.0.0");
  Serial.println("Starting...");

  minimum = minWeightStorage.recall();
  maximum = maxWeightStorage.recall();
  pinMode(LOCK, OUTPUT);
  pinMode(SAVE_BTN_PIN, INPUT_PULLUP);
  pinMode(FURNICE_PIN, INPUT_PULLUP);
  scale.read();
  scale.read_average(40);
  scale.get_value(5);
  scale.get_units(5);
  scale.set_scale(280.f);
  scale.tare();
  scale.read();
  scale.read_average(40);
  scale.get_value(5);
  scale.get_units(5);
  state = WORK_1;

  Serial.println("Started");
  reportTargetWeight();
}

void loop() {
  unsigned long currentMillis = millis();
  if (digitalRead(SAVE_BTN_PIN) == LOW)
  {
    if (currentMillis - saveMillis >= 1000)
    {
      saveMillis = currentMillis;
      state = SAVE;
    }
  }
  else
  {
    saveMillis = currentMillis;
  }
  switch (state)
  {
    case SAVE:
      minimum = w - (w * TARGET_WEIGHT_DISPERSION / 100);
      maximum = w + (w * TARGET_WEIGHT_DISPERSION / 100);
    
      minWeightStorage.memorize(minimum);
      maxWeightStorage.memorize(maximum);
      
      reportTargetWeight();
      saveMillis = currentMillis;
      state = WORK_1;
      break;

    case WORK_1:
      if (digitalRead(FURNICE_PIN) == LOW)
        state = WORK_2;
      break;
      
    case WORK_2:
      w = scale.get_units(10);
      Serial.print("The weight is: ");
      Serial.println(w);
      if (w < 0)w = w * (-1);
      float kg = 185;
      float gr = 1000;
      float weight = (w / kg) * gr;
      w = weight;
      if ((old_w > (w + 1)) || (old_w < (w - 1)))
      {
        old_w = w;
      }
      if ((w >= minimum) && (w <= maximum))
      {
        if (currentMillis - previousMillis >= interval)
        {
          digitalWrite(LOCK, HIGH);
        }
        closeMillis = currentMillis;
      }
      else
      {
        if (currentMillis - closeMillis >= interval)
        {
          digitalWrite(LOCK, LOW);
        }
        previousMillis = currentMillis;
      }
      state = WORK_2;
      break;
  }
}
void reportTargetWeight() {
  Serial.print("Target weight: [");
  Serial.print(minimum);
  Serial.print(", ");
  Serial.print(maximum);
  Serial.println("]");
}
