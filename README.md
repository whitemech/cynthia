# Cynthia

Cynthia is a tool for SDD-based Forward LTLf Synthesis.

## Preliminaries

- CMake, at least version 3.2;
- SDD 2.0
    - You can find it here
      http://reasoning.cs.ucla.edu/sdd/
    - Follow the compilation instructions
```
cd libsdd-2.0
scons
cp sddapi.h /usr/local/include
cp build/libsdd.a /usr/local/lib
cp build/libsdd.a /usr/local/lib
```

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

To check that copyright notices are OK:

```
python scripts/check_copyright_notice.py
```
