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

# Function defines the main library component
#
# By looking at the folder structure it is possible to determine what type of library it is i.e.
# header only or static library.
function (setup_library name)

    # look up the files for the library
    file(GLOB_RECURSE headers include/*.hpp)
    file(GLOB_RECURSE srcs src/*.cpp)
    list(LENGTH headers headers_length)
    list(LENGTH srcs srcs_length)

    set(internal_headers_path ${CMAKE_CURRENT_SOURCE_DIR}/internal)

    # main library configuration
    if (srcs_length EQUAL 0)

        # define header only library
        add_library(${name} INTERFACE)
        target_sources(${name} INTERFACE ${headers})
        target_include_directories(${name} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

    else ()

        # define the normal library
        add_library(${name} ${headers} ${srcs})
        target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

        # add internal headers if one exists
        if (EXISTS ${internal_headers_path})
            target_include_directories(${name} PRIVATE ${internal_headers_path})
        endif ()

        # CoreFoundation Support on MacOS
        if (APPLE)
            target_link_libraries(${name} PUBLIC "-framework CoreFoundation")
        endif ()

    endif ()
endfunction ()
