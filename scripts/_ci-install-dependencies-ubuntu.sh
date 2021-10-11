#!/usr/bin/env bash

set -e

echo "Installing SDD..."
git clone https://github.com/wannesm/PySDD.git
cd PySDD/pysdd/lib/sdd-2.0
sudo cp -P lib/Linux/* /usr/local/lib/
sudo cp -Pr include/* /usr/local/include
cd ..
echo "SDD installed."
