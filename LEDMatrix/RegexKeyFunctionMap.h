#ifndef RegexKeyFunctionMap_H_
#define RegexKeyFunctionMap_H_

#include <vector>
#include <map>
#include "Regexp.h"
#include <Arduino.h>

typedef void (*VoidFuncPointer)(std::vector<String>);

class RegexKeyFunctionMap {
  public:
    void exec(String);
    void map(String, VoidFuncPointer);
    // void match_callback(const char * match,          // matching string (not null-terminated)
    //                   const unsigned int length,   // length of matching string
    //                   const MatchState & ms);      // MatchState in use (to get captures)
  protected:

    std::map<String, VoidFuncPointer> regexmap;

};

#endif
