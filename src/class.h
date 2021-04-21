#pragma once

#include "utilities.h"

namespace JVM {

struct Constant {
  enum class Type {
    Empty = 0,
    Unicode = 1,
    Integer = 3,
    Float = 4,
    Long = 5,
    Double = 6,
    Class = 7,
    String = 8,
    Field = 9,
    Method = 10,
    InterfaceMethod = 11,
    Variable = 12,
    MethodHandle = 15,
    MethodType = 16,
    InvokeDynamic = 18,
  };
  Type tag;
  virtual ~Constant() {}  // Polymorphism!
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
  uint32_t high_bytes;
  uint32_t low_bytes;
};

struct DoubleConstant : public Constant {
  uint32_t high_bytes;
  uint32_t low_bytes;
};

struct StringConstant : public Constant {
  uint16_t string_index;
  shared_ptr<UnicodeConstant> string;
};

struct ClassConstant : public Constant {
  uint16_t name_index;
  shared_ptr<UnicodeConstant> name;
};

struct VariableConstant : public Constant {
  uint16_t name_index;
  shared_ptr<UnicodeConstant> name;
  uint16_t type_index;
  shared_ptr<UnicodeConstant> type;
};

struct FieldConstant : public Constant {
  uint16_t class_index;
  shared_ptr<ClassConstant> class_;
  uint16_t variable_index;
  shared_ptr<VariableConstant> variable;
};

struct MethodConstant : public Constant {
  uint16_t class_index;
  shared_ptr<ClassConstant> class_;
  uint16_t variable_index;
  shared_ptr<VariableConstant> variable;
};

struct InterfaceMethodConstant : public Constant {
  uint16_t class_index;
  shared_ptr<ClassConstant> class_;
  uint16_t variable_index;
  shared_ptr<VariableConstant> variable;
};

struct MethodHandleConstant : public Constant {
  uint8_t reference_kind;
  uint16_t reference_index;
};

struct MethodTypeConstant : public Constant {
  uint16_t type_index;
  shared_ptr<UnicodeConstant> type;
};

struct InvokeDynamicConstant : public Constant {
  uint16_t method_attr_index;
  uint16_t variable_index;
  shared_ptr<VariableConstant> variable;
};

struct Attribute {
  uint16_t name_index;
  shared_ptr<UnicodeConstant> name;
  vector<uint8_t> bytes;
  virtual ~Attribute() {}  // Polymorphism!
};

struct Exception {
  uint16_t start_pc;
  uint16_t end_pc;
  uint16_t handler_pc;
  uint16_t catch_type;
};

struct CodeAttribute : public Attribute {
  uint16_t max_stack;
  uint16_t max_locals;
  vector<uint8_t> code;
  vector<Exception> exception_table;
  vector<Attribute> attributes;
};

struct LineNumber {
  uint16_t start_pc;
  uint16_t line_number;
};

struct LineNumberTableAttribute : public Attribute {
  vector<LineNumber> table;
};

struct SourceFileAttribute : public Attribute {
  uint16_t file_index;
  shared_ptr<UnicodeConstant> file;
};

struct Field {
  uint16_t access_flags;
  uint16_t name_index;
  shared_ptr<UnicodeConstant> name;
  uint16_t type_index;
  shared_ptr<UnicodeConstant> type;
  vector<shared_ptr<Attribute>> attributes;
};

struct Method {
  uint16_t access_flags;
  uint16_t name_index;
  shared_ptr<UnicodeConstant> name;
  uint16_t type_index;
  shared_ptr<UnicodeConstant> type;
  vector<shared_ptr<Attribute>> attributes;
};

struct Class {
  uint16_t minor_version;
  uint16_t major_version;
  vector<shared_ptr<Constant>> constant_pool;
  uint16_t access_flags;
  uint16_t this_class_index;
  shared_ptr<ClassConstant> this_class;
  uint16_t super_class_index;
  shared_ptr<ClassConstant> super_class;
  vector<uint16_t> interfaces;
  vector<Field> fields;
  vector<Method> methods;
  vector<shared_ptr<Attribute>> attributes;

 public:
  friend ostream& operator<<(ostream& os, const Class& main);
};

}  // namespace JVM
