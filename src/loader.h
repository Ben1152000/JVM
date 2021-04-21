#pragma once

#include "class.h"
#include "parser.h"
#include "utilities.h"

namespace JVM {

class ClassLoader {
 public:
  static Class LoadClass(const string& source) {
    auto main = Parser::ParseClass(*make_unique<string>(source));
    Parser::TraceConstants(main);
    return main;
  }
};

}  // namespace JVM
