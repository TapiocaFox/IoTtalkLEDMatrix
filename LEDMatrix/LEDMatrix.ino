#include <vector>
#include <Arduino.h>

#include "IoTtalkDevice.h"
#include "RegexKeyFunctionMap.h"
#include "LEDMatrixCommands.h"

#define NUMBER_OF_DEVICES 4
#define CS_PIN 2
LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);

long cycleTimestamp = millis();
RegexKeyFunctionMap Router;
IoTtalkDevice IoTtalk;

void setup() {
    Serial.begin(115200);

    ledMatrix.init();
    ledMatrix.setIntensity(4); // range is 0-15
    ledMatrix.setText("MAX7219");
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

  if (millis() - cycleTimestamp > 1000) {
    result = IoTtalk.pull("LEDMatrixOutput");
    if (result != "___NULL_DATA___"){
        Serial.println ("LEDMatrixOutput: "+result);
        Router.exec(result);
    }
    cycleTimestamp = millis();
  };
};
