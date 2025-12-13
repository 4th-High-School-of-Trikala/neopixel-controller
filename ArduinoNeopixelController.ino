// Neopixel stuff

#define NEOPIXEL_PIN 6
#define NEOPIXEL_COUNT 150

#define IR_PIN 2
#define RELAY_PIN 10
// #define IR

#include <IRremote.h>

#include "lights.h"

LightsModule* lights = nullptr;

void PRG_PROXIMITY(LightsModule* controller) {
  controller->ColorWipeAll(Color {127, 0, 0}, 5);
}

void PRG17(LightsModule* controller) {
  for (int i = 0; i < 2; i++) {
    // gold
    controller->Fade(Color {218, 165, 32});
  }
}

void PRG16(LightsModule* controller) {
  controller->Rainbow(2, true);
}

void PRG15(LightsModule* controller) {
  for (int i = 0; i < 2; i++) {
    controller->Fade(Color {127, 0, 255});
  }
}

void PRG14(LightsModule* controller) {
  controller->Rainbow();
}

void PRG13(LightsModule* controller) {
  for (int i = 0; i < 2; i++) {
    controller->Fade(Color { 0, 255, 255 });
  }
}

void PRG12(LightsModule* controller) {
  for (int i = 0; i < 2; i++) {
    controller->Fade(Color { 255, 0, 0 });
  }
}

void PRG11(LightsModule* controller) {
  for (int i = 0; i < 2; i++) {
    controller->Fade(Color { 0, 255, 0 });
  }
}

void PRG10(LightsModule* controller) {
  controller->Sprinkle(Color {180, 180, 180});
  delay(400);
  
  controller->Sprinkle(Color {0, 180, 0});
  delay(400);

  controller->BlinkAllToColor(Color {100, 100, 100}, 50);
}

void PRG9(LightsModule* controller) {
  controller->Sprinkle(Color {180, 180, 180});
  delay(400);
  
  controller->Sprinkle(Color {0, 180, 0});
  delay(400);

  controller->Snake(Color {0, 180, 0}, 1, 30);
}

void PRG8(LightsModule* controller) {
  controller->Sprinkle(Color {180, 180, 180});
  delay(400);
  
  controller->Sprinkle(Color {180, 0, 0});
  delay(400);

  controller->BlinkAllToColor(Color {100, 100, 100}, 50);
}

void PRG7(LightsModule* controller) {
  controller->Sprinkle(Color {180, 180, 180});
  delay(400);
  
  controller->Sprinkle(Color {180, 0, 0});
  delay(400);

  controller->Snake(Color {180, 0, 0}, 1, 30);
}

void PRG6(LightsModule* controller) {
  for (int i = 0; i < 2; i++) {
    controller->Fade(Color { 63, 63, 255 });
  }
}

void PRG5(LightsModule* controller) {
  controller->MergeSnake(Color {0, 0, 127}, Color {127, 127, 127}, 12);
  delay(50);
}

void PRG4(LightsModule* controller) {
  controller->Merge(Color {0, 0, 127}, Color {127, 127, 127});
  delay(50);
}

void PRG3(LightsModule* controller) {
  controller->MergeSnake(Color {0, 0, 127}, Color {127, 127, 127}, 12);
  delay(50);
  controller->Snake(Color {0, 0, 127}, 1, 30);
  delay(50);
  controller->Sprinkle(Color {180, 180, 180});
}

void PRG2(LightsModule* controller) {
  controller->Sprinkle(Color {180, 180, 180});
  delay(400);
  
  controller->Sprinkle(Color {0, 0, 180});
  delay(400);

  controller->BlinkAllToColor(Color {100, 100, 100}, 50);
}

void PRG1(LightsModule* controller) {
  controller->Sprinkle(Color {180, 180, 180});
  delay(400);
  
  controller->Sprinkle(Color {0, 0, 180});
  delay(400);

  controller->Snake(Color {0, 0, 127}, 1, 30);
}

void PRG0(LightsModule* controller) {
  controller->SetAllToColor(Color {0, 0, 0});
}

void(*ProgramList[])(LightsModule*) = {/*PRG1, PRG2,*/ PRG3, PRG4, PRG5, PRG6, PRG7, /*PRG8,*/ PRG9, PRG10, PRG11, PRG12, PRG13, PRG14, PRG15, PRG16, PRG17};

void ProgramChooseRandom() {
  int i = random(0, sizeof(ProgramList) / sizeof(void(*)(LightsModule*)));
  lights->SetProgram(ProgramList[i]);
  Serial.print("Now running program at index ");
  Serial.println(i);
}

void setup() {
  randomSeed(analogRead(0));
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  Serial.begin(115200);
  Serial.print("FRAM ST: ");
  Serial.println(freeRam());

  #ifdef IR
  IrReceiver.begin(IR_PIN);
  #endif

  lights = new LightsModule{NEOPIXEL_PIN, NEOPIXEL_COUNT};
  lights->SetProgram(PRG4);

  #ifdef IR
  Serial.println("IR CONTROLLER MENU");
  Serial.println("0: PRG0");
  Serial.println("1: PRG1");
  Serial.println("2: PRG2");
  Serial.println("3: PRG3");
  Serial.println("4: PRG4");
  Serial.println("5: PRG5");
  Serial.println("6: PRG5");
  Serial.println("7: PRG5");
  Serial.println("8: PRG5");
  Serial.println("9: PRG5");
  Serial.println("10: PRG5");
  Serial.println("11: PRG5");
  Serial.println("12: PRG5");
  Serial.println("-: BRIGHTNESS DECREASE");
  Serial.println("+: BRIGHTNESS INCREASE");
  Serial.println("CH-: MOTOR DISABLE");
  Serial.println("CH+: MOTOR ENABLE");
  Serial.println("CH: SWITCH TO RED WARNING");
  Serial.println("EQ: RANDOMLY CHOOSE PROGRAM");
  #endif

  pingTimer = millis(); // Start now.
}

void loop() {
  ProgramChooseRandom();
  // delay(50);

  lights->Loop();

  // if (digitalRead(PROXIMITY_PIN) == HIGH) {
  //   lights->SetProgram(PRG_PROXIMITY);
  // }

  #ifdef IR
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.command);
    switch (IrReceiver.decodedIRData.command) {
      case 22:
      lights->SetProgram(PRG0);
      break;

      case 12:
      lights->SetProgram(PRG1);
      break;

      case 24:
      lights->SetProgram(PRG2);
      break;

      case 94:
      lights->SetProgram(PRG3);
      break;

      case 8:
      lights->SetProgram(PRG4);
      break;

      case 28:
      lights->SetProgram(PRG5);
      break;

      case 7:
      lights->DecreaseBrightness();
      break;

      case 21:
      lights->IncreaseBrightness();
      break;

      case 71:
      //digitalWrite(MOTOR_CONTROL_PIN, LOW);
      break;

      case 69:
      //digitalWrite(MOTOR_CONTROL_PIN, HIGH);
      break;

      case 70:
      lights->SetProgram(PRG_PROXIMITY);
      break;

      case 9:
      ProgramChooseRandom();
      break;
    }

    IrReceiver.resume();
  }
  #endif
}