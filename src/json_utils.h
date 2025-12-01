#pragma once

#include <string>

namespace cpp_example {

class JsonUtils final {
public:
	static std::string prettyPrint(const std::string& jsonInput, int indent = 2);
};

}  // namespace cpp_example


