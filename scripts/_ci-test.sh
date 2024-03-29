#!/usr/bin/env bash

set -e
shopt -s inherit_errexit

if [ $# -lt 1 ] &&  [ $# -gt 3 ]
  then
    echo "Usage: _ci-test.sh MODULE_NAME TEST_SPEC [CODECOV_TOKEN]"
    exit 1
fi

MODULE_NAME="$1"
TEST_SPEC="$2"
CODECOV_TOKEN="$3"
MODULE_ID="cynthia-${MODULE_NAME}"
MODULE_TEST_APP="${MODULE_ID}-test"

cd build                                            &&\
 make -j "${MODULE_TEST_APP}"                       &&\
 cd ..                                              &&\
 ./build/libs/"${MODULE_NAME}"/tests/"${MODULE_TEST_APP}" "${TEST_SPEC}"

echo "========== Code coverage =========="
gcovr -r . --config "gcovr.cfg" --print-summary --html --html-details -o coverage.html
gcovr -r . --config "gcovr.cfg" --xml -o coverage.xml

if [ -n "${CODECOV_TOKEN}" ]; then
  echo "Uploading coverage to Codecov";
  ./codecov -t "${CODECOV_TOKEN}" -c -F "${MODULE_ID}";
fi
