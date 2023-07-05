cmake_minimum_required(VERSION 3.20)
project(containers LANGUAGES C CXX VERSION 0.1)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options("-Wall" "-Weffc++" "-Wextra" "-Wsign-conversion" "-g" "-Werror")
else()
    message("WE NEED GCC FOR CODE COVERAGE AND COMPILE OPTIONS")
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/modules)
# FOR MAC'S HOMEBREW
list(APPEND CMAKE_SYSTEM_PREFIX_PATH /opt/goinfre/$ENV{USER}/homebrew/bin)
list(APPEND CMAKE_SYSTEM_PREFIX_PATH /opt/goinfre/$ENV{USER}/homebrew/sbin)

add_subdirectory(dependencies)

# CPPCHECK
find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if(CMAKE_CXX_CPPCHECK)
	list(APPEND CMAKE_CXX_CPPCHECK 
    "--enable=performance,portability"
    "--suppress=missingIncludeSystem")
endif()

#LIB COMPILATION
file(GLOB SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/lib/*.h)
add_library(${PROJECT_NAME} INTERFACE ${SRC_FILES})
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/lib)

#TEST COMPILATION
file(GLOB TEST_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/test/*.cc)
add_executable(test ${TEST_FILES})
target_link_libraries(test PUBLIC ${PROJECT_NAME} gtest gtest_main)

#CODE COVERAGE
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_BUILD_TYPE STREQUAL "Debug")
include(CodeCoverage)
append_coverage_compiler_flags_to_target(test)
setup_target_for_coverage_lcov(
            NAME test_coverage
            EXECUTABLE test
            EXCLUDE "${PROJECT_SOURCE_DIR}/dependencies/*" "/Applications/*" "/usr/local/*" "/usr/include/*"
            DEPENDENCIES test
    )
endif()

# FORMATTING
set(CLANG_FORMAT_EXCLUDE_PATTERNS  "build/" "dependencies/" ${CMAKE_BINARY_DIR})
find_package(ClangFormat)