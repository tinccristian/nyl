# NYL - Component-based 2D Game Engine

NYL is a 2D game engine written in C++20 with OpenGL. A central **Scene** owns
entities and typed component pools; **systems** operate over the scene; a
**component registry** drives serialization and a built-in **ImGui editor**.

## Architecture

- **Scene / Entity** (`engine/src/entity/`) — a `Scene` owns entities (generational
  handles) and component pools. `Entity` is a lightweight `{ id, scene }` handle.
- **Component registry** (`component_registry.*`) — each component type registers
  add/has/remove plus optional `serialize` / `deserialize` (JSON) and `inspect`
  (ImGui) callbacks. Serialization and the editor are both registry-driven, so
  game-defined components participate automatically.
- **Systems** (`engine/src/system/`) — batched sprite renderer (one draw call per
  texture, unified world-space camera), fixed-timestep physics, collision with a
  uniform-grid broadphase, particles, tilemaps (+ Tiled JSON import), an 8x8
  bitmap text renderer, and an off-screen `Framebuffer` for the editor viewport.
- **Core** (`engine/src/_core/`) — windowing/GLFW, a polling keyboard/mouse/gamepad
  `Input` layer with edge detection, `AudioEngine` (miniaudio), `ResourceManager`,
  logging, and the `Game` loop (`ProcessInput → FixedUpdate(s) → Update → Render →
  OnImGui`).

## Targets

- `examples/platformer` — the sample game (keyboard + gamepad, particles, text,
  on-screen save-scene button).
- `editor` — the **NYL Editor**: a scene hierarchy, a registry-driven inspector
  (add/remove/edit any component), an FBO viewport, and scene save/load.

## Controls (platformer)

- Move: `A`/`D` or arrow keys · Jump: `Space`/`W` · Gamepad supported
- `F1`: toggle wireframe · `Esc`: quit

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

## Contributing

Contributions to NYL are welcome! Please read the [CONTRIBUTING.md](CONTRIBUTING.md) file for guidelines on how to contribute to the project. Additionally, tasks and project boards can be found on our Trello board: [NYL Trello Board](https://trello.com/w/nylengine).


## License

This project is licensed under the Apache 2.0 License. See the [LICENSE](LICENSE) file for details.
