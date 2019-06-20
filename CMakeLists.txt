cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE Release)
include(CheckSymbolExists)
project(PyDistance)

# For development, only,
# Comment the next line after the project is working properly
# set(CMAKE_VERBOSE_MAKEFILE on)

set(PyDistance_NAME "PyDistance Library")
message(STATUS "${PyDistance_NAME}")
set(CMAKE_CXX_FLAGS_DEBUG "-g")


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

### Base Flags
set(CXXFLAGS_O0 "-O0")
set(CXXFLAGS_O2 "-O2 -finline-functions -funswitch-loops -fpredictive-commoning -fgcse-after-reload -fno-tree-loop-vectorize -ftree-loop-distribute-patterns -fsplit-paths -fno-tree-slp-vectorize -fno-vect-cost-model -ftree-partial-pre -fpeel-loops -fipa-cp-clone -fopenacc")
set(CXXFLAGS_SSE2 "-O3 -march=native")
set(CXXFLAGS_AVX2 "-O3 -march=native -mavx2")
set(CXXFLAGS_AVX5 "-O3 -march=native -mavx512f -mavx512cd")

### OpenACC
include(${CMAKE_ROOT}/Modules/FindOpenACC.cmake)
if(OpenACC_CXX_FOUND)
    message("Found OpenACC ${OpenACC_CXX_VERSION}: added ${OpenACC_CXX_FLAGS} flag")
    set(CXXFLAGS_SSE2 "${CXX_FLAGS_SSE2} ${OpenACC_CXX_FLAGS}")
endif()

### Output
# Set the output directory for the .so file
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# Set the output directory for the binaries
set(CMAKE_BINARY_DIR ${PROJECT_BINARY_DIR}/bin)

# Set the output directory for the executables
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Option to choose the build's SIMD width
set(SIMD_WIDTH "auto" CACHE STRING "SIMD_WIDTH set")
set_property(CACHE SIMD_WIDTH PROPERTY STRINGS auto 128 256 512)

### Build
# Create the CaliperFactory.so artifact
add_library(CaliperFactory SHARED src/adapter/cpp/CaliperFactory.cpp src/adapter/cpp/CaliperFactory.hpp)

if ("${SIMD_WIDTH}" MATCHES "auto")

    # detect cpu extensions with some shellscript
    if(CMAKE_SYSTEM_NAME MATCHES "Linux" OR CMAKE_SYSTEM_NAME MATCHES "Darwin")

        exec_program(cat ARGS "/proc/cpuinfo" OUTPUT_VARIABLE CPUINFO)

        string(REGEX REPLACE "^.*(avx).*$" "\\1" CPU_FLAG_REGEX ${CPUINFO})
        string(COMPARE EQUAL "avx" "${CPU_FLAG_REGEX}" AVX_TRUE)
        if (AVX_TRUE)
            set(SIMD_WIDTH "128")
        endif()

        string(REGEX REPLACE "^.*(avx2).*$" "\\1" CPU_FLAG_REGEX ${CPUINFO})
        string(COMPARE EQUAL "avx2" "${CPU_FLAG_REGEX}" AVX2_TRUE)
        if (AVX2_TRUE)
            set(SIMD_WIDTH "256")
        endif()

        string(REGEX REPLACE "^.*(avx5).*$" "\\1" CPU_FLAG_REGEX ${CPUINFO})
        string(COMPARE EQUAL "avx5" "${CPU_FLAG_REGEX}" AVX5_TRUE)
        if (AVX5_TRUE)
            set(SIMD_WIDTH "512")
        endif()

    # elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")

    else()

        set(SIMD_WIDTH "auto")

    endif()
endif()

message(STATUS "SIMD_WIDTH set: ${SIMD_WIDTH}")

if ("${SIMD_WIDTH}" MATCHES "128")

    add_library(caliper SHARED src/caliper_128.cpp src/caliper_128.hpp src/caliper.hpp)
    set_property(TARGET caliper PROPERTY COMPILE_FLAGS ${CXXFLAGS_SSE2})
    target_compile_definitions(CaliperFactory PRIVATE _128)
    target_link_libraries(CaliperFactory caliper)

elseif ("${SIMD_WIDTH}" MATCHES "256")

    add_library(caliper SHARED src/caliper_256.cpp src/caliper_256.hpp src/caliper.hpp)
    set_property(TARGET caliper PROPERTY COMPILE_FLAGS ${CXXFLAGS_AVX2})
    target_compile_definitions(CaliperFactory PRIVATE _256)
    target_link_libraries(CaliperFactory caliper)

elseif ("${SIMD_WIDTH}" MATCHES "512")

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

elseif("${SIMD_WIDTH}" MATCHES AUTO)

    add_library(caliper SHARED src/caliper_basic.cpp src/caliper_basic.hpp src/caliper.hpp)
    set_property(TARGET caliper PROPERTY COMPILE_FLAGS ${CXXFLAGS_SSE2})
    target_compile_definitions(CaliperFactory PRIVATE _AUTO)
    target_link_libraries(CaliperFactory caliper)

else()

    message (FATAL_ERROR "Invalid SIMD width: '${SIMD_WIDTH}'")

endif()


##########################
### PYTHON INTEGRATION ###
##########################

# if(WIN32)
#     
#     # find_path(PYH_PATH Python.h)
#     include_directories(${PYH_PATH})
# endif()

# Create the Caliper.so artifact
add_library(PyDistance SHARED src/adapter/cpp/CaliperAdapter.cpp)

# Set the Python version to default in the OS
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

# Link the Caliper lib with the CaliperFactory lib
if(WIN32)
    target_link_libraries(PyDistance CaliperFactory "C:/Python/libs/python3.dll")
else()
    target_link_libraries(PyDistance CaliperFactory ${PYTHON_LIBRARIES})
endif()

#############
### DEBUG ###
#############

add_library(benchmark SHARED src/benchmark/benchmark.cpp src/benchmark/benchmark.hpp src/benchmark/StopWatch.cpp src/caliper.hpp)

add_executable(test_bin test/test_init.cpp)
target_link_libraries(test_bin benchmark CaliperFactory)

# test.py
# Copy the Python's test file to the library directory
file(COPY test/test.py DESTINATION ${PROJECT_BINARY_DIR}/lib)
