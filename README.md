# Cynthia

Cynthia is a tool for SDD-based Forward LTLf Synthesis.

## Preliminaries

- CMake, at least version 3.2;
- Flex & Bison
- SDD 2.0
```
git clone https://github.com/wannesm/PySDD.git
cd PySDD/pysdd/lib/sdd-2.0
sudo cp -P lib/Linux/* /usr/local/lib/
sudo cp -Pr include/* /usr/local/include
```

## Build

Build as any CMake project:

```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
```

## Development


The project uses `clang-format` for code formatting.
Install the tool `clang-format`, e.g. on Ubuntu using APT:
```
sudo apt install clang-format
```

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
