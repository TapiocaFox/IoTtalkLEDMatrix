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
    ledMatrix.setIntensity(2); // range is 0-15
    ledMatrix.setText("MAX7219 LEDMatrix");
    setupCommandRouter(Router, ledMatrix);

    String DFs[] = {"LEDMatrixOutput"};
    IoTtalk.setDeviceModelName("LEDMatrix");
    IoTtalk.setDeviceFeatures(DFs, 1);
    IoTtalk.initialize();
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
