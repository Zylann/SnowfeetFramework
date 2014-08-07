Snowfeet Framework
--------------------

This is an experimental, cross-platform game engine specialized for 2D desktop
game making. Currently, it is built on top of SFML, a nice multimedia library.

The project was originally a game prototype, but I decided to put its
engine-related files open-source and leave a sample as demo.
I update the framework when I need something special or just as a hobby.

The name "Snowfeet" came from the game I initially planned to realize.
In an post-apocalyptic future, It's the name of an uber-rare pet supposed to 
be the closest descendant of cats.

The main namespace used is "zn" for historic reason (ZCraft/Zylann engine) and is not subject to change yet.

Features
--------

- Cross-platform (uses only cross-platform open-source libs and standard C++11)
- Entity-component system included in Scenes (inspired by Unity3D)
- Sorted rendering with dynamic draw order
- Texture atlases with frames and animation sequences
- Simple audio system based on spatialized emitters and a listener
- Uses JSON as main data format
- Asset loader based on C++ templates
- C++ API designed for easy game coding
- TilEd JSON import http://www.mapeditor.org/
- JSON scene dump (editor serialization preview, currently for debug)
- Shaders with merged files support (vertex+fragment in same file)
- Layers with sorting semantic
- Simple 2D culling
- Log system
- Particle systems
- Grid-based A* pathfinding
- And more...

Planned/suggested :

- Lighting and 2D shadows (might be optional)
- Box2D integration
- Better culling for BIG scenes
- Tween/animation library
- JSON/PropertyTree-based serialization of components
- Enable depth buffer as an option
- Scripting language, might be Squirrel (lightweight as Lua but object-oriented and less error-prone)
- Scene editor made using the engine?
- Easily customizable rendering pipeline
- GUI components (for both game and editor?)
- Scene transitions (and multi-scene?)
- Networking (using enet? Need ipv6 update)
- Asynchronous tasks
- Audio effects (future SFML releases? FmodEx? raw OpenAL?)
- Prefabs
- ...

External dependencies
------------

- SFML 2.1 http://www.sfml-dev.org/
- C++11 (works with GCC MinGW 4.7+)

Folder structure
----------------

Note: not all the source code is involved into the game/component-based
approach. Some parts (folders) have only a few dependencies,
and other were there before the 2D engine to be implemented. Keep in mind that
no additionnal dependency must be added to these files unless it is really
necessary.

|-assets/    : the assets of the game
|
|-demo/       : contains the game's code (namespace is demo::)
|
|-fm/        : the framework's code.
| |            "fm" is meant for "framework". (namespace is zn::).
| |
| |-asset/   : asset system (doesnt depends on the game engine,
| |              it's a separate system)
| |
| |-proto/   : the actual game engine (window, components, scene...).
| |            it uses all the other source files under fm/.
| |
| |-pcg/     : Procedural content generation library
| |
| |-json/    : the JSON API used by the engine.
| |              more at https://github.com/anhero/JsonBox
| |
| |-sfml/      : sfml utility that doesn't depends on the engine. It may also
| |              contain drawable classes that are not component-based.
| |
| |-squirrel/ : all about the Squirrel scripting engine
| |
| |-network/  : things related to network communication
| |
| |-util/      : Only-STL-dependent files for various things such as 2D arrays,
|              pathfinding, math, Vector2i, Rect...

Notes
-----

- Undocumented/uncommented elements are either likely to change in the future,
may be explicit enough, or simply not done yet.

- As the framework gets bigger, I can't keep everything up to date at the same time.
so it is possible to find outdated code or documentation, until I fix it.

- Some "3D" stuff may be implemented in the future to ease things like
the use of normal maps, lighting or scrolling effects, but this stays a 2D engine,
and is not aimed at rendering complex 3D models (better use a 3D engine, then !).
Discussion is open for the far future, but this is what I think at the moment.

- I also took part of game-engine-related discussions with friends,
Take a look ! http://ninjasdk.freeforall.fr/

