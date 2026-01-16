from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout


class MqttBridge(ConanFile):
    name = "MqttBridge"
    version = "0.1.0"
    description = "A C++ MQTT Bridge Library"
    settings = "os", "compiler", "build_type", "arch"

    default_options = {
        "boost/*:shared": True,
        "tomlplusplus/*:shared": True,
        "fmt/*:shared": True
    }

    def requirements(self):
        self.requires("boost/1.83.0", transitive_headers=True, transitive_libs=True)
        self.requires("tomlplusplus/3.4.0")
        self.requires("fmt/12.1.0")
        self.requires("cli11/2.6.0")
        self.requires("mosquitto/2.0.18")

    def build_requirements(self):
        self.build_requires("cmake/3.26.4")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
