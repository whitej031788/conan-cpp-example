#include "json_utils.h"

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <fmt/format.h>

namespace cpp_example {

std::string JsonUtils::prettyPrint(const std::string& jsonInput, int indent) {
	try {
		const auto parsed = nlohmann::json::parse(jsonInput);
		return parsed.dump(indent);
	} catch (const std::exception& ex) {
		spdlog::error("Failed to parse JSON: {}", ex.what());
		return fmt::format("JSON parse error: {}", ex.what());
	}
}

}  // namespace cpp_example


