from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout


class HandLearnerAcquisition(ConanFile):
    name = "hand_learner_acquisition"
    version = "0.1.0"
    license = "MIT"
    author = "Ivan Iziumov"
    description = "Hand learner hardware data acquisition folder"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    def requirements(self):
        self.requires("raylib/5.5")

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.30]")

    def generate(self):
        tc = CMakeToolchain(self, generator="Ninja")
        tc.generate()

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
