#include<regex>
#include<iostream>
#include"RegexKeyFunctionMap.h"
using namespace std;

void path1(std::smatch matches) {
  cout << "got it." << endl;
  cout << matches[0].str() << endl;
}

void commandStar(std::smatch matches) {
  cout << "star command:" << matches[1].str() << endl;
}

int main() {
  RegexKeyFunctionMap router;
  router.map("(haha)(.*)", path1);
  router.map("[!][s|S]tar[ ](.*)", commandStar);
  router.exec("ha2ha");
  router.exec("haha2");
  router.exec("!star haha");
}
