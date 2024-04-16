# NYL - Entity Component System 2D Game Engine

NYL is an Entity Component System (ECS) 2D game engine written in C++ with OpenGL. It provides a flexible and efficient architecture for developing 2D games with a focus on performance and simplicity.

## Getting the Project

To clone the project, use the `--recursive` option to ensure that all submodules are cloned:

```bash
git clone --recursive https://github.com/tinccristian/nyl.git
```

This will clone the NYL repository along with its submodules, including spdlog.

## Building

### Visual Studio 2022 (Windows)

1. Run the `cmake-generate-vs22` script.
2. Navigate to the `build-vs22` folder.
3. Open `platformer.sln`.
4. Set the `platformer` project as the startup project.
5. Build the solution normally. The output can be found in the `bin/` directory.

### GCC (Windows with Mingw)

Ensure that Ninja and Mingw are installed and available in your PATH.

1. Run the `build.bat` script from the project root directory.
2. Optionally, you can specify additional build options:
   - `-clean`: Perform a clean build.
   - `-run`: Run the executable after building.
   - `-r`: Build in Release mode.
   - `-info`: Generate build information in a text file.

Example usage:

```bash
build.bat -clean -run -r -info
```

This will clean the build, build the project in Release mode, generate build information, and run the executable.

## Usage

Once the project is built, you can use the generated executable to run your game. Additional usage instructions specific to your game or application should be provided here.

## Contributing

Contributions to NYL are welcome! Please read the [CONTRIBUTING.md](CONTRIBUTING.md) file for guidelines on how to contribute to the project.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
