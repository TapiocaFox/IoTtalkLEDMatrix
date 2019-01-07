#include <vector>
#include <Arduino.h>

#include "IoTtalkDevice.h"
#include "RegexKeyFunctionMap.h"
#include "LEDMatrixCommands.h"

#define NUMBER_OF_DEVICES 4
#define CS_PIN 0
LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);
long cycleTimestamp = millis();
RegexKeyFunctionMap Router;
IoTtalkDevice IoTtalk;

void setup() {
    Serial.begin(115200);

    pinMode(16, OUTPUT);// D0~
    pinMode(5, OUTPUT); // D1~
    pinMode(4, OUTPUT); // D2~

    ledMatrix.init();
    ledMatrix.setIntensity(0); // range is 0-15
    setupCommandRouter(Router, ledMatrix);

    onWIFIFailed([&ledMatrix]() {
      digitalWrite(16, LOW);// Extra LED
      digitalWrite(4, HIGH);
      ledMatrix.setText("WIFI");
      ledMatrix.clear();
      ledMatrix.setTextAlignment(0);
      ledMatrix.drawText();
      ledMatrix.commit();
    });
    String DFs[] = {"LEDMatrixOutput"};
    IoTtalk.setDeviceModelName("LEDMatrix");
    IoTtalk.setDeviceFeatures(DFs, 1);
    IoTtalk.initialize();
    ledMatrix.setText("LEDMatrix has connected to IoTtalk.");
    ledMatrix.setTextAlignment(1);
};

void loop() {
  String result;
  IoTtalk.loop();
  loopLED(ledMatrix);

  if (millis() - cycleTimestamp > 500) {
    analogWrite(16,10);// Extra LED
    result = IoTtalk.pull("LEDMatrixOutput");
    digitalWrite(16,LOW);// Extra LED
    if (result != "___NULL_DATA___"){
        digitalWrite(5,HIGH);// Extra LED
        Serial.println ("LEDMatrixOutput: "+result);
        if(result[0]=='\"') {
            result=result.substring(1, result.length()-1);
        }
        Router.exec(result);
        digitalWrite(5,LOW);// Extra LED
    }
    cycleTimestamp = millis();
  };
};
