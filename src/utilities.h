#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

namespace JVM {

using std::make_unique;
using std::ostream;
using std::pair;
using std::shared_ptr;
using std::stack;
using std::string;
using std::unique_ptr;
template<class T> using u_ptr = unique_ptr<T>;
using std::vector;

} // namespace JVM