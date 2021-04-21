#include "parser.h"

namespace JVM {

uint8_t Parser::ParseByte(string& source) {
  if (source.empty())
    throw InvalidFormatError("The class file must not be truncated.");
  uint8_t byte = static_cast<uint8_t>(source.front());
  source.erase(0, 1);
  return byte;
}

uint16_t Parser::ParseShort(string& source) {
  return static_cast<uint16_t>(ParseByte(source)) << 8 | ParseByte(source);
}

uint32_t Parser::ParseInteger(string& source) {
  return static_cast<uint32_t>(ParseShort(source)) << 16 | ParseShort(source);
}

uint64_t Parser::ParseLong(string& source) {
  return static_cast<uint64_t>(ParseInteger(source)) << 32 |
         ParseInteger(source);
}

string Parser::ParseString(string& source, size_t length) {
  string result;
  for (int index = 0; index < length; ++index) result += ParseByte(source);
  return result;
}

shared_ptr<Attribute> Parser::ParseAttribute(string& source) {
  auto attribute = make_shared<Attribute>();
  attribute->name_index = ParseShort(source);
  uint32_t attribute_size = ParseInteger(source);
  for (uint32_t index = 0; index < attribute_size; ++index) {
    attribute->bytes.push_back(ParseByte(source));
  }
  return attribute;
}

shared_ptr<Constant> Parser::ParseConstant(string& source) {
  Constant::Type tag = static_cast<Constant::Type>(ParseByte(source));
  switch (tag) {
    case Constant::Type::Unicode: {
      auto constant = make_shared<UnicodeConstant>();
      constant->tag = tag;
      constant->bytes = ParseString(source, ParseShort(source));
      return std::move(constant);
    }
    case Constant::Type::Integer: {
      auto constant = make_shared<IntegerConstant>();
      constant->tag = tag;
      constant->bytes = ParseInteger(source);
      return std::move(constant);
    }
    case Constant::Type::Float: {
      auto constant = make_shared<FloatConstant>();
      constant->tag = tag;
      constant->bytes = ParseInteger(source);
      return std::move(constant);
    }
    case Constant::Type::Long: {
      auto constant = make_shared<LongConstant>();
      constant->tag = tag;
      constant->high_bytes = ParseInteger(source);
      constant->low_bytes = ParseInteger(source);
      return std::move(constant);
    }
    case Constant::Type::Double: {
      auto constant = make_shared<DoubleConstant>();
      constant->tag = tag;
      constant->high_bytes = ParseInteger(source);
      constant->low_bytes = ParseInteger(source);
      return std::move(constant);
    }
    case Constant::Type::Class: {
      auto constant = make_shared<ClassConstant>();
      constant->tag = tag;
      constant->name_index = ParseShort(source);
      return std::move(constant);
    }
    case Constant::Type::String: {
      auto constant = make_shared<StringConstant>();
      constant->tag = tag;
      constant->string_index = ParseShort(source);
      return std::move(constant);
    }
    case Constant::Type::Field: {
      auto constant = make_shared<FieldConstant>();
      constant->tag = tag;
      constant->class_index = ParseShort(source);
      constant->variable_index = ParseShort(source);
      return std::move(constant);
    }
    case Constant::Type::Method: {
      auto constant = make_shared<MethodConstant>();
      constant->tag = tag;
      constant->class_index = ParseShort(source);
      constant->variable_index = ParseShort(source);
      return std::move(constant);
    }
    case Constant::Type::InterfaceMethod: {
      auto constant = make_shared<InterfaceMethodConstant>();
      constant->tag = tag;
      constant->class_index = ParseShort(source);
      constant->variable_index = ParseShort(source);
      return std::move(constant);
    }
    case Constant::Type::Variable: {
      auto constant = make_shared<VariableConstant>();
      constant->tag = tag;
      constant->name_index = ParseShort(source);
      constant->type_index = ParseShort(source);
      return std::move(constant);
    }
    case Constant::Type::MethodHandle: {
      auto constant = make_shared<MethodHandleConstant>();
      constant->tag = tag;
      constant->reference_kind = ParseByte(source);
      constant->reference_index = ParseShort(source);
      return std::move(constant);
    }
    case Constant::Type::MethodType: {
      auto constant = make_shared<MethodTypeConstant>();
      constant->tag = tag;
      constant->type_index = ParseShort(source);
      return std::move(constant);
    }
    case Constant::Type::InvokeDynamic: {
      auto constant = make_shared<InvokeDynamicConstant>();
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

Field Parser::ParseField(string& source) {
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

Method Parser::ParseMethod(string& source) {
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

Class Parser::ParseClass(string& source) {
  Class main_class;
  uint32_t magic_number = ParseInteger(source);
  if (magic_number != 0xcafebabe)
    throw InvalidFormatError("The first four bytes must contain 0xCAFEBABE.");
  main_class.minor_version = ParseShort(source);
  main_class.major_version = ParseShort(source);
  uint16_t constant_pool_size = ParseShort(source);
  auto constant = make_shared<Constant>();
  constant->tag = Constant::Type::Empty;
  main_class.constant_pool.push_back(std::move(constant));
  for (uint16_t index = 1; index < constant_pool_size; ++index) {
    main_class.constant_pool.push_back(ParseConstant(source));
  }
  main_class.access_flags = ParseShort(source);
  main_class.this_class_index = ParseShort(source);
  main_class.super_class_index = ParseShort(source);
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

void Parser::TraceConstants(Class& main) {
  const auto& pool = main.constant_pool;
  for (int index = 1; index < pool.size(); ++index) {
    switch (pool[index]->tag) {
      case Constant::Type::Class: {
        auto constant = static_pointer_cast<ClassConstant>(pool[index]);
        constant->name =
            dynamic_pointer_cast<UnicodeConstant>(pool[constant->name_index]);
        if (!constant->name)
          throw InvalidFormatError("Invalid constant pool entry.");
        break;
      }
      case Constant::Type::String: {
        auto constant = static_pointer_cast<StringConstant>(pool[index]);
        constant->string =
            dynamic_pointer_cast<UnicodeConstant>(pool[constant->string_index]);
        if (!constant->string)
          throw InvalidFormatError("Invalid constant pool entry.");
        break;
      }
      case Constant::Type::Field: {
        auto constant = static_pointer_cast<FieldConstant>(pool[index]);
        constant->class_ =
            dynamic_pointer_cast<ClassConstant>(pool[constant->class_index]);
        constant->variable = dynamic_pointer_cast<VariableConstant>(
            pool[constant->variable_index]);
        if (!constant->class_ || !constant->variable)
          throw InvalidFormatError("Invalid constant pool entry.");
        break;
      }
      case Constant::Type::Method: {
        auto constant = static_pointer_cast<MethodConstant>(pool[index]);
        constant->class_ =
            dynamic_pointer_cast<ClassConstant>(pool[constant->class_index]);
        constant->variable = dynamic_pointer_cast<VariableConstant>(
            pool[constant->variable_index]);
        if (!constant->class_ || !constant->variable)
          throw InvalidFormatError("Invalid constant pool entry.");
        break;
      }
      case Constant::Type::InterfaceMethod: {
        auto constant =
            static_pointer_cast<InterfaceMethodConstant>(pool[index]);
        constant->class_ =
            dynamic_pointer_cast<ClassConstant>(pool[constant->class_index]);
        constant->variable = dynamic_pointer_cast<VariableConstant>(
            pool[constant->variable_index]);
        if (!constant->class_ || !constant->variable)
          throw InvalidFormatError("Invalid constant pool entry.");
        break;
      }
      case Constant::Type::Variable: {
        auto constant = static_pointer_cast<VariableConstant>(pool[index]);
        constant->name =
            dynamic_pointer_cast<UnicodeConstant>(pool[constant->name_index]);
        constant->type =
            dynamic_pointer_cast<UnicodeConstant>(pool[constant->type_index]);
        if (!constant->name || !constant->type)
          throw InvalidFormatError("Invalid constant pool entry.");
        break;
      }
      case Constant::Type::MethodType: {
        auto constant = static_pointer_cast<MethodTypeConstant>(pool[index]);
        constant->type =
            dynamic_pointer_cast<UnicodeConstant>(pool[constant->type_index]);
        if (!constant->type)
          throw InvalidFormatError("Invalid constant pool entry.");
        break;
      }
      default:
        break;
    }
  }
  main.this_class =
      dynamic_pointer_cast<ClassConstant>(pool[main.this_class_index]);
  if (!main.this_class)
    throw InvalidFormatError("Invalid constant pool entry.");
  main.super_class =
      dynamic_pointer_cast<ClassConstant>(pool[main.super_class_index]);
  if (!main.super_class)
    throw InvalidFormatError("Invalid constant pool entry.");
  for (Field& field : main.fields) {
    field.name = dynamic_pointer_cast<UnicodeConstant>(pool[field.name_index]);
    if (!field.name) throw InvalidFormatError("Invalid constant pool entry.");
    field.type = dynamic_pointer_cast<UnicodeConstant>(pool[field.type_index]);
    if (!field.type) throw InvalidFormatError("Invalid constant pool entry.");
    for (shared_ptr<Attribute> attribute : field.attributes) {
      attribute = Parser::InterpretAttribute(attribute, pool);
    }
  }
  for (Method& method : main.methods) {
    method.name =
        dynamic_pointer_cast<UnicodeConstant>(pool[method.name_index]);
    if (!method.name) throw InvalidFormatError("Invalid constant pool entry.");
    method.type =
        dynamic_pointer_cast<UnicodeConstant>(pool[method.type_index]);
    if (!method.type) throw InvalidFormatError("Invalid constant pool entry.");
    for (shared_ptr<Attribute> attribute : method.attributes) {
      attribute = Parser::InterpretAttribute(attribute, pool);
    }
  }
  for (shared_ptr<Attribute> attribute : main.attributes) {
    attribute = Parser::InterpretAttribute(attribute, pool);
  }
}

shared_ptr<Attribute> Parser::InterpretAttribute(
    shared_ptr<Attribute> original, const vector<shared_ptr<Constant>>& pool) {
  uint16_t name_index = original->name_index;
  auto name = dynamic_pointer_cast<UnicodeConstant>(pool[name_index]);
  string source(original->bytes.begin(), original->bytes.end());
  if (name->bytes == "Code") {
    auto attribute = make_shared<CodeAttribute>();
    attribute->name_index = name_index;
    attribute->name = name;
    return attribute;
  }
  if (name->bytes == "LineNumberTable") {
    auto attribute = make_shared<LineNumberTableAttribute>();
    attribute->name_index = name_index;
    attribute->name = name;
    return attribute;
  }
  if (name->bytes == "SourceFile") {
    auto attribute = make_shared<SourceFileAttribute>();
    attribute->name_index = name_index;
    attribute->name = name;
    attribute->file_index = ParseShort(source);
    attribute->file =
        dynamic_pointer_cast<UnicodeConstant>(pool[attribute->file_index]);
    return attribute;
  }
  std::cout << name->bytes << std::endl;
  throw InvalidFormatError("Unsupported attribute.");
}

}  // namespace JVM
