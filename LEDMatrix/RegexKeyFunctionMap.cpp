#include"RegexKeyFunctionMap.h"
#include "Regexp.h"
#include <vector>

VoidFuncPointer operating_function;

void RegexKeyFunctionMap::exec(std::string plain) {
  // std::regex match_regex("(Some)(.*)");
  // if(std::regex_match(s, match_regex)) {
  //     std::cout << "matched!";
  // }
  // char buff[128] = plain;
  char *buff=&plain[0];

  MatchState ms (buff);
  for(auto it=regexmap.begin(); it!=regexmap.end(); it++) {
    operating_function = it->second;
    int count = ms.GlobalMatch ((it->first).c_str(), [](const char * match,          // matching string (not null-terminated)
      const unsigned int length,   // length of matching string
      const MatchState & ms) {
        std::vector<std::string> result_vector;
        result_vector.push_back(std::string(ms.src));
        char cap [128];
        for (int i = 0; i < ms.level; i++)
         {
           ms.GetCapture (cap, i);
           result_vector.push_back(std::string(cap));
         }  // end of for each capture
         operating_function(result_vector);
    });
    // std::smatch matches;
    // std::regex re(it->first);
    // if(std::regex_search(plain, matches, re)) {
    //   it->second(matches);
    // }
    operating_function = NULL;
  };
};

void RegexKeyFunctionMap::map(std::string re, VoidFuncPointer f) {
  regexmap[re] = f;
};
