#ifndef IoTtalkDevice_H_
#define IoTtalkDevice_H_

#include <regex>
#include <map>
typedef void (*VoidFuncPointer)(std::smatch);

class RegexKeyFunctionMap{
  public:
    void exec(std::string);
    void map(std::string, VoidFuncPointer);
  private:
    std::map<std::string, VoidFuncPointer> regexmap;
};

#endif
