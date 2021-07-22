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
set(CYNTHIA_SDD_LIB_NAME cynthia-sdd)

project(${CYNTHIA_SDD_LIB_NAME})


#configure directories
set (CYNTHIA_SDD_MODULE_PATH ${PROJECT_SOURCE_DIR})
set (CYNTHIA_SDD_SRC_PATH  "${CYNTHIA_SDD_MODULE_PATH}/src" )
set (CYNTHIA_SDD_INCLUDE_PATH  "${CYNTHIA_SDD_MODULE_PATH}/include")

#set includes
include_directories (${CYNTHIA_SDD_INCLUDE_PATH} ${THIRD_PARTY_INCLUDE_PATH})
include_directories (${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

#set sources
file (GLOB_RECURSE CYNTHIA_SDD_HEADER_FILES "${CYNTHIA_SDD_INCLUDE_PATH}/*.hpp" ${CYNTHIA_SDD_INCLUDE_PATH}/*.h ${CYNTHIA_SDD_INCLUDE_PATH}/*.hh)
file (GLOB_RECURSE CYNTHIA_SDD_SOURCE_FILES "${CYNTHIA_SDD_SRC_PATH}/*.cpp" ${CYNTHIA_SDD_INCLUDE_PATH}/*.cpp ${CYNTHIA_SDD_INCLUDE_PATH}/*.cc)


# ------------------------------------------------------------------------------
# Main Library Target
# ------------------------------------------------------------------------------

add_library (${CYNTHIA_SDD_LIB_NAME}
        STATIC
            ${CYNTHIA_SDD_SOURCE_FILES}
            ${CYNTHIA_SDD_HEADER_FILES})


set_target_properties(${CYNTHIA_SDD_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(tests)

#export vars (globally)
set (CYNTHIA_SDD_LIB_NAME  ${CYNTHIA_SDD_LIB_NAME} CACHE INTERNAL "CYNTHIA_SDD_LIB_NAME")
set (CYNTHIA_SDD_INCLUDE_PATH  ${CYNTHIA_SDD_INCLUDE_PATH}  CACHE INTERNAL "CYNTHIA_SDD_INCLUDE_PATH")
