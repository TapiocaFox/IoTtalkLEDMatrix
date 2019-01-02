#include "LEDMatrixCommands.h"
#include <Arduino.h>

int loopMode = 0;
long LEDcycleTimestamp = millis();
int LEDInterval = 500;
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
    Router.map("^!mode[ ](.*)$", [](std::vector<String> matches) {
      // c_str for Arduino string compatability
      usblog("mode set: "+matches[1]);
     if(matches[1]=="normal"||matches[1]=="scrollTextLeft"||matches[1]=="scrollLeft") {
       loopMode =0;
     }
     else if(matches[1]=="scrollTextRight"||matches[1]=="scrollRight") {
       loopMode=1;
     }
      // return 0;
    });

    Router.map("^!interval (.*)$", [](std::vector<String> matches) {
      // c_str for Arduino string compatability
      usblog("interval set: "+matches[1]);
      LEDInterval = matches[1].toInt();
      // return 0;
    });

    Router.map("^[^!](.*)$", [&ledMatrix](std::vector<String> matches) {
      usblog("Plain text: "+matches[0]);
      ledMatrix.setText(matches[0]);
    });

    // Router.map("^(.*)[.](.*)$", [](std::vector<String> matches) {
    //   usblog("Dot text(full): "+matches[0]);
    //   usblog("Dot text(int): "+matches[1]);
    //   usblog("Dot text(float): "+matches[2]);
    //
    //   // return 0;
    // });

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
    };
  }
  LEDcycleTimestamp = millis();
}
