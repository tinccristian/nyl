from conan import ConanFile

class NylEngineConan(ConanFile):
    name = "nylengine"
    version = "1.0"
    # license =
    # author =
    # url =
    # description =
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("glfw/3.4")
        self.requires("glm/cci.20230113")
        self.requires("imgui/cci.20230105+1.89.2.docking")
        self.requires("stb/cci.20230920")
        self.requires("spdlog/1.13.0")