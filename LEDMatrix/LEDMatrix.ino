#include <vector>
#include <Arduino.h>
#include "IoTtalkDevice.h"
#include "Regexp.h"
#include "RegexKeyFunctionMap.h"

namespace std {

void __throw_bad_cast(void)
{
    // print error and stop execution
}
void __throw_ios_failure(char const*){}
void __throw_runtime_error(char const*){}


} // namespace std

long cycleTimestamp = millis();
RegexKeyFunctionMap StringRouter;
IoTtalkDevice IoTtalk;

std::string ArduinoStringTostdString(String str) {
  std::string result = "";
  for( char c : str ) result += c ;
  return result;
}

String StdStringToArduinoString(std::string str) {
  String result = "";
  for( char c : str ) result += c ;
  return result;
}

void usblog(String str) {
  Serial.println(str);
}

void setup() {
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

    Serial.begin(115200);

    /// 大家要在這裡完成各種功能！！！
    /// *************** setup command here!!!! ***************

    // !bounce "This Text Should be bouncing"
    StringRouter.map("^[!](bounce|bouncetext)[ ](.*)$", [](std::vector<std::string> matches) {
      // c_str for Arduino string compatability
      usblog("Bouncing text: "+StdStringToArduinoString(matches[2]));
      // return 0;
    });

    StringRouter.map("^[^!](.*)$", [](std::vector<std::string> matches) {
      usblog("Plain text: "+StdStringToArduinoString(matches[0]));
      // return 0;
    });

    StringRouter.map("^(.*)[.](.*)$", [](std::vector<std::string> matches) {
      usblog("Dot text(full): "+StdStringToArduinoString(matches[0]));
      usblog("Dot text(int): "+StdStringToArduinoString(matches[1]));
      usblog("Dot text(float): "+StdStringToArduinoString(matches[2]));

      // return 0;
    });

    /// *************** setup command end ********************

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
        StringRouter.exec(ArduinoStringTostdString(result));
    }
  }
}
