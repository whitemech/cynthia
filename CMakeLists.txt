cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(cynthia
        LANGUAGES CXX
        HOMEPAGE_URL https://github.com/whitemech/sdd-based-forward-ltlf-synthesis.git
        DESCRIPTION "SDD-based Forward LTLf Synthesis.")
set(PROJECT_VERSION "0.1.0")
set(${PROJECT_NAME}_VERSION ${PROJECT_VERSION})

add_definitions( -DCYNTHIA_VERSION="${PROJECT_VERSION}" )
set_property (GLOBAL PROPERTY USE_FOLDERS ON)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4.8
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8)
        message(FATAL_ERROR "GCC version must be at least 8!")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # require at least clang 3.2
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6)
        message(FATAL_ERROR "Clang version must be at least 6!")
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC.")
endif()


set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message("-- Using build type ${CMAKE_BUILD_TYPE}")


if ((ENABLE_COVERAGE) AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    message("-- Code coverage enabled")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ftest-coverage")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fPIC")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

if (PROFILE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
endif()


set(CMAKE_MODULE_PATH
        "${CMAKE_MODULE_PATH}"
        "${CMAKE_ROOT_DIR}/cmake/Modules"
        "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

add_subdirectory (vendor EXCLUDE_FROM_ALL)
add_subdirectory(libs)
add_subdirectory(apps)


