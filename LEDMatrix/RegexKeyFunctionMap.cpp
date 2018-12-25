#include"RegexKeyFunctionMap.h"
#include<regex>

void RegexKeyFunctionMap::exec(std::string plain) {
  // std::regex match_regex("(Some)(.*)");
  // if(std::regex_match(s, match_regex)) {
  //     std::cout << "matched!";
  // }
  for(auto it=regexmap.begin(); it!=regexmap.end(); it++) {
    std::smatch matches;
    std::regex re(it->first);
    if(std::regex_search(plain, matches, re)) {
      it->second(matches);
    }
  };
};

void RegexKeyFunctionMap::map(std::string re, VoidFuncPointer f) {
  regexmap[re] = f;
};
