#ifndef LEDMatrixCommands_H_
#define LEDMatrixCommands_H_

#include "RegexKeyFunctionMap.h"
#include "LEDMatrix.h"

void setupCommandRouter(RegexKeyFunctionMap &Router, LedMatrix& ledMatrix);

void loopLED(LedMatrix &ledMatrix);
#endif
