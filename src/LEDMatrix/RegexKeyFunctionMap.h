#ifndef RegexKeyFunctionMap_H_
#define RegexKeyFunctionMap_H_

#include <vector>
#include <map>
#include "Regexp.h"
#include <Arduino.h>
// std::function<*void(std::vector<String>)>
typedef std::function<void(std::vector<String>)> test;

class RegexKeyFunctionMap {
  public:
    void exec(String);
    void map(String, std::function<void(std::vector<String>)>);
    // void match_callback(const char * match,          // matching string (not null-terminated)
    //                   const unsigned int length,   // length of matching string
    //                   const MatchState & ms);      // MatchState in use (to get captures)
  protected:

    std::map<String, std::function<void(std::vector<String>)>> regexmap;

};

#endif
