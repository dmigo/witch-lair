//E+ - красный
//E- - черный
//A- - белый
//A+ - зеленый

#define TARGET_WEIGHT_DISPERSION 10// разброс веса

#include <HX711.h>
HX711 scale(3, 2);    // parameter "gain" is ommited; the default value 128 is used by the library
enum State {SAVE, WORK_1, WORK_2, FINAL, CLOSED, ALL, ORDER1, ORDER2, ORDER3, ORDER_OPEN};
State state;
#include <EEPROM.h>
//Электромагнитный замок
#define LOCK 4
//Пин мехов
#define FURNICE_PIN 9
//Кнопка для сохранения значения
#define SAVE_BTN_PIN 8

//Вес на весах
float w;
float old_w = 444;
//Сохраненный вес
int lev;
//Погрешность
int calibr = TARGET_WEIGHT_DISPERSION;
int val;
int minimum;
int maximum;
unsigned long previousMillis = 0;
unsigned long saveMillis = 0;
unsigned long closeMillis = 0;
const long interval = 3000;
void setup() {
  Serial.begin(9600);
  Serial.println("Version 1.0.0");
  Serial.println("Starting...");

  lev = (EEPROM.read(0) * 1000) + (EEPROM.read(1) * 100) + (EEPROM.read(2) * 10) + EEPROM.read(3);
  minimum = lev - calibr;
  maximum = lev + calibr;
  pinMode(LOCK, OUTPUT);
  pinMode(SAVE_BTN_PIN, INPUT_PULLUP);
  pinMode(FURNICE_PIN, INPUT_PULLUP);
  scale.read();
  scale.read_average(20);
  scale.get_value(5);
  scale.get_units(5);
  scale.set_scale(2280.f);
  scale.tare();
  scale.read();
  scale.read_average(20);
  scale.get_value(5);
  scale.get_units(5);
  state = WORK_1;

  Serial.println("Started");
  reportTargetWeight();
}

void loop() {
  unsigned long currentMillis = millis();
  if (digitalRead(SAVE_BTN_PIN) == HIGH)
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
      int new_w, r1, r2, r3, r4;
      new_w = w;
      r1 = new_w / 1000;
      r2 = (new_w % 1000) / 100;
      r3 = (new_w % 100) / 10;
      r4 = new_w % 10;
      EEPROM.write(0, r1);
      EEPROM.write(1, r2);
      EEPROM.write(2, r3);
      EEPROM.write(3, r4);
      lev = (r1 * 1000) + (r2 * 100) + (r3 * 10) + r4;
      minimum = lev - calibr;
      maximum = lev + calibr;
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
