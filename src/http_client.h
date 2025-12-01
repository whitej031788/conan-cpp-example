#pragma once

#include <string>

namespace cpp_example {

class HttpClient final {
public:
	static std::string fetchUrl(const std::string& url, long& statusCode, std::string& effectiveUrl);
};

}  // namespace cpp_example


