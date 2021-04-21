#include "class.h"

namespace JVM {

ostream& operator<<(ostream& os, const Class& main) {
  os << "Minor Version: " << main.minor_version << std::endl;
  os << "Major Version: " << main.major_version << std::endl;
  os << "Constant Pool: " << std::endl;
  for (int index = 1; index < main.constant_pool.size(); ++index) {
    os << index << ':' << '\t';
    switch (main.constant_pool[index]->tag) {
      case Constant::Type::Unicode: {
        const auto& constant =
            static_cast<const UnicodeConstant&>(*main.constant_pool[index]);
        os << '"' << constant.bytes << '"';
        break;
      }
      case Constant::Type::Integer: {
        const auto& constant =
            static_cast<const IntegerConstant&>(*main.constant_pool[index]);
        os << constant.bytes << 'i';
        break;
      }
      case Constant::Type::Float: {
        const auto& constant =
            static_cast<const FloatConstant&>(*main.constant_pool[index]);
        os << constant.bytes << 'f';
        break;
      }
      case Constant::Type::Long: {
        const auto& constant =
            static_cast<const LongConstant&>(*main.constant_pool[index]);
        os << constant.high_bytes << constant.low_bytes << 'L';
        break;
      }
      case Constant::Type::Double: {
        const auto& constant =
            static_cast<const DoubleConstant&>(*main.constant_pool[index]);
        os << constant.high_bytes << constant.low_bytes << 'd';
        break;
      }
      case Constant::Type::Class: {
        const auto constant =
            static_pointer_cast<ClassConstant>(main.constant_pool[index]);
        os << "Class   " << '\t' << '#' << constant->name_index;
        break;
      }
      case Constant::Type::String: {
        const auto constant =
            static_pointer_cast<StringConstant>(main.constant_pool[index]);
        os << "String  " << '\t' << '#' << constant->string_index;
        break;
      }
      case Constant::Type::Field: {
        const auto constant =
            static_pointer_cast<FieldConstant>(main.constant_pool[index]);
        os << "Field   " << '\t' << '#' << constant->class_index << '\t' << '#'
           << constant->variable_index;
        break;
      }
      case Constant::Type::Method: {
        const auto constant =
            static_pointer_cast<MethodConstant>(main.constant_pool[index]);
        os << "Method  " << '\t' << '#' << constant->class_index << '\t' << '#'
           << constant->variable_index;
        break;
      }
      case Constant::Type::InterfaceMethod: {
        const auto constant = static_pointer_cast<InterfaceMethodConstant>(
            main.constant_pool[index]);
        os << "InterfaceMethod" << '\t' << '#' << constant->class_index << '\t'
           << '#' << constant->variable_index;
        break;
      }
      case Constant::Type::Variable: {
        const auto constant =
            static_pointer_cast<VariableConstant>(main.constant_pool[index]);
        os << "Variable" << '\t' << '#' << constant->name_index << '\t' << '#'
           << constant->type_index;
        break;
      }
      default:
        break;
    }
    os << std::endl;
  }
  os << "Access Flags: " << main.access_flags << std::endl;
  os << "Class: " << '#' << main.this_class_index << std::endl;
  os << "Superclass: " << '#' << main.super_class_index << std::endl;
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

}  // namespace JVM
