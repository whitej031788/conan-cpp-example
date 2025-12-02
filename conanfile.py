from conan import ConanFile
from conan.tools.cmake import cmake_layout


class ConanCppExample(ConanFile):
	name = "conan_cpp_example"
	version = "0.1.0"
	package_type = "application"
	settings = "os", "arch", "compiler", "build_type"
	generators = ("CMakeToolchain", "CMakeDeps", "VirtualBuildEnv")
	tool_requires = (
		"cmake/4.2.0",
		"ninja/1.11.1",
		"nasm/2.15.05",
	)

	def layout(self) -> None:
		cmake_layout(self)

	def requirements(self) -> None:
		# Core dependencies used by the sample
		self.requires("fmt/10.2.1")
		self.requires("spdlog/1.14.1")
		self.requires("nlohmann_json/3.11.3")
		self.requires("cpr/1.10.5")
		self.requires("sqlite3/3.46.0")
		self.requires("cxxopts/3.2.0")

		# Extra dependencies to increase the dependency graph for SCA tooling
		self.requires("boost/1.84.0")
		self.requires("libxml2/2.12.7")
		self.requires("expat/2.6.3")
		self.requires("libpng/1.6.43")
		self.requires("libjpeg-turbo/3.0.2")
		self.requires("zstd/1.5.6")
		self.requires("re2/20230301")
		self.requires("abseil/20240116.2")
		self.requires("c-ares/1.27.0")

