#pragma once

#include "class.h"
#include "utilities.h"

namespace JVM {

struct InvalidFormatError : public std::runtime_error {
  InvalidFormatError(const string& message)
      : runtime_error("Format Error: " + message) {}
};

class Parser {
 public:
  static uint8_t ParseByte(string& source);

  static uint16_t ParseShort(string& source);

  static uint32_t ParseInteger(string& source);

  static uint64_t ParseLong(string& source);

  static string ParseString(string& source, size_t length);

  static shared_ptr<Attribute> ParseAttribute(string& source);

  static shared_ptr<Constant> ParseConstant(string& source);

  static Field ParseField(string& source);

  static Method ParseMethod(string& source);

  // Parse the relevant class data from the class file source. Format checking
  // is also performed to ensure that the class file is properly formed.
  static Class ParseClass(string& source);

  static void TraceConstants(Class& main);

  static shared_ptr<Attribute> InterpretAttribute(
      shared_ptr<Attribute> original, const vector<shared_ptr<Constant>>& pool);
};

}  // namespace JVM
