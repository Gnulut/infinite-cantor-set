# Infinite Cantor Set
This project is a simple interactive visualization of the Menger Sponge fractal using [SDL3](https://github.com/libsdl-org/SDL) for cross-platform graphics.

## Controls

- Mouse Wheel Up: zoom in at mouse position
- Keyboard:
    - \- (minus) sign: reduce scaling speed
    - = (equal) sign: increase scaling speed
    - z alphabet: zoom in at mouse position

## Live Demo

[Web Demo](https://gnulut.github.io/infinite-cantor-set/)

## Download (Windows)

[Docs](docs)

## Building
### Build Requirement
- C++ Compiler (Recommended C++20 and newer)
- git
- cmake (Recommended cmake 3.16)

### Build commands

```shell
# You need to clone with submodules, otherwise SDL will not download.
git clone https://github.com/Gnulut/infinite-cantor-set --depth=1 --recurse-submodules
cd infinite-cantor-set
cmake -S . -B build
cmake --build build
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
---

*Made with SDL3 and C++*