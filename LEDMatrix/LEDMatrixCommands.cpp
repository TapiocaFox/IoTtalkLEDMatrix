#include "LEDMatrixCommands.h"
#include <Arduino.h>

int loopMode = 0;
long LEDcycleTimestamp = millis();
int LEDInterval = 1000;
// 0 scrollTextLeft;
// 1 scrollTextRight;

void usblog(String str) {
  Serial.println(str);
}

void setupCommandRouter(RegexKeyFunctionMap &Router, LedMatrix &ledMatrix) {

    /// 大家要在這裡完成各種功能！！！
    /// *************** setup command here!!!! ***************
    // exaple:
    // Router.map(正則表達式, [](std::vector<std::string> matches) {
    //   matches[0]; //  整格字串
    //   matches[1]; // 第一組 re groupd
    //   matches[2]; // 第二組 re group, and so on
    // });

    // !bounce "This Text Should be bouncing"
    Router.map("^[!](setmode|mode)[ ](.*)$", [](std::vector<String> matches) {
      // c_str for Arduino string compatability
      usblog("mode set: "+matches[2]);
//      if(matches[2]=="normal"||matches[2]=="scrollTextLeft"||matches[2]=="scrollLeft") {
//
//      }
//      else if() {
//
//      }
      // return 0;
    });

    // !bounce "This Text Should be bouncing"
    Router.map("^[!](bounce|bouncetext)[ ](.*)$", [](std::vector<String> matches) {
      // c_str for Arduino string compatability
      usblog("Bouncing text: "+matches[2]);
      // return 0;
    });

    Router.map("^[^!](.*)$", [&ledMatrix](std::vector<String> matches) {
      usblog("Plain text: "+matches[0]);
      ledMatrix.setText(matches[0]);
    });

    Router.map("^(.*)[.](.*)$", [](std::vector<String> matches) {
      usblog("Dot text(full): "+matches[0]);
      usblog("Dot text(int): "+matches[1]);
      usblog("Dot text(float): "+matches[2]);

      // return 0;
    });

    /// *************** setup command end ********************
}

void loopLED(LedMatrix &ledMatrix) {
  if(loopMode == 0) {
    if (millis() - LEDcycleTimestamp > LEDInterval) {
        ledMatrix.clear();
        ledMatrix.scrollTextLeft();
        ledMatrix.drawText();
        ledMatrix.commit(); // commit transfers the byte buffer to the displays
    };
  }
  else if(loopMode == 1) {
    if (millis() - LEDcycleTimestamp > LEDInterval) {
        ledMatrix.clear();
        ledMatrix.scrollTextRight();
        ledMatrix.drawText();
        ledMatrix.commit(); // commit transfers the byte buffer to the displays
        LEDcycleTimestamp = millis();
    };
  }
}
