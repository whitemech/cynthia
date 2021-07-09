# CMake build : third party

#configure directories
set (THIRD_PARTY_MODULE_PATH "${PROJECT_SOURCE_DIR}/vendor")

# -------------------------------------------------------------------------
# catch
set (CATCH_MODULE_PATH "${THIRD_PARTY_MODULE_PATH}/catch2")
set (CATCH_INCLUDE_PATH "${CATCH_MODULE_PATH}/single_include/catch2")
include ( "${CATCH_MODULE_PATH}/extras/ParseAndAddCatchTests.cmake")
# -------------------------------------------------------------------------
# -------------------------------------------------------------------------
# spdlog
set (SPDLOG_MODULE_PATH "${THIRD_PARTY_MODULE_PATH}/spdlog")
set (SPDLOG_INCLUDE_PATH "${SPDLOG_MODULE_PATH}/include")
# -------------------------------------------------------------------------
# -------------------------------------------------------------------------
# CLI11
set (CLI11_MODULE_PATH "${THIRD_PARTY_MODULE_PATH}/cli11")
set (CLI11_INCLUDE_PATH "${CLI11_MODULE_PATH}/include")
# -------------------------------------------------------------------------

#set variables
set (THIRD_PARTY_INCLUDE_PATH
        ${SPDLOG_INCLUDE_PATH}
        ${CLI11_INCLUDE_PATH})

#set variables for tests
set (TEST_THIRD_PARTY_INCLUDE_PATH
        ${CATCH_INCLUDE_PATH})

#export vars
set (THIRD_PARTY_INCLUDE_PATH  ${THIRD_PARTY_INCLUDE_PATH} PARENT_SCOPE)
set (TEST_THIRD_PARTY_INCLUDE_PATH  ${TEST_THIRD_PARTY_INCLUDE_PATH} PARENT_SCOPE)
