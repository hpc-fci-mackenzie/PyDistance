cmake_minimum_required(VERSION 3.8)
set(CMAKE_BUILD_TYPE Release)
project(PyDistance)

# For development, only,
# Comment the next line after the project is working properly
# set(CMAKE_VERBOSE_MAKEFILE on)

set(PyDistance_NAME "PyDistance Library")
message(STATUS "${PyDistance_NAME}")

set(PyDistance_VERSION_MAJOR "1")
set(PyDistance_VERSION_MINOR "1")
set(PyDistance_VERSION_PATCH "1")

set(PyDistance_VERSION "${PyDistance_VERSION_MAJOR}.")
set(PyDistance_VERSION "${PyDistance_VERSION}${PyDistance_VERSION_MINOR}.")
set(PyDistance_VERSION "${PyDistance_VERSION}${PyDistance_VERSION_PATCH}")
message(STATUS "version: ${PyDistance_VERSION}")

# Set the reference standard of the C++ Language
set(CMAKE_CXX_STANDARD 11)

# Remove the "lib" prefix from all library objects
set(CMAKE_SHARED_LIBRARY_PREFIX "")
# Set the ".so" suffix in all library objects
set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")

### Flags
set(CXXFLAGS_O0 "-O0")
set(CXXFLAGS_O2 "-O2 -finline-functions -funswitch-loops -fpredictive-commoning -fgcse-after-reload -fno-tree-loop-vectorize -ftree-loop-distribute-patterns -fsplit-paths -fno-tree-slp-vectorize -fno-vect-cost-model -ftree-partial-pre -fpeel-loops -fipa-cp-clone")
set(CXXFLAGS_SSE2 "-O3 -march=native")
set(CXXFLAGS_AVX2 "-O3 -march=native -mavx2")
set(CXXFLAGS_AVX5 "-O3 -march=native -mavx512f -mavx512cd")

### Output
# Set the output directory for the .so file
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# Set the output directory for the binaries
set(CMAKE_BINARY_DIR ${PROJECT_BINARY_DIR}/bin)

# Set the output directory for the executables
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Option to choose the build's register width
set(REGISTER_WIDTH "auto" CACHE STRING "REGISTER_WIDTH set")
set_property(CACHE REGISTER_WIDTH PROPERTY STRINGS auto 128 256 512)
message(STATUS "REGISTER_WIDTH set: ${REGISTER_WIDTH}")

### Build
# Create the CaliperFactory.so artifact
add_library(CaliperFactory SHARED src/adapter/cpp/CaliperFactory.cpp src/adapter/cpp/CaliperFactory.hpp)

if ("${REGISTER_WIDTH}" MATCHES "auto")

    add_library(caliper SHARED src/caliper_basic.cpp src/caliper_basic.hpp src/caliper.hpp)
    set_property(TARGET caliper PROPERTY COMPILE_FLAGS ${CXXFLAGS_SSE2})

    target_compile_definitions(CaliperFactory PRIVATE _AUTO)
    target_link_libraries(CaliperFactory caliper)

elseif ("${REGISTER_WIDTH}" MATCHES "128")

    add_library(caliper SHARED src/caliper_128.cpp src/caliper_128.hpp src/caliper.hpp)
    set_property(TARGET caliper PROPERTY COMPILE_FLAGS ${CXXFLAGS_SSE2})

    target_compile_definitions(CaliperFactory PRIVATE _128)
    target_link_libraries(CaliperFactory caliper)

elseif ("${REGISTER_WIDTH}" MATCHES "256")

    add_library(caliper SHARED src/caliper_256.cpp src/caliper_256.hpp src/caliper.hpp)
    set_property(TARGET caliper PROPERTY COMPILE_FLAGS ${CXXFLAGS_AVX2})

    target_compile_definitions(CaliperFactory PRIVATE _256)
    target_link_libraries(CaliperFactory caliper)


elseif ("${REGISTER_WIDTH}" MATCHES "512")

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # requires at least gcc 7
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
            message(FATAL_ERROR "GCC version must be at least 7.0!")
        endif()
    endif()

    add_library(caliper SHARED src/caliper_512.cpp src/caliper.hpp src/caliper.hpp)
    set_property(TARGET caliper PROPERTY COMPILE_FLAGS ${CXXFLAGS_AVX5})

    target_compile_definitions(CaliperFactory PRIVATE _512)
    target_link_libraries(CaliperFactory caliper)

else()

    message (FATAL_ERROR "Invalid register width: '${REGISTER_WIDTH}'")

endif()


##########################
### PYTHON INTEGRATION ###
##########################

# Create the Caliper.so artifact
add_library(PyDistance SHARED src/adapter/cpp/CaliperAdapter.cpp)

# Set the Python version to default in the OS
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

# Link the Caliper lib with the CaliperFactory lib
target_link_libraries(PyDistance CaliperFactory ${PYTHON_LIBRARIES})

#############
### DEBUG ###
#############

add_library(benchmark SHARED src/benchmark/benchmark.cpp src/benchmark/benchmark.hpp src/benchmark/StopWatch.cpp src/caliper.hpp)

add_executable(test test/test.cpp)
target_link_libraries(test benchmark CaliperFactory)

# test.py
# Copy the Python's test file to the library directory
file(COPY test/test.py DESTINATION ${PROJECT_BINARY_DIR}/lib)
