# Entity Component System
Part of the Editor project

A entity component game system. The architecture is based on this blog: http://bitsquid.blogspot.se/2014/08/building-data-oriented-entity-system.html; much love to him.

Currently concists of a scene manager and transform manager.
Scene manager build a scene of entities.
Transform managers hold the transform.
- [x] Inherited transforms
- [x] Deferred calculations, which makes it easy to multi-thread.

- [x] The managers are all serialized and can be saved and loaded.
- [x] Each individual component can be saved and recreated.(Create a scene and load that for the game)

# Dependencis
* CompileTimeGUID
* Profiler
* Sofa
* Renderer
* Resource Handler

# Build
If using Visual Studio (2017) just get each dependency so that they are located in the same folder then you should only need to press build.

If not using Visual Studio I wish you the best of luck.
