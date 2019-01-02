#include "LEDMatrixCommands.h"
#include <Arduino.h>

int Mode = 1;
long LEDcycleTimestamp = millis();
int LEDInterval = 100;
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
    Router.map("^!mode[ ](.*)$", [&ledMatrix](std::vector<String> matches) {
      // c_str for Arduino string compatability
      usblog("mode set: "+matches[1]);
     if(matches[1]=="normal"||matches[1]=="scrollTextLeft"||matches[1]=="scrollLeft") {
       ledMatrix.setTextAlignment(1);
       Mode =1;
     }
     else if(matches[1]=="scrollTextRight"||matches[1]=="scrollRight") {
       ledMatrix.setTextAlignment(3);
       Mode=2;
     }
     else if(matches[1]=="stillRight"||matches[1]=="right"||matches[1]=="Right") {
       ledMatrix.clear();
       ledMatrix.setTextAlignment(2);
       ledMatrix.drawText();
       ledMatrix.commit();
       Mode=3;
     }
     else if(matches[1]=="stillLeft"||matches[1]=="left"||matches[1]=="Left") {
       ledMatrix.clear();
       ledMatrix.setTextAlignment(0);
       ledMatrix.drawText();
       ledMatrix.commit();
       Mode=4;
     }
     else if(matches[1]=="oscillate"||matches[1]=="vibrate"||matches[1]=="shake") {
       ledMatrix.setTextAlignment(1);
       Mode=5;
     }
      // return 0;
    });

    // !bounce "This Text Should be bouncing"
    Router.map("^!intensity[ ](.*)$", [&ledMatrix](std::vector<String> matches) {
      // c_str for Arduino string compatability
      usblog("intensity set: "+matches[1]);
      ledMatrix.setIntensity(matches[1].toInt());
      // return 0;
    });

    Router.map("^!interval (.*)$", [](std::vector<String> matches) {
      // c_str for Arduino string compatability
      usblog("interval set: "+matches[1]);
      LEDInterval = matches[1].toInt();
      // return 0;
    });

    Router.map("^!charWidth (.*)$", [&ledMatrix](std::vector<String> matches) {
      // c_str for Arduino string compatability
      usblog("charWidth set: "+matches[1]);
      ledMatrix.setCharWidth(matches[1].toInt());
      // return 0;
    });

    Router.map("^[^!](.*)$", [&ledMatrix](std::vector<String> matches) {
      usblog("Plain text: "+matches[0]);
      ledMatrix.setText(matches[0]);
      if(Mode == 1) {
        ledMatrix.clear();
        ledMatrix.setTextAlignment(1);
        ledMatrix.drawText();
        ledMatrix.commit();
      }
      else if(Mode == 2) {
        ledMatrix.clear();
        ledMatrix.setTextAlignment(3);
        ledMatrix.drawText();
        ledMatrix.commit();
      }
      else if(Mode == 3) {
        ledMatrix.clear();
        ledMatrix.setTextAlignment(2);
        ledMatrix.drawText();
        ledMatrix.commit();
      }
      else if(Mode == 4) {
        ledMatrix.clear();
        ledMatrix.setTextAlignment(0);
        ledMatrix.drawText();
        ledMatrix.commit();
      }
      else if(Mode == 5) {
      }
    });

    Router.map("^!text (.*)$", [&ledMatrix](std::vector<String> matches) {
      usblog("set text: "+matches[1]);
      ledMatrix.setText(matches[1]);
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
  if(Mode == 1) {
    if (millis() - LEDcycleTimestamp > LEDInterval) {
        ledMatrix.clear();
        ledMatrix.scrollTextLeft();
        ledMatrix.drawText();
        ledMatrix.commit(); // commit transfers the byte buffer to the displays
        LEDcycleTimestamp = millis();

    };
  }
  else if(Mode == 2) {
    if (millis() - LEDcycleTimestamp > LEDInterval) {
        ledMatrix.clear();
        ledMatrix.scrollTextRight();
        ledMatrix.drawText();
        ledMatrix.commit(); // commit transfers the byte buffer to the displays
        LEDcycleTimestamp = millis();

    };
  }
  else if(Mode == 5) {
    if (millis() - LEDcycleTimestamp > LEDInterval) {
        ledMatrix.clear();
        ledMatrix.oscillateText();
        ledMatrix.drawText();
        ledMatrix.commit(); // commit transfers the byte buffer to the displays
        LEDcycleTimestamp = millis();

    };
  }
}
