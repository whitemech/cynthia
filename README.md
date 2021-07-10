# Cynthia

Cynthia is a tool for SDD-based Forward LTLf Synthesis.

## Preliminaries

- CMake, at least version 3.2;

## Build

Build as any CMake project:

```
mkdir build && cd build
cmake ..
make -j4
```

## Development


The project uses clang-format for code formatting.
To check the project satisfies the format:
```
./scripts/check-clang-format.sh
```

To apply the changes:
```
./scripts/apply-clang-format.sh
```
