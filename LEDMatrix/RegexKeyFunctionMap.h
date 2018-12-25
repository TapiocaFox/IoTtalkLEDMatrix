#ifndef RegexKeyFunctionMap_H_
#define RegexKeyFunctionMap_H_

#include <vector>
#include <map>
#include "Regexp.h"

typedef void (*VoidFuncPointer)(std::vector<std::string>);

class RegexKeyFunctionMap {
  public:
    void exec(std::string);
    void map(std::string, VoidFuncPointer);
    // void match_callback(const char * match,          // matching string (not null-terminated)
    //                   const unsigned int length,   // length of matching string
    //                   const MatchState & ms);      // MatchState in use (to get captures)
  protected:

    std::map<std::string, VoidFuncPointer> regexmap;

};

#endif
