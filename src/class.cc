#include "class.h"

namespace JVM {

uint8_t Classfile::ParseByte(string& source) {
  if (source.empty()) throw ParseError();
  uint8_t byte = static_cast<uint8_t>(source.front());
  source.erase(0, 1);
  return byte;
}

uint16_t Classfile::ParseShort(string& source) {
  return static_cast<uint16_t>(ParseByte(source)) << 8 | ParseByte(source);
}

uint32_t Classfile::ParseInteger(string& source) {
  return static_cast<uint32_t>(ParseShort(source)) << 16 | ParseShort(source);
}

uint64_t Classfile::ParseLong(string& source) {
  return static_cast<uint64_t>(ParseInteger(source)) << 32 |
         ParseInteger(source);
}

string Classfile::ParseString(string& source, size_t length) {
  string result;
  for (int index = 0; index < length; ++index) result += ParseByte(source);
  return result;
}

Constant Classfile::ParseConstant(string& source) {
  uint8_t tag = ParseByte(source);
  std::cout << static_cast<int>(tag);
  switch (tag) {
    case 1: {
      auto constant = UnicodeConstant();
      constant.tag = tag;
      constant.bytes = ParseString(source, ParseShort(source));
      std::cout << " " << constant.bytes;
      return std::move(constant);
    }
    case 3: {
      auto constant = IntegerConstant();
      constant.tag = tag;
      constant.bytes = ParseInteger(source);
      return std::move(constant);
    }
    case 4: {
      auto constant = FloatConstant();
      constant.tag = tag;
      constant.bytes = ParseInteger(source);
      return std::move(constant);
    }
    case 5: {
      auto constant = LongConstant();
      constant.tag = tag;
      constant.bytes = ParseLong(source);
      return std::move(constant);
    }
    case 6: {
      auto constant = DoubleConstant();
      constant.tag = tag;
      constant.bytes = ParseLong(source);
      return std::move(constant);
    }
    case 7: {
      auto constant = ClassConstant();
      constant.tag = tag;
      constant.name_index = ParseShort(source);
      return std::move(constant);
    }
    case 8: {
      auto constant = StringConstant();
      constant.tag = tag;
      constant.string_index = ParseShort(source);
      return std::move(constant);
    }
    case 9: {
      auto constant = FieldConstant();
      constant.tag = tag;
      constant.class_index = ParseShort(source);
      constant.variable_index = ParseShort(source);
      return std::move(constant);
    }
    case 10: {
      auto constant = MethodConstant();
      constant.tag = tag;
      constant.class_index = ParseShort(source);
      constant.variable_index = ParseShort(source);
      return std::move(constant);
    }
    case 11: {
      auto constant = InterfaceMethodConstant();
      constant.tag = tag;
      constant.class_index = ParseShort(source);
      constant.variable_index = ParseShort(source);
      return std::move(constant);
    }
    case 12: {
      auto constant = VariableConstant();
      constant.tag = tag;
      constant.name_index = ParseShort(source);
      constant.type_index = ParseShort(source);
      return std::move(constant);
    }
    case 15: {
      auto constant = MethodHandleConstant();
      constant.tag = tag;
      constant.reference_kind = ParseByte(source);
      constant.reference_index = ParseShort(source);
      return std::move(constant);
    }
    case 16: {
      auto constant = MethodTypeConstant();
      constant.tag = tag;
      constant.type_index = ParseShort(source);
      return std::move(constant);
    }
    case 18: {
      auto constant = InvokeDynamicConstant();
      constant.tag = tag;
      constant.method_attr_index = ParseShort(source);
      constant.variable_index = ParseShort(source);
      return std::move(constant);
    }
    default:
      throw ParseError();
  }
}

Class Classfile::ParseClass(string& source) {
  Class main_class;
  main_class.magic_number = ParseInteger(source);
  main_class.major_version = ParseShort(source);
  main_class.minor_version = ParseShort(source);
  uint16_t constant_pool_size = ParseShort(source);
  for (uint16_t index = 1; index < constant_pool_size; ++index) {
    main_class.constant_pool.push_back(ParseConstant(source));
    std::cout << std::endl;
  }
  return main_class;
}

}  // namespace JVM