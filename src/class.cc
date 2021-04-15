#include "class.h"

namespace JVM {

ostream& operator<<(ostream& os, const Class& main) {
  os << "Minor Version: " << main.minor_version << std::endl;
  os << "Major Version: " << main.major_version << std::endl;
  os << "Constant Pool: " << std::endl;
  for (int index = 1; index < main.constant_pool.size(); ++index) {
    os << index << ':' << '\t'
       << static_cast<uint32_t>(main.constant_pool[index]->tag);
    switch (main.constant_pool[index]->tag) {
      case Constant::Type::Unicode: {
        const auto& constant =
            static_cast<const UnicodeConstant&>(*main.constant_pool[index]);
        os << '\t' << '"' << constant.bytes << '"';
        break;
      }
      case Constant::Type::Integer: {
        const auto& constant =
            static_cast<const IntegerConstant&>(*main.constant_pool[index]);
        os << '\t' << constant.bytes << 'i';
        break;
      }
      case Constant::Type::Float: {
        const auto& constant =
            static_cast<const FloatConstant&>(*main.constant_pool[index]);
        os << '\t' << constant.bytes << 'f';
        break;
      }
      case Constant::Type::Long: {
        const auto& constant =
            static_cast<const LongConstant&>(*main.constant_pool[index]);
        os << '\t' << constant.bytes << 'L';
        break;
      }
      case Constant::Type::Double: {
        const auto& constant =
            static_cast<const DoubleConstant&>(*main.constant_pool[index]);
        os << '\t' << constant.bytes << 'd';
        break;
      }
      default:
        break;
    }
    os << std::endl;
  }
  os << "Access Flags: " << main.access_flags << std::endl;
  os << "Class: " << '#' << main.this_class << std::endl;
  os << "Superclass: " << '#' << main.super_class << std::endl;
  os << "Fields: " << std::endl;
  for (int index = 0; index < main.fields.size(); ++index) {
    os << index << ':';
    os << '\t' << '#' << main.fields[index].name_index;
    os << '\t' << '#' << main.fields[index].type_index;
    os << std::endl;
  }
  os << "Methods: " << std::endl;
  for (int index = 0; index < main.methods.size(); ++index) {
    os << index << ':';
    os << '\t' << '#' << main.methods[index].name_index;
    os << '\t' << '#' << main.methods[index].type_index;
    os << std::endl;
  }
  return os;
}

uint8_t ClassLoader::ParseByte(string& source) {
  if (source.empty())
    throw InvalidFormatError("The class file must not be truncated.");
  uint8_t byte = static_cast<uint8_t>(source.front());
  source.erase(0, 1);
  return byte;
}

uint16_t ClassLoader::ParseShort(string& source) {
  return static_cast<uint16_t>(ParseByte(source)) << 8 | ParseByte(source);
}

uint32_t ClassLoader::ParseInteger(string& source) {
  return static_cast<uint32_t>(ParseShort(source)) << 16 | ParseShort(source);
}

uint64_t ClassLoader::ParseLong(string& source) {
  return static_cast<uint64_t>(ParseInteger(source)) << 32 |
         ParseInteger(source);
}

string ClassLoader::ParseString(string& source, size_t length) {
  string result;
  for (int index = 0; index < length; ++index) result += ParseByte(source);
  return result;
}

Attribute ClassLoader::ParseAttribute(string& source) {
  Attribute attribute;
  attribute.name_index = ParseShort(source);
  uint32_t attribute_size = ParseInteger(source);
  for (uint32_t index = 0; index < attribute_size; ++index) {
    attribute.bytes.push_back(ParseByte(source));
  }
  return attribute;
}

u_ptr<Constant> ClassLoader::ParseConstant(string& source) {
  Constant::Type tag = static_cast<Constant::Type>(ParseByte(source));
  switch (tag) {
    case Constant::Type::Unicode: {
      auto constant = make_unique<UnicodeConstant>();
      constant->tag = tag;
      constant->bytes = ParseString(source, ParseShort(source));
      return std::move(constant);
    }
    case Constant::Type::Integer: {
      auto constant = make_unique<IntegerConstant>();
      constant->tag = tag;
      constant->bytes = ParseInteger(source);
      return std::move(constant);
    }
    case Constant::Type::Float: {
      auto constant = make_unique<FloatConstant>();
      constant->tag = tag;
      constant->bytes = ParseInteger(source);
      return std::move(constant);
    }
    case Constant::Type::Long: {
      auto constant = make_unique<LongConstant>();
      constant->tag = tag;
      constant->bytes = ParseLong(source);
      return std::move(constant);
    }
    case Constant::Type::Double: {
      auto constant = make_unique<DoubleConstant>();
      constant->tag = tag;
      constant->bytes = ParseLong(source);
      return std::move(constant);
    }
    case Constant::Type::Class: {
      auto constant = make_unique<ClassConstant>();
      constant->tag = tag;
      constant->name_index = ParseShort(source);
      return std::move(constant);
    }
    case Constant::Type::String: {
      auto constant = make_unique<StringConstant>();
      constant->tag = tag;
      constant->string_index = ParseShort(source);
      return std::move(constant);
    }
    case Constant::Type::Field: {
      auto constant = make_unique<FieldConstant>();
      constant->tag = tag;
      constant->class_index = ParseShort(source);
      constant->variable_index = ParseShort(source);
      return std::move(constant);
    }
    case Constant::Type::Method: {
      auto constant = make_unique<MethodConstant>();
      constant->tag = tag;
      constant->class_index = ParseShort(source);
      constant->variable_index = ParseShort(source);
      return std::move(constant);
    }
    case Constant::Type::InterfaceMethod: {
      auto constant = make_unique<InterfaceMethodConstant>();
      constant->tag = tag;
      constant->class_index = ParseShort(source);
      constant->variable_index = ParseShort(source);
      return std::move(constant);
    }
    case Constant::Type::Variable: {
      auto constant = make_unique<VariableConstant>();
      constant->tag = tag;
      constant->name_index = ParseShort(source);
      constant->type_index = ParseShort(source);
      return std::move(constant);
    }
    case Constant::Type::MethodHandle: {
      auto constant = make_unique<MethodHandleConstant>();
      constant->tag = tag;
      constant->reference_kind = ParseByte(source);
      constant->reference_index = ParseShort(source);
      return std::move(constant);
    }
    case Constant::Type::MethodType: {
      auto constant = make_unique<MethodTypeConstant>();
      constant->tag = tag;
      constant->type_index = ParseShort(source);
      return std::move(constant);
    }
    case Constant::Type::InvokeDynamic: {
      auto constant = make_unique<InvokeDynamicConstant>();
      constant->tag = tag;
      constant->method_attr_index = ParseShort(source);
      constant->variable_index = ParseShort(source);
      return std::move(constant);
    }
    default:
      throw InvalidFormatError(
          "The constant pool entry must have a valid tag.");
  }
}

Field ClassLoader::ParseField(string& source) {
  Field field;
  field.access_flags = ParseShort(source);
  field.name_index = ParseShort(source);
  field.type_index = ParseShort(source);
  uint16_t attributes_count = ParseShort(source);
  for (uint16_t index = 0; index < attributes_count; ++index) {
    field.attributes.push_back(ParseAttribute(source));
  }
  return field;
}

Method ClassLoader::ParseMethod(string& source) {
  Method method;
  method.access_flags = ParseShort(source);
  method.name_index = ParseShort(source);
  method.type_index = ParseShort(source);
  uint16_t attributes_count = ParseShort(source);
  for (uint16_t index = 0; index < attributes_count; ++index) {
    method.attributes.push_back(ParseAttribute(source));
  }
  return method;
}

Class ClassLoader::ParseClass(string& source) {
  Class main_class;
  uint32_t magic_number = ParseInteger(source);
  if (magic_number != 0xcafebabe)
    throw InvalidFormatError("The first four bytes must contain 0xCAFEBABE.");
  main_class.minor_version = ParseShort(source);
  main_class.major_version = ParseShort(source);
  uint16_t constant_pool_size = ParseShort(source);
  auto constant = make_unique<Constant>();
  constant->tag = Constant::Type::Empty;
  main_class.constant_pool.push_back(std::move(constant));
  for (uint16_t index = 1; index < constant_pool_size; ++index) {
    main_class.constant_pool.push_back(ParseConstant(source));
  }
  main_class.access_flags = ParseShort(source);
  main_class.this_class = ParseShort(source);
  main_class.super_class = ParseShort(source);
  uint16_t interfaces_count = ParseShort(source);
  for (uint16_t index = 0; index < interfaces_count; ++index) {
    main_class.interfaces.push_back(ParseShort(source));
  }
  uint16_t fields_count = ParseShort(source);
  for (uint16_t index = 0; index < fields_count; ++index) {
    main_class.fields.push_back(ParseField(source));
  }
  uint16_t methods_count = ParseShort(source);
  for (uint16_t index = 0; index < methods_count; ++index) {
    main_class.methods.push_back(ParseMethod(source));
  }
  uint16_t attributes_count = ParseShort(source);
  for (uint16_t index = 0; index < attributes_count; ++index) {
    main_class.attributes.push_back(ParseAttribute(source));
  }
  if (source.length() > 0)
    throw InvalidFormatError(
        "The class file must not have extra bytes at the end.");
  return main_class;
}

}  // namespace JVM