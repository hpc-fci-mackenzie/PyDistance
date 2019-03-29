cmake_minimum_required(VERSION 3.8)
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)
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
set(CXXFLAGS_AVX5 "-O3 -march=native -mavx512f -mavx512vl")

### Output
# Set the output directory for the .so
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# Set the output directory for the binaries
set(CMAKE_BINARY_DIR ${PROJECT_BINARY_DIR}/bin)

# Set the output directory for the executables
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Option to choose DataType usage
set(ATA_TYPE "auto" CACHE STRING "DataType chosen")
set_property(CACHE ATA_TYPE PROPERTY STRINGS auto 128 256 512)
message(STATUS "DataType chosen: ${ATA_TYPE}")

### Build
# Create the DistanceMeasureFactory.so artifact
add_library(DistanceMeasureFactory SHARED adapter/cpp/DistanceMeasureFactory.cpp adapter/cpp/DistanceMeasureFactory.hpp)

if ("${ATA_TYPE}" MATCHES "auto")

    add_library(distance SHARED distance-measure/baseline_distance.cpp distance-measure/baseline_distance.hpp distance-measure/distance_measure.hpp)
    set_property(TARGET distance PROPERTY COMPILE_FLAGS ${CXXFLAGS_SSE2})

    target_compile_definitions(DistanceMeasureFactory PRIVATE _0)
    target_link_libraries(DistanceMeasureFactory distance)

elseif ("${ATA_TYPE}" MATCHES "128")

    add_library(distance SHARED distance-measure/__m128d__distance.cpp distance-measure/__m128d__distance.hpp distance-measure/distance_measure.hpp)
    set_property(TARGET distance PROPERTY COMPILE_FLAGS ${CXXFLAGS_SSE2})

    target_compile_definitions(DistanceMeasureFactory PRIVATE _128)
    target_link_libraries(DistanceMeasureFactory distance)

elseif ("${ATA_TYPE}" MATCHES "256")

    add_library(distance SHARED distance-measure/__m256d__distance.cpp distance-measure/__m256d__distance.hpp distance-measure/distance_measure.hpp)
    set_property(TARGET distance PROPERTY COMPILE_FLAGS ${CXXFLAGS_AVX2})

    target_compile_definitions(DistanceMeasureFactory PRIVATE _256)
    target_link_libraries(DistanceMeasureFactory distance)


elseif ("${ATA_TYPE}" MATCHES "512")

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # require at least gcc 7
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0)
            message(FATAL_ERROR "GCC version must be at least 7.0!")
        endif()
    endif()

    add_library(distance SHARED distance-measure/__m512d__distance.cpp distance-measure/__m512d__distance.hpp distance-measure/distance_measure.hpp)
    set_property(TARGET distance PROPERTY COMPILE_FLAGS ${CXXFLAGS_AVX5})

    target_compile_definitions(DistanceMeasureFactory PRIVATE _512)
    target_link_libraries(DistanceMeasureFactory distance)

else()

    message (FATAL_ERROR "DataType '${ATA_TYPE}' not recognized")

endif()


###################
### INTEGRATION ###
###################

# Create the DistanceMeasure.so artifact
add_library(PyDistance SHARED adapter/cpp/DistanceMeasureAdapter.cpp)

# Set the Python version to default in the OS
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

# Link the DistanceMeasure lib with the DistanceMeasureFactory lib
target_link_libraries(PyDistance DistanceMeasureFactory ${PYTHON_LIBRARIES})

###################
### DEBUG #########
###################

add_library(benchmark SHARED benchmark/benchmark.cpp benchmark/benchmark.hpp benchmark/StopWatch.cpp distance-measure/distance_measure.hpp)

add_executable(main debug/main.cpp)
target_link_libraries(main benchmark DistanceMeasureFactory)

# main.py
# Copy the Python's main file to the library directory
file(COPY debug/main.py DESTINATION ${PROJECT_BINARY_DIR}/lib)

###################
###################
###################

