
# Specify cmake version - make sure we have the needed features
cmake_minimum_required (VERSION 2.8)

# Define the project name and language
set (PROJECT_NAME oda)
project (${PROJECT_NAME} C CXX)

# Add a CMake module path for custom modules
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

set (CMAKE_C_FLAGS_DEBUG "-O0 -g -Wall")
set (CMAKE_C_FLAGS_RELEASE "-O3 -Wall")

# Where to look for when using #include <...>
include_directories (${OPENAL_INCLUDE_DIR})
include_directories (${CMAKE_SOURCE_DIR}/src)
include_directories (${CMAKE_BINARY_DIR}/externals/include)

link_directories (${CMAKE_BINARY_DIR}/externals/lib)

# Define a exporting name
set (PROJECT_EXPORT "lib${PROJECT_NAME}")

add_subdirectory (externals)
add_subdirectory (src)

# Install ODA Panel patches
install (FILES
         ${CMAKE_SOURCE_DIR}/patches/openda_core.pd
         ${CMAKE_SOURCE_DIR}/patches/openda_panel.pd
         ${CMAKE_SOURCE_DIR}/patches/openda_bus.pd
         ${CMAKE_SOURCE_DIR}/patches/openda_parameter.pd
         ${CMAKE_SOURCE_DIR}/patches/openda_seq16.pd
         ${CMAKE_SOURCE_DIR}/patches/openda_metro.pd
         ${CMAKE_SOURCE_DIR}/patches/openda_sample.pd
         ${CMAKE_SOURCE_DIR}/patches/openda_osc.pd
         ${CMAKE_SOURCE_DIR}/patches/openda_tri.pd
         DESTINATION patches
         CONFIGURATIONS Debug Release)

