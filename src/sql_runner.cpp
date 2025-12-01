#include "sql_runner.h"

#include <sqlite3.h>
#include <spdlog/spdlog.h>
#include <fmt/format.h>

#include <sstream>
#include <vector>

namespace cpp_example {

namespace {
int captureCallback(void* data, int argc, char** argv, char** colNames) {
	auto* out = static_cast<std::stringstream*>(data);
	for (int i = 0; i < argc; ++i) {
		*out << (i == 0 ? "" : " | ") << (colNames[i] ? colNames[i] : "") << "=" << (argv[i] ? argv[i] : "NULL");
	}
	*out << "\n";
	return 0;
}

void seedDatabase(sqlite3* db) {
	const char* initSql =
		R"(CREATE TABLE users(id INTEGER PRIMARY KEY, username TEXT, role TEXT);
		   INSERT INTO users(username, role) VALUES ('alice', 'admin');
		   INSERT INTO users(username, role) VALUES ('bob', 'user');
		   INSERT INTO users(username, role) VALUES ('charlie', 'user');)";
	char* errMsg = nullptr;
	if (sqlite3_exec(db, initSql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
		std::string err = errMsg ? errMsg : "unknown error";
		sqlite3_free(errMsg);
		throw std::runtime_error(fmt::format("Failed to seed database: {}", err));
	}
}
}  // namespace

std::string SqlRunner::executeUnsafeInMemory(const std::string& sql) {
	spdlog::warn("Executing UNSAFE SQL for testing purposes: {}", sql);
	sqlite3* db = nullptr;
	if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
		const char* msg = sqlite3_errmsg(db);
		sqlite3_close(db);
		return fmt::format("Failed to open in-memory DB: {}", msg ? msg : "unknown");
	}

	try {
		seedDatabase(db);
	} catch (const std::exception& ex) {
		sqlite3_close(db);
		return ex.what();
	}

	std::stringstream accumulator;
	char* errMsg = nullptr;
	int rc = sqlite3_exec(db, sql.c_str(), &captureCallback, &accumulator, &errMsg);
	if (rc != SQLITE_OK) {
		std::string err = errMsg ? errMsg : "unknown error";
		sqlite3_free(errMsg);
		sqlite3_close(db);
		return fmt::format("SQL error: {}", err);
	}
	sqlite3_close(db);
	return accumulator.str();
}

}  // namespace cpp_example


