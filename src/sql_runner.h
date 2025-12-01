#pragma once

#include <string>

namespace cpp_example {

class SqlRunner final {
public:
	// Intentionally unsafe execution for injection testing purposes.
	// Executes SQL and returns a textual table of results or error message.
	static std::string executeUnsafeInMemory(const std::string& sql);
};

}  // namespace cpp_example


