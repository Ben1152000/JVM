#include "utilities.h"

namespace JVM {

class Class {
  public:

    Class(const string& source);

  private:
    // using u1 = uint8_t;
    // using u2 = uint16_t;
    // using u4 = uint32_t;

    // struct ClassFile {
    //   u4 magic;
    //   u2 minor_version;
    //   u2 major_version;
    //   u2 constant_pool_count;
    //   vector<cp_info> constant_pool;
    //   u2 access_flags;
    //   u2 this_class;
    //   u2 super_class;
    //   u2 interfaces_count;
    //   u2 interfaces[interfaces_count];
    //   u2 fields_count;
    //   field_info fields[fields_count];
    //   u2 methods_count;
    //   method_info methods[methods_count];
    //   u2 attributes_count;
    //   attribute_info attributes[attributes_count];
    // };

};

}