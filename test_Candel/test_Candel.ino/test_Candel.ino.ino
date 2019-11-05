#define ADDRESS 11

#define SOLVED 1
#define NOT_SOLVED 0

#define PLAYER_RX 0
#define PLAYER_TX 1

#include <Wire.h>

#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

#include "Button.cpp"
#include "Indicators.cpp"
#include "Sequence.cpp"

int pinToId(int pin) {
  switch (pin) {
    case 4:
      return 0;
      break;
    case 5:
      return 1;
      break;
    case 6:
      return 2;
      break;
    case 7:
      return 3;
      break;
  }
}

SoftwareSerial mySoftwareSerial(PLAYER_RX, PLAYER_TX); // сюда подключаем плэйер

const int fragmentsCount = 4;
 buttons[fragmentsCount] = {
  Button(A1),
  Button(A2),
  Button(A3),
  Button(A4),
};
 indicators[fragmentsCount] = {
  SimpleIndicator(4),
  SimpleIndicator(5),
  SimpleIndicator(6),
  SimpleIndicator(7),

};
Button exampleButton(12);
SimpleIndicator exampleIndicator(A3);

BlinkingIndicator* red;
SimpleIndicator* green;

//int fragments [fragmentsCount] = { // фрагменты трэка
//  1,
//  2,
//  3,
//  4,
//  5,
//  6
//};
int completeSong = 7;
//int durations [fragmentsCount] = { // длинны фрагментов
  //100,
  //100,
  //100,
  //100,
  //100,
  //100
//};
int completeSongDuration = 24000;
int failDuration = 1000;

const int sequenceSize = 4; // длинна правильной последовательности
int pinsSequence[sequenceSize] = {1, 3, 2, 0}; // правильная последовательность
                                   // 0, 1, 2, 4, 5, 6
Sequence sequence(sequenceSize, pinsSequence);

void setup() {
  mySoftwareSerial.begin(9600);

  mp3_set_serial(mySoftwareSerial);
  mp3_set_volume(20);  // громкость крутить здесь

  for (int i = 0; i < fragmentsCount; i++) {
    buttons[i].onPress(handleButtonPress);
    buttons[i].onRelease(handleButtonRelease);
  }
  SimpleIndicator* simpleRed = new SimpleIndicator(10);
  red = new BlinkingIndicator(simpleRed, 1000, 500);
  green = new SimpleIndicator(11);
  
  //exampleButton.onPress(handleExampleButtonPress);
  //exampleButton.onRelease(handleExampleButtonRelease);
  sequence.onSuccess(handleSuccess);
  sequence.onFailure(handleFail);

  Wire.begin(ADDRESS);
  Wire.onRequest(requestEvent);
  
  //Serial.begin(115200);
  //Serial.println("Version 0.1.1");
}

void deactivateAll(int duration) {
  for (int i = 0; i < fragmentsCount; i++) {
    buttons[i].deactivate(duration);
  }
  exampleButton.deactivate(duration);
}

void deactivateAll() {
  for (int i = 0; i < fragmentsCount; i++) {
    buttons[i].deactivate();
  }
  exampleButton.deactivate();
}

void activateAll() {
  for (int i = 0; i < fragmentsCount; i++) {
    buttons[i].activate();
  }
  exampleButton.activate();
}

void handleButtonPress(int pin) {
  deactivateAll();
  int id = pinToId(pin);

  
  buttons[id].activate();
  indicators[id].switchOn();
}


void handleButtonRelease(int pin) {
  int id = pinToId(pin);
  //Serial.print("Button release ");
  //Serial.println(pin);  
  mp3_play(fragments[id]);
  indicators[id].switchOff();
  
  activateAll();
  deactivateAll(durations[id]);
  
  sequence.add(id);
}

void handleExampleButtonPress(int pin) {
  //Serial.println("Button press example");  
  deactivateAll();
  exampleButton.activate();
  exampleIndicator.switchOn();
}


void handleExampleButtonRelease(int pin) {
  //Serial.println("Button release example");
  mp3_play(completeSong);
  exampleIndicator.switchOff();
  activateAll();
  deactivateAll(completeSongDuration);
}

void handleSuccess() {
  //Serial.println("Win!");
  mp3_play(8);
  delay(10);
  mp3_play(8);
  deactivateAll();
  green->switchOn();
}

void handleFail() {
  //Serial.println("Fail!");
  mp3_play(9);
  delay(10);
  mp3_play(9);
  deactivateAll(failDuration);
  red->blinkNTimes(3);
}

void loop() {
  if analogRead (button[i]>35
  
  for (int i = 0; i < fragmentsCount; i++) {
    buttons[i].check();
  }
  exampleButton.check();
  red->check();
}

//void requestEvent() {
  //if (sequence.isSolved())
    //Wire.write(SOLVED);
  //else
    //Wire.write(NOT_SOLVED);
//}
