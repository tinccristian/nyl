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

- `examples/platformer` — the original sample game (kept as a code example).
- `editor` — the **NYL Editor**: create/open/configure **projects**, author
  scenes, and manage assets without an external IDE.
- `runtime` — the **NYL Runtime**: a standalone, data-driven player. Run
  `runtime.exe <path-to-.nylproj>` (or no args for the bundled sample) to play a
  project: rendering, fixed-step physics, particles, and a built-in platformer
  controller for the entity whose collider is flagged `"player"`. This is how a
  project authored in the editor "ships" — no editor, no IDE. The editor's
  **Run Project** (F5) saves and launches it.

The engine has no runtime file dependency: its shader is embedded, and the
editor/runtime resolve the bundled sample relative to the executable.

## Projects

A project is a folder with a `.nylproj` settings file (name, window size,
asset/scene dirs, startup scene), an `assets/` folder, and a `scenes/` folder.
`projects/sample-platformer/` is a ready-made example (chikboy + background +
cloud sprites, a particle emitter) — the editor opens it on first run.

The editor (`Project`/`Scene` menus) can:
- **New / Open / Save Project** and edit settings in the **Project** panel.
- **New / Open / Save / Save As** scenes; **Set as Startup Scene**.
- **Import textures** (native file dialogs via tinyfiledialogs) and auto-load a
  project's assets so the Sprite picker is populated.
- **Hierarchy**: create entities/sprites, duplicate, delete.
- **Inspector**: registry-driven — add/remove/edit any registered component
  (Transform, Sprite, Physics, BoxCollider, Camera, ParticleEmitter, Tilemap,
  AudioSource).
- **Viewport** (rendered to an off-screen framebuffer): click to select, drag to
  move, arrows to pan, wheel to zoom; Play/Pause to preview simulation.
- **Undo/redo** (Ctrl+Z / Ctrl+Y), **Run Project** (F5), Save Scene (Ctrl+S).
- Remembers the last project (`editor_config.txt`) and reopens it on launch.

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
