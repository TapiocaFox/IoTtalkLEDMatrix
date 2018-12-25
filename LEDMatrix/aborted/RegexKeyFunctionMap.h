#ifndef RegexKeyFunctionMap_H_
#define RegexKeyFunctionMap_H_

#include <regex>
#include <map>
typedef void (*VoidFuncPointer)(std::smatch);

class RegexKeyFunctionMap {
  public:
    void exec(std::string);
    void map(std::string, VoidFuncPointer);
  protected:
    std::map<std::string, VoidFuncPointer> regexmap;
};

#endif
