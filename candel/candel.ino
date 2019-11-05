#include <DFPlayer_Mini_Mp3.h>

#define PLAYER_RX 0
#define PLAYER_TX 1
#include "Sequence.cpp"
#include <SoftwareSerial.h>



//SoftwareSerial mySoftwareSerial(PLAYER_RX, PLAYER_TX); // сюда подключаем плэйер

const int fragmentsCount = 4;
 int buttons [fragmentsCount] = {A1,A2,A3,A4};
 int indicators [fragmentsCount] = {7,6,5,4};
int relay = 11;
int completeSong = 1;
int completeSongDuration = 24000;

const int sequenceSize = 4; // длинна правильной последовательности
int pinsSequence[sequenceSize] = {1, 3, 2, 0}; // правильная последовательность

Sequence sequence(sequenceSize, pinsSequence);


  
void setup() {
  Serial.begin(9600);

  mp3_set_serial (Serial);
  mp3_set_volume(30);  // громкость крутить здесь

  for (int i = 0; i < fragmentsCount; i++) {
    pinMode (buttons[i],INPUT);
  }
  for (int i = 0; i < fragmentsCount; i++) {
    pinMode (indicators[i],OUTPUT);
  }
    for (int i = 0; i < fragmentsCount; i++) {
    digitalWrite (indicators[i],HIGH);
  }
  sequence.onSuccess(handleSuccess);
  sequence.onFailure(handleFail);  
  pinMode (relay,OUTPUT);
  digitalWrite(relay,LOW); 

 // Serial.begin(115200);
  //Serial.println("Version 0.1.1");
}


void deactivateAll() {
  for (int i = 0; i < fragmentsCount; i++) {
    digitalWrite (indicators[i],LOW);
  }
}

void activateAll() {
  for (int i = 0; i < fragmentsCount; i++) {
    digitalWrite (indicators[i],HIGH);
  }

}

void handleSuccess() {
 // Serial.println("Win!");
  mp3_play(1);
  delay(100);
  mp3_play(1);
  delay(100);
  deactivateAll();
  digitalWrite(relay,HIGH);
}

void handleFail() {
  //Serial.println("Fail!");
  mp3_play(2);
  delay(100);
  mp3_play(2);
  delay(1000);
  activateAll();
  digitalWrite(relay,LOW);
}




void loop() {

  for (int i = 0; i < fragmentsCount; i++) {
    if (analogRead (buttons[i]) > 65){
     digitalWrite(indicators[i],LOW);
     Serial.println(i);
    sequence.add(i);
    }
   

  }  

}
