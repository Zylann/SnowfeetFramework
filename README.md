Proto2D (temp title)
--------------------

This is an experimental, cross-platform game engine specialized for 2D desktop
game making. Currently, it is built on top of SFML
(a nice but basic 2D multimedia library), but as the development progresses,
it may shift to SDL 2 and some external libs.

The project was originally a game prototype, but I decided to put its
engine-related files open-source and leave a sample as demo.
I update the framework when I need something special or just as a hobby.

Features
--------

- Cross-platform (Windows, Linux, Mac)
- Component-oriented design, using subsystems instead of container-entities
- Sorted rendering with dynamic draw order
- Texture atlases with frames and animation sequences
- Simple audio system based on spatialized emitters and a listener
- Aim to be easily versionnable by using text (JSON) data files
- Asset loader based on templates
- Native interface designed for both gameplay and engine modification
- TilEd JSON import http://www.mapeditor.org/
- JSON scene dump (editor serialization preview, currently for debug)
- Entity hierarchy (parenting)
- Shaders with merged files support (vertex+fragment in same file)
- Layers with sorting semantic
- Simple 2D culling
- Log system

Planned/suggested :

- Lighting and 2D shadows
- Box2D integration
- Better culling for BIG scenes
- Scene editor
- Tween library
- JSON/PropertyTree-based serialization of components
- Enable depth buffer as an option
- Scripting language (Lua? Python? AngelScript? Squirrel?)
- Scene editor made using the engine
- Scriptable export pipeline
- Easily customizable rendering pipeline
- GUI components (for both game and editor?)
- Scene transitions (and multi-scene?)
- Networking (using enet? Need ipv6 update)
- Asynchronous tasks
- Audio effects (future SFML releases? FmodEx? raw OpenAL?)
- Prefabs
- ...

Dependencies
------------

- SFML 2.1 http://www.sfml-dev.org/
- JsonBox (included in source (not for ever)) https://github.com/anhero/JsonBox
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
| |-sfml/    : sfml utility that doesn't depends on the engine. It may also
| |            contain drawable classes that are not component-based.
| |
| |-util/    : Only-STL-dependent files for various things such as 2D arrays,
|            pathfinding, math, Vector2i, Rect...

Notes
-----

- Undocumented/uncommented elements are either likely to change in the future,
may be explicit enough, or simply not done yet.

- Some "3D" stuff may be implemented in the future to ease things like
the use of normal maps, lighting or scrolling effects, but this stays a 2D engine,
and is not aimed at rendering complex 3D models (better use a 3D engine, then !).
Discussion is open for the far future, but this is what I think at the moment.

- I'm also involved into the NinjaSDK, an open-source 2D game framework.
Take a look ! http://ninjasdk.freeforall.fr/

