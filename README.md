# lonely-tennis

Little 3D game about one player tennis vs walls.

# Compilation
**Dependencies**
- cmake
- [cglm](https://github.com/recp/cglm)
- SDL2
- SDL2_image
- OpenGL
- glew
- Testing
    - check
    - valgrind
    
**Build**
1. Create a build folder (`mkdir build`)
2. Go into that folder (`cd build`)
3. Run cmake (debug version: `cmake -DCMAKE_BUILD_TYPE=DEBUG ..` release version: `cmake -DCMAKE_BUILD_TYPE=RELEASE ..`)
4. Run make (`make`)
5. Run tests (`ctest -T memcheck`)
