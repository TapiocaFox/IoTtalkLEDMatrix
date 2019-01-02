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

    ledMatrix.init();
    ledMatrix.setIntensity(4); // range is 0-15
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

  if (millis() - cycleTimestamp > 300) {
    result = IoTtalk.pull("LEDMatrixOutput");
    if (result != "___NULL_DATA___"){
        Serial.println ("LEDMatrixOutput: "+result);
        if(result[0]=='\"') {
            result=result.substring(1, result.length()-1);
        }
        Router.exec(result);
    }
    cycleTimestamp = millis();
  };
};
