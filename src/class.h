#pragma once

#include "utilities.h"

namespace JVM {

struct InvalidFormatError : public std::runtime_error {
  InvalidFormatError(const string& message)
      : runtime_error("Format Error: " + message) {}
};

struct Attribute {
  uint16_t name_index;
  vector<uint8_t> bytes;
};

struct Constant {
  uint8_t tag;
};

struct UnicodeConstant : public Constant {
  string bytes;
};

struct IntegerConstant : public Constant {
  uint32_t bytes;
};

struct FloatConstant : public Constant {
  uint32_t bytes;
};

struct LongConstant : public Constant {
  uint64_t bytes;
};

struct DoubleConstant : public Constant {
  uint64_t bytes;
};

struct ClassConstant : public Constant {
  uint16_t name_index;
};

struct StringConstant : public Constant {
  uint16_t string_index;
};

struct FieldConstant : public Constant {
  uint16_t class_index;
  uint16_t variable_index;
};

struct MethodConstant : public Constant {
  uint16_t class_index;
  uint16_t variable_index;
};

struct InterfaceMethodConstant : public Constant {
  uint16_t class_index;
  uint16_t variable_index;
};

struct VariableConstant : public Constant {
  uint16_t name_index;
  uint16_t type_index;
};

struct MethodHandleConstant : public Constant {
  uint8_t reference_kind;
  uint16_t reference_index;
};

struct MethodTypeConstant : public Constant {
  uint16_t type_index;
};

struct InvokeDynamicConstant : public Constant {
  uint16_t method_attr_index;
  uint16_t variable_index;
};

struct Field {
  uint16_t access_flags;
  uint16_t name_index;
  uint16_t type_index;
  vector<Attribute> attributes;
};

struct Method {
  uint16_t access_flags;
  uint16_t name_index;
  uint16_t type_index;
  vector<Attribute> attributes;
};

struct Class {
  uint16_t minor_version;
  uint16_t major_version;
  vector<u_ptr<Constant>> constant_pool;
  uint16_t access_flags;
  uint16_t this_class;
  uint16_t super_class;
  vector<uint16_t> interfaces;
  vector<Field> fields;
  vector<Method> methods;
  vector<Attribute> attributes;
};

class Classfile {
 public:
  // Parse the relevant class data from the class file source. Format checking
  // is also performed to ensure that the class file is properly formed.
  Classfile(string source) : main_class(ParseClass(source)) {}

  friend ostream& operator<<(ostream& os, const Classfile& classfile);

 private:
  Class main_class;

  static uint8_t ParseByte(string& source);

  static uint16_t ParseShort(string& source);

  static uint32_t ParseInteger(string& source);

  static uint64_t ParseLong(string& source);

  static string ParseString(string& source, size_t length);

  static Attribute ParseAttribute(string& source);

  static u_ptr<Constant> ParseConstant(string& source);

  static Field ParseField(string& source);

  static Method ParseMethod(string& source);

  static Class ParseClass(string& source);
};

}  // namespace JVM