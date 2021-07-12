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
set(CYNTHIA_APP_NAME cynthia-app)
project(${CYNTHIA_APP_NAME})

#configure directories
set (CYNTHIA_APP_MODULE_PATH ${PROJECT_SOURCE_DIR}/)
set (CYNTHIA_APP_SRC_PATH  "${CYNTHIA_APP_MODULE_PATH}/src" )
set (CYNTHIA_APP_INCLUDE_PATH  "${CYNTHIA_APP_MODULE_PATH}/include")

add_executable(${CYNTHIA_APP_NAME} main.cpp)
target_link_libraries(${CYNTHIA_APP_NAME}
        PRIVATE ${CYNTHIA_CORE_LIB_NAME})
target_include_directories(${CYNTHIA_APP_NAME}
        PRIVATE ${THIRD_PARTY_INCLUDE_PATH}
                ${CYNTHIA_CORE_INCLUDE_PATH})