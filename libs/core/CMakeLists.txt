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
set(CYNTHIA_CORE_LIB_NAME cynthia-core)

project(${CYNTHIA_CORE_LIB_NAME})


#configure directories
set (CYNTHIA_CORE_MODULE_PATH ${PROJECT_SOURCE_DIR})
set (CYNTHIA_CORE_SRC_PATH  "${CYNTHIA_CORE_MODULE_PATH}/src" )
set (CYNTHIA_CORE_INCLUDE_PATH  "${CYNTHIA_CORE_MODULE_PATH}/include")

#set includes
include_directories (${CYNTHIA_CORE_INCLUDE_PATH} ${THIRD_PARTY_INCLUDE_PATH})
include_directories (${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

#set sources
file (GLOB_RECURSE CYNTHIA_CORE_HEADER_FILES "${CYNTHIA_CORE_INCLUDE_PATH}/*.hpp" ${CYNTHIA_CORE_INCLUDE_PATH}/*.h ${CYNTHIA_CORE_INCLUDE_PATH}/*.hh)
file (GLOB_RECURSE CYNTHIA_CORE_SOURCE_FILES "${CYNTHIA_CORE_SRC_PATH}/*.cpp" ${CYNTHIA_CORE_INCLUDE_PATH}/*.cpp ${CYNTHIA_CORE_INCLUDE_PATH}/*.cc)


# ------------------------------------------------------------------------------
# Main Library Target
# ------------------------------------------------------------------------------

add_library (${CYNTHIA_CORE_LIB_NAME} STATIC ${CYNTHIA_CORE_SOURCE_FILES} ${CYNTHIA_CORE_HEADER_FILES})
#target_link_libraries(${CYNTHIA_CORE_LIB_NAME})

add_subdirectory(tests)

#export vars
set (CYNTHIA_CORE_LIB_NAME  ${${CYNTHIA_CORE_LIB_NAME}} PARENT_SCOPE)
set (CYNTHIA_CORE_INCLUDE_PATH  ${CYNTHIA_CORE_INCLUDE_PATH} PARENT_SCOPE)
