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

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(cynthia)


#configure directories
set (CYNTHIA_MODULE_PATH ${PROJECT_SOURCE_DIR})
set (CYNTHIA_SRC_PATH  "${CYNTHIA_MODULE_PATH}/src" )
set (CYNTHIA_INCLUDE_PATH  "${CYNTHIA_MODULE_PATH}/include")

#set includes
include_directories (${CYNTHIA_INCLUDE_PATH} ${THIRD_PARTY_INCLUDE_PATH})
include_directories (${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

#set sources
file (GLOB_RECURSE CYNTHIA_HEADER_FILES "${CYNTHIA_INCLUDE_PATH}/*.hpp" ${CYNTHIA_INCLUDE_PATH}/*.h ${CYNTHIA_INCLUDE_PATH}/*.hh)
file (GLOB_RECURSE CYNTHIA_SOURCE_FILES "${CYNTHIA_SRC_PATH}/*.cpp" ${CYNTHIA_INCLUDE_PATH}/*.cpp ${CYNTHIA_INCLUDE_PATH}/*.cc)


# ------------------------------------------------------------------------------
# Main Library Target
# ------------------------------------------------------------------------------

add_library (cynthia STATIC ${CYNTHIA_SOURCE_FILES} ${CYNTHIA_HEADER_FILES})
#target_link_libraries(${LIB_NAME})

add_subdirectory(tests)

#export vars
set (CYNTHIA_INCLUDE_PATH  ${CYNTHIA_INCLUDE_PATH} PARENT_SCOPE)
