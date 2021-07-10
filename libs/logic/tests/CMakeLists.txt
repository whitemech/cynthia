#
# This file is part of Cynthia.
#
# Cynthia is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Cynthia is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Cynthia.  If not, see <https://www.gnu.org/licenses/>.
#

#configure variables
set (TEST_APP_NAME "cynthia-logic-test")

#configure directories
set (TEST_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

#configure test directories
set (TEST_UNIT_SRC_PATH  "${TEST_MODULE_PATH}/unit" )

#set includes
include_directories (${CYNTHIA_LOGIC_INCLUDE_PATH} ${TEST_THIRD_PARTY_INCLUDE_PATH})

#set test sources
file (GLOB_RECURSE TEST_SOURCE_FILES "${TEST_UNIT_SRC_PATH}/*.cpp")
file (GLOB_RECURSE TEST_HEADER_FILES "${TEST_UNIT_SRC_PATH}/*.hpp")

#set target executable
add_executable (${TEST_APP_NAME} ${TEST_SOURCE_FILES} ${TEST_HEADER_FILES})

#add the library
target_link_libraries (${TEST_APP_NAME}
        ${LIB_NAME}
        PRIVATE Catch2::Catch2)

# Turn on CMake testing capabilities
enable_testing()

catch_discover_tests(${TEST_APP_NAME})