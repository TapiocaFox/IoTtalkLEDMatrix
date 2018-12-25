#include <vector>
#include <Arduino.h>
#include "IoTtalkDevice.h"
#include "Regexp.h"
#include "RegexKeyFunctionMap.h"

long cycleTimestamp = millis();
RegexKeyFunctionMap Router;
IoTtalkDevice IoTtalk;

void usblog(String str) {
  Serial.println(str);
}

void setup() {
    Serial.begin(115200);

    /// 大家要在這裡完成各種功能！！！
    /// *************** setup command here!!!! ***************
    // exaple:
    // Router.map(正則表達式, [](std::vector<std::string> matches) {
    //   matches[0]; //  整格字串
    //   matches[1]; // 第一組 re group
    //   matches[2]; // 第二組 re group, and so on
    // });

    // !bounce "This Text Should be bouncing"
    Router.map("^[!](bounce|bouncetext)[ ](.*)$", [](std::vector<String> matches) {
      // c_str for Arduino string compatability
      usblog("Bouncing text: "+matches[2]);
      // return 0;
    });

    Router.map("^[^!](.*)$", [](std::vector<String> matches) {
      usblog("Plain text: "+matches[0]);
      // return 0;
    });

    Router.map("^(.*)[.](.*)$", [](std::vector<String> matches) {
      usblog("Dot text(full): "+matches[0]);
      usblog("Dot text(int): "+matches[1]);
      usblog("Dot text(float): "+matches[2]);

      // return 0;
    });

    /// *************** setup command end ********************


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
