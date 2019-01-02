#include"RegexKeyFunctionMap.h"
#include "Regexp.h"
#include <vector>

std::function<void(std::vector<String>)> operating_function;

std::string ArduinoStringTostdString(String str) {
  std::string result = "";
  for( char c : str ) result += c ;
  return result;
}

String StdStringToArduinoString(std::string str) {
  String result = "";
  for( char c : str ) result += c ;
  return result;
}

void RegexKeyFunctionMap::exec(String plain) {

  // std::regex match_regex("(Some)(.*)");
  // if(std::regex_match(s, match_regex)) {
  //     std::cout << "matched!";
  // }
  // char buff[128] = plain;
  char *buff=&plain[0];

  MatchState ms (buff);
  for(auto it=regexmap.begin(); it!=regexmap.end(); it++) {
    operating_function = it->second;
    Serial.println (plain+' '+it->first);
    int count = ms.GlobalMatch ((it->first).c_str(), [](const char * match,          // matching string (not null-terminated)
      const unsigned int length,   // length of matching string
      const MatchState & ms) {
        std::vector<String> result_vector;
        result_vector.push_back(String(ms.src));
        char cap [128];
        for (int i = 0; i < ms.level; i++)
         {
           ms.GetCapture (cap, i);
           result_vector.push_back(String(cap));
         }  // end of for each capture
         operating_function(result_vector);
    });
    Serial.println(count);
    // std::smatch matches;
    // std::regex re(it->first);
    // if(std::regex_search(plain, matches, re)) {
    //   it->second(matches);
    // }
    operating_function = NULL;
  };
};

void RegexKeyFunctionMap::map(String re, std::function<void(std::vector<String>)> f) {
  regexmap[re] = f;
};
