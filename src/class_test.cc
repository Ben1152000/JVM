#include "class.h"

#include <glog/logging.h>
#include <gtest/gtest.h>

#include "utilities.h"

namespace {

using namespace JVM;

const auto readfile = [](const string& filename) -> string {
  std::ifstream file(filename);
  EXPECT_TRUE(file.is_open());
  return string((std::istreambuf_iterator<char>(file)),
              std::istreambuf_iterator<char>());
};

TEST(ParserTests, Hello) { 
  string data = readfile("test/Hello.class");
  Classfile classfile(data);
  std::cout << classfile << std::endl;
}

TEST(ParserTests, Simple) { 
  string data = readfile("test/Simple.class");
  Classfile classfile(data);
  std::cout << classfile << std::endl;
}

} // namespace

int main(int argc, char **argv) {
    ::google::InitGoogleLogging(argv[0]);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
