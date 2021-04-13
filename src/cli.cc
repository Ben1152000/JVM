// Command line interface for the automata DSL

#include "utilities.h"
#include "class.h"

using namespace JVM;

int main(int argc, char* argv[]) {
  string binary_name = argv[0];

  // Check usage:
  if (argc != 2) {
    std::cerr << "Usage: " << binary_name << " foo.class" << std::endl;
    exit(1);
  }

  // Read from file given as argv[1]
  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Unable to open file " << argv[1] << std::endl;
    exit(1);
  }
  std::string data((std::istreambuf_iterator<char>(file)),
                  std::istreambuf_iterator<char>());

  Classfile classfile(data);
}