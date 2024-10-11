# RenderingEngine

My C++ render engine

# Build

There's 320 building steps and pretty big libraries so building may take some time.

## Windows

You can use CLion or Visual Studio with Ninja for building

- Clone the repository: `git clone --recursive https://github.com/Arimekiku/RenderingEngine`
- Make sure you use Ninja generator
- Hit `F5 (Build button)` and wait

## Linux

To compile on x86_64-linux platform one can use nix or cmake.

### Nix

```shell
nix build "git+file://$(pwd)?submodules=1"
```

Argument "git+file://$(pwd)?submodules=1" is used due to the fact that this project uses git submodules.

### Cmake

```shell
mkdir build && cd build
cmake configure ..
make
```

Compile time dependencies for Ubuntu roughly specified in the [workflow](./.github/workflows/build-check-x86_64-linux.yaml).
