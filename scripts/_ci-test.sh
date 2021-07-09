#!/usr/bin/env bash

set -e

./libs/cynthia/tests/cynthia-test

echo "========== Code coverage =========="
python3 -m gcovr -r . --config "gcovr.cfg" --print-summary --html --html-details -o coverage.html
python3 -m gcovr -r . --config "gcovr.cfg" --xml -o coverage.xml