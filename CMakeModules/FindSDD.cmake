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

# Try to find SDD headers and libraries.
#
# Usage of this module as follows:
#
# find_package(SDD)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
# SDD_ROOT Set this variable to the root installation of
# libSDD if the module has problems finding the
# proper installation path.
#
# Variables defined by this module:
#
# SDD_FOUND System has SDD libraries and headers
# SDD_LIBRARIES The SDD library
# SDD_INCLUDE_DIRS The location of SDD headers

# Get hint from environment variable (if any)
if(NOT SDD_ROOT AND DEFINED ENV{SDD_ROOT})
	set(SDD_ROOT "$ENV{SDD_ROOT}" CACHE PATH "SDD base directory location (optional, used for nonstandard installation paths)")
	mark_as_advanced(SDD_ROOT)
else()
	set(SDD_ROOT "/usr/local")
endif()

set(SDD_ROOT "/usr/local")
# Search path for nonstandard locations
if(SDD_ROOT)
	set(SDD_INCLUDE_PATH PATHS "${SDD_ROOT}/include" NO_DEFAULT_PATH)
	set(SDD_LIBRARY_PATH PATHS "${SDD_ROOT}/lib" NO_DEFAULT_PATH)
endif()

find_path(SDD_INCLUDE_DIRS NAMES sddapi.h HINTS ${SDD_INCLUDE_PATH})
find_library(SDD_LIBRARIES NAMES libsdd.a libsdd.so HINTS ${SDD_LIBRARY_PATH})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(SDD DEFAULT_MSG SDD_LIBRARIES SDD_INCLUDE_DIRS)

mark_as_advanced(SDD_ROOT SDD_LIBRARIES SDD_INCLUDE_DIRS)
