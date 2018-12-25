#include<regex>
#include<iostream>
#include"RegexKeyFunctionMap.h"
using namespace std;
//-std=c++11


int main() {
  RegexKeyFunctionMap router;
  router.map("^(haha.*)$", [](std::smatch matches) {
    cout << "haha command inself: " << matches[1].str() << endl;
    // return 0;
  });

  // !star 123
  // !starcommand hasdfasd asdfs
  router.map("^[!](star|starcommand)[ ](.*)$", [](std::smatch matches) {
    cout << "star command suffix: " << matches[2].str() << endl;
    // return 0;
  });

  router.exec("ha2ha");
  router.exec("haha2");
  router.exec("!star haha sfs");
}
