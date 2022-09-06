// Работает
// Выводит микросекунды
// https://alexgyver.ru/lessons/pointers/ - Указатели. Переписать
// 1

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//// 8622
//const int pinLL = D5; // LM out2, in5 // esp
//const int pinLH = D6; // LM out1, in 7
//const int pinRL = D7; // LM out14, in9
//const int pinRH = D8; // на плате - D3, // LM out13, in9

// esp32
const int pinLL = 4; // LM out2, in5
const int pinLH = 0; // LM out1, in 7
const int pinRL = 2; // LM out14, in9
const int pinRH = 15; // LM out13, in9



//volatile boolean isTimeOn = false;
volatile boolean isShotInProgress = false;
volatile unsigned long timeShot = 0;
volatile unsigned long maxTimeShot = 500000; //1000 микросекунд

volatile unsigned long tickLL = 0;
volatile unsigned long tickLH = 0;
volatile unsigned long tickRL = 0;
volatile unsigned long tickRH = 0;

volatile boolean isComletedLL = false;
volatile boolean isComletedLH = false;
volatile boolean isComletedRL = false;
volatile boolean isComletedRH = false;

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  SerialBT.println("The device started, now you can pair it with bluetooth!");

  // pinmode
  pinMode(pinLL, INPUT_PULLUP);
  pinMode(pinLH, INPUT_PULLUP);
  pinMode(pinRL, INPUT_PULLUP);
  pinMode(pinRH, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pinLL), shotLL, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinLH), shotLH, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinRL), shotRL, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinRH), shotRH, CHANGE);

  timeShot = micros();
  printResult();
}

void loop() {
  if (isShotInProgress) {
    if ((micros() - timeShot) > maxTimeShot) { // Время истекло
      isShotInProgress = false;
//      printResult();
      btResult();
      resetPins();
    }
  }
}

void btResult() {
  SerialBT.print("tickLL - ");
  SerialBT.println(tickLL);

  SerialBT.print("tickLH - ");
  SerialBT.println(tickLH);

  SerialBT.print("tickRL - ");
  SerialBT.println(tickRL);

  SerialBT.print("tickRH - ");
  SerialBT.println(tickRH);

//  Serial.println(micros());
//  Serial.println("---------------");
}

void printResult() {
  Serial.print("tickLL - ");
  Serial.println(tickLL);

  Serial.print("tickLH - ");
  Serial.println(tickLH);

  Serial.print("tickRL - ");
  Serial.println(tickRL);

  Serial.print("tickRH - ");
  Serial.println(tickRH);

  Serial.println(micros());
  Serial.println("---------------");
}

// ICACHE_RAM_ATTR void https://randomnerdtutorials.com/interrupts-timers-esp8266-arduino-ide-nodemcu/
//IRAM_ATTR void shotLH() {


ICACHE_RAM_ATTR void shotLL() {
  if (!isShotInProgress) {
    isShotInProgress = true;  // Сработал первый датчик, считаем время
    timeShot = micros();
  }

  if (!isComletedLL) {  // Если это первая волна, запоминаем время
    tickLL = micros();
    isComletedLL = true;
  }
}

ICACHE_RAM_ATTR void shotLH() {
  if (!isShotInProgress) {
    isShotInProgress = true;  // Сработал первый датчик, считаем время
    timeShot = micros();
  }

  if (!isComletedLH) {  // Если это первая волна, запоминаем время
    tickLH = micros();
    isComletedLH = true;
  }
}

ICACHE_RAM_ATTR void shotRL() {
  if (!isShotInProgress) {
    isShotInProgress = true;  // Сработал первый датчик, считаем время
    timeShot = micros();
  }

  if (!isComletedRL) {  // Если это первая волна, запоминаем время
    tickRL = micros();
    isComletedRL = true;
  }
}

ICACHE_RAM_ATTR void shotRH() {
  if (!isShotInProgress) {
    isShotInProgress = true;  // Сработал первый датчик, считаем время
    timeShot = micros();
  }

  if (!isComletedRH) {  // Если это первая волна, запоминаем время
    tickRH = micros();
    isComletedRH = true;
  }
}

void resetPins() {
  isComletedLL = false;
  isComletedLH = false;
  isComletedRL = false;
  isComletedRH = false;

  tickLL = 0;
  tickLH = 0;
  tickRL = 0;
  tickRH = 0;
}
