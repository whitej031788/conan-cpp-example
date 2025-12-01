#include <cxxopts.hpp>
#include <spdlog/spdlog.h>
#include <fmt/format.h>

#include "http_client.h"
#include "sql_runner.h"
#include "json_utils.h"
#include "cpp_example/version.h"

#include <iostream>
#include <string>

using cpp_example::HttpClient;
using cpp_example::JsonUtils;
using cpp_example::SqlRunner;

int main(int argc, char** argv) {
	try {
		cxxopts::Options options("conan_cpp_example", "Sample C++ app with many Conan dependencies");
		options.add_options()
			("u,url", "HTTP URL to fetch", cxxopts::value<std::string>())
			("s,sql", "Unsafe SQL to execute against in-memory SQLite", cxxopts::value<std::string>())
			("j,json", "JSON string to pretty-print", cxxopts::value<std::string>())
			("l,log-level", "Logging level (trace, debug, info, warn, err)", cxxopts::value<std::string>()->default_value("info"))
			("v,version", "Print version and exit")
			("h,help", "Print usage");

		const auto result = options.parse(argc, argv);

		if (result.count("help")) {
			std::cout << options.help() << "\n";
			return 0;
		}
		if (result.count("version")) {
			std::cout << "conan_cpp_example version " << cpp_example::kVersion << "\n";
			return 0;
		}

		const auto levelStr = result["log-level"].as<std::string>();
		if (levelStr == "trace") spdlog::set_level(spdlog::level::trace);
		else if (levelStr == "debug") spdlog::set_level(spdlog::level::debug);
		else if (levelStr == "info") spdlog::set_level(spdlog::level::info);
		else if (levelStr == "warn") spdlog::set_level(spdlog::level::warn);
		else if (levelStr == "err") spdlog::set_level(spdlog::level::err);
		else spdlog::set_level(spdlog::level::info);

		if (result.count("url")) {
			const auto url = result["url"].as<std::string>();
			long statusCode = 0;
			std::string effectiveUrl;
			const auto body = HttpClient::fetchUrl(url, statusCode, effectiveUrl);
			std::cout << fmt::format("Status: {}\nEffective-URL: {}\nBody (first 400 bytes):\n{}\n",
			                         statusCode, effectiveUrl, body.substr(0, 400));
		}

		if (result.count("sql")) {
			const auto sql = result["sql"].as<std::string>();
			const auto output = SqlRunner::executeUnsafeInMemory(sql);
			std::cout << "SQL Output:\n" << output << "\n";
		}

		if (result.count("json")) {
			const auto jsonStr = result["json"].as<std::string>();
			const auto pretty = JsonUtils::prettyPrint(jsonStr, 2);
			std::cout << "JSON Output:\n" << pretty << "\n";
		}

		if (!result.count("url") && !result.count("sql") && !result.count("json")) {
			std::cout << "No operation specified. Try --help for usage.\n";
		}

		return 0;
	} catch (const cxxopts::exceptions::exception& ex) {
		std::cerr << "Argument error: " << ex.what() << "\n";
		return 2;
	} catch (const std::exception& ex) {
		std::cerr << "Unhandled error: " << ex.what() << "\n";
		return 1;
	}
}


