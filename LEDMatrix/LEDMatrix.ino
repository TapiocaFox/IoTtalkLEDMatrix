#include <vector>
#include <Arduino.h>

#include "IoTtalkDevice.h"
#include "RegexKeyFunctionMap.h"
#include "LEDMatrixCommands.h"


long cycleTimestamp = millis();
RegexKeyFunctionMap Router;
IoTtalkDevice IoTtalk;

void setup() {
    Serial.begin(115200);
    setupCommandRouter(Router);

    String DFs[] = {"LEDMatrixOutput"};
    IoTtalk.setDeviceModelName("LEDMatrix");
    IoTtalk.setDeviceFeatures(DFs, 1);

    pinMode(2, OUTPUT);// D4 : on board led
    digitalWrite(2,HIGH);
    pinMode(0, INPUT_PULLUP); // D3, GPIO0: clear eeprom button

    pinMode(16, OUTPUT);// D0~
    pinMode(5, OUTPUT); // D1~
    pinMode(4, OUTPUT); // D2~
    pinMode(14, OUTPUT);// D5
    pinMode(12, OUTPUT);// D6
    pinMode(13, OUTPUT);// D7
    pinMode(15, OUTPUT);// D8

    IoTtalk.initialize();

    digitalWrite(16,LOW);
    digitalWrite(5,LOW);
    digitalWrite(4,LOW);
    digitalWrite(14,LOW);
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    digitalWrite(15,LOW);
}

void loop() {
  String result;
  IoTtalk.loop();
  if (millis() - cycleTimestamp > 200) {
    result = IoTtalk.pull("LEDMatrixOutput");
    if (result != "___NULL_DATA___"){
        Serial.println ("LEDMatrixOutput: "+result);
        Router.exec(result);
    }
  }
}
