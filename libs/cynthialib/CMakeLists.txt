cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(cynthialib)


#configure directories
set (CYNTHIALIB_MODULE_PATH ${PROJECT_SOURCE_DIR}/)
set (CYNTHIALIB_SRC_PATH  "${CYNTHIALIB_MODULE_PATH}/src" )
set (CYNTHIALIB_INCLUDE_PATH  "${CYNTHIALIB_MODULE_PATH}/include")

#set includes
include_directories (${CYNTHIALIB_INCLUDE_PATH} ${THIRD_PARTY_INCLUDE_PATH})
include_directories (${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

#set sources
file (GLOB_RECURSE CYNTHIALIB_HEADER_FILES "${CYNTHIALIB_INCLUDE_PATH}/*.hpp" ${CYNTHIALIB_INCLUDE_PATH}/*.h ${CYNTHIALIB_INCLUDE_PATH}/*.hh)
file (GLOB_RECURSE CYNTHIALIB_SOURCE_FILES "${CYNTHIALIB_SRC_PATH}/*.cpp" ${CYNTHIALIB_INCLUDE_PATH}/*.cpp ${CYNTHIALIB_INCLUDE_PATH}/*.cc)


# ------------------------------------------------------------------------------
# Main Library Target
# ------------------------------------------------------------------------------

add_library (${LIB_NAME} STATIC ${CYNTHIALIB_SOURCE_FILES} ${CYNTHIALIB_HEADER_FILES})
#target_link_libraries(${LIB_NAME})
