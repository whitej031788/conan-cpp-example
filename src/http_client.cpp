#include "http_client.h"

#include <cpr/cpr.h>
#include <spdlog/spdlog.h>

namespace cpp_example {

std::string HttpClient::fetchUrl(const std::string& url, long& statusCode, std::string& effectiveUrl) {
	spdlog::debug("Fetching URL: {}", url);
	const auto response = cpr::Get(cpr::Url{url});
	statusCode = response.status_code;
	effectiveUrl = response.url.str();
	spdlog::info("HTTP GET {} -> status {}, effective URL {}", url, statusCode, effectiveUrl);
	return response.text;
}

}  // namespace cpp_example


