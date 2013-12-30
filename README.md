Proto2D (temp title)
--------------------

This is an experimental, cross-platform game engine specialized for 2D desktop
game making. Currently, it is built on top of SFML2
(a nice but basic 2D multimedia library), but as the development progresses,
it may shift to SDL2 and some external libs.

The project was originally a game prototype, but I decided to put its
engine-related files open-source. The game is still there, but now it serves as
a testbed for the engine.

Features
--------

- Cross-platform (Windows, Linux, Mac)
- Component-oriented design
- Component-subsystems instead of Container-entities
- Cameras, sorted renderers and texture atlases
- Audio system based on spatialized emitters and a listener
- Easily versionnable meta-file system using JSON
- Template asset pipeline
- Source access for both gameplay and engine modifications
- TilEd JSON import http://www.mapeditor.org/

Planned/suggested :

- JSON/PropertyTree-based serialization
- Box2D integration
- Python, Lua or AngelScript scripting
- Script-based export pipeline
- Full-featured editor made using the engine
- Highly customizable rendering pipeline
- GUI (for both game and editor?)
- Entity parenting
- Scene transitions
- Networking
- Asynchronous tasks
- ...

Dependencies
------------

- SFML 2.1 http://www.sfml-dev.org/
- JsonBox (included in source) https://github.com/anhero/JsonBox
- zlib 1.2.8 (included in source) http://zlib.net/
- C++11 (works with GCC MinGW 4.7+)

Folder structure
----------------

Note: not all the source code is involved into the game/component-based
approach. Some parts (folders) have only a few dependencies,
and other were there before the 2D engine to be implemented. Keep in mind that
no additionnal dependency must be added to these files unless it is really
necessary.

|-assets/      : the assets of the game
|                (fonts, textures, atlases, maps... scripts?)
|
|-src/         : the C++ source code
| |
| |-fm/        : the engine's code.
| | |            "fm" is meant for "framework". (namespace is zn::).
| | |
| | |-asset/   : asset system (doesnt depends on the game engine,
| | |            it's a separate system)
| | |
| | |-proto/   : the actual game engine (window, components, scene...).
| | |            it uses all the other source files under fm/.
| | |
| | |-json/    : the JSON API used by the engine.
| | |              more at https://github.com/anhero/JsonBox
| | |
| | |-sfml/    : sfml utility that doesn't depends on the engine. It may also
| | |            contain drawable classes that are not component-based.
| | |
| | |-util/    : Only-STL-dependent files for various things such as 2D arrays,
| | |            pathfinding, math, Vector2i, Rect...
| | |
| | |-zlib/    : zlib compression library code
| |  
| |-ssm/       : contains the game's code (namespace is ssm::)

Notes
-----

I'm also involved into the NinjaSDK, an open-source 2D game engine.
Take a look ! http://ninjasdk.freeforall.fr/

