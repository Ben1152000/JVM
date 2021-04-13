#include "class.h"

#include <glog/logging.h>
#include <gtest/gtest.h>

#include "utilities.h"

namespace {

using namespace JVM;

TEST(BasicTest, HelloWorld) { 
    std::cout << "Hello, world" << std::endl;
}

} // namespace

int main(int argc, char **argv) {
    ::google::InitGoogleLogging(argv[0]);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
