#include "utilities.h"

namespace JVM {

struct ParseError : public std::runtime_error {
  ParseError(int location = 0, const string& message = "Parse Error")
      : runtime_error(std::to_string(location) + ": " + message) {}
};

struct Constant {
  uint8_t tag;
};

struct UnicodeConstant : virtual Constant {
  string bytes;
};

struct IntegerConstant : virtual Constant {
  uint32_t bytes;
};

struct FloatConstant : virtual Constant {
  uint32_t bytes;
};

struct LongConstant : virtual Constant {
  uint64_t bytes;
};

struct DoubleConstant : virtual Constant {
  uint64_t bytes;
};

struct ClassConstant : virtual Constant {
  uint16_t name_index;
};

struct StringConstant : virtual Constant {
  uint16_t string_index;
};

struct FieldConstant : virtual Constant {
  uint16_t class_index;
  uint16_t variable_index;
};

struct MethodConstant : virtual Constant {
  uint16_t class_index;
  uint16_t variable_index;
};

struct InterfaceMethodConstant : virtual Constant {
  uint16_t class_index;
  uint16_t variable_index;
};

struct VariableConstant : virtual Constant {
  uint16_t name_index;
  uint16_t type_index;
};

struct MethodHandleConstant : virtual Constant {
  uint8_t reference_kind;
  uint16_t reference_index;
};

struct MethodTypeConstant : virtual Constant {
  uint16_t type_index;
};

struct InvokeDynamicConstant : virtual Constant {
  uint16_t method_attr_index;
  uint16_t variable_index;
};

struct Field {};

struct Method {};

struct Attribute {};

struct Class {
  uint32_t magic_number;
  uint16_t minor_version;
  uint16_t major_version;
  vector<Constant> constant_pool;
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
  Classfile(string source) : main_class(ParseClass(source)) {}

 private:
  Class main_class;

  static uint8_t ParseByte(string& source);

  static uint16_t ParseShort(string& source);

  static uint32_t ParseInteger(string& source);

  static uint64_t ParseLong(string& source);

  static string ParseString(string& source, size_t length);

  static Constant ParseConstant(string& source);

  static Class ParseClass(string& source);
};

}  // namespace JVM