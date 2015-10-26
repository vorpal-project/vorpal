
# Specify cmake version - make sure we have the needed features
cmake_minimum_required (VERSION 2.8)

# Define the project name and language
set (PROJECT_NAME oda)
project (${PROJECT_NAME} C CXX)

# Add a CMake module path for custom modules
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

set (CMAKE_C_FLAGS_DEBUG "-O0 -g -Wall")
set (CMAKE_C_FLAGS_RELEASE "-O3 -Wall")

# Find the OpenAL package
# This sets the variables OPENAL_INCLUDE_DIR and OPENAL_LIBRARY
find_package (OpenAL REQUIRED)

# Define source code files
set (PROJECT_SRC
  ${CMAKE_SOURCE_DIR}/src/oda/oda.cxx
  ${CMAKE_SOURCE_DIR}/src/oda/engine.cxx
  ${CMAKE_SOURCE_DIR}/src/oda/event.cxx
  ${CMAKE_SOURCE_DIR}/src/oda/player.cxx
  ${CMAKE_SOURCE_DIR}/src/oda/dspserver.cxx
  ${CMAKE_SOURCE_DIR}/src/oda/command.cxx
)

# Define source code headers
set (PROJECT_HEADERS
  ${CMAKE_SOURCE_DIR}/src/oda/oda.h
  ${CMAKE_SOURCE_DIR}/src/oda/engine.h
  ${CMAKE_SOURCE_DIR}/src/oda/event.h
  ${CMAKE_SOURCE_DIR}/src/oda/status.h
  ${CMAKE_SOURCE_DIR}/src/oda/player.h
  ${CMAKE_SOURCE_DIR}/src/oda/dspserver.h
  ${CMAKE_SOURCE_DIR}/src/oda/command.h
)

# Where to look for when using #include <...>
include_directories (${OPENAL_INCLUDE_DIR})
include_directories (${CMAKE_SOURCE_DIR}/src)
include_directories (${CMAKE_BINARY_DIR}/externals/include)

link_directories (${CMAKE_BINARY_DIR}/externals/lib)

# Define a exporting name
set (PROJECT_EXPORT "lib${PROJECT_NAME}")

add_subdirectory (externals)

# Define our build target as a shared lib
add_library (${PROJECT_NAME} STATIC ${PROJECT_SRC} ${PROJECT_HEADERS})

# Link with OpenAL + libpd
target_link_libraries (
  ${PROJECT_NAME} PUBLIC
  ${OPENAL_LIBRARY}
  pdcpp
)

# Set a few properties:
#  - Use C++11
#  - Output the lib file to <build directory>/lib
set_target_properties (
  ${PROJECT_NAME} PROPERTIES
  COMPILE_FLAGS "-std=c++11 -fPIC"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Is there a better way to do this?
# Install the library target, with export option
install (TARGETS ${PROJECT_NAME}
         EXPORT ${PROJECT_EXPORT}
         DESTINATION lib
         CONFIGURATIONS Debug Release
         INCLUDES DESTINATION include)
# Install ODA's headers
install (FILES ${PROJECT_HEADERS}
         DESTINATION include/oda
         CONFIGURATIONS Debug Release)
# Install libpd's headers
install (DIRECTORY ${CMAKE_BINARY_DIR}/externals/include/libpd
         DESTINATION include
         CONFIGURATIONS Debug Release)
# Install ODA Panel patches
install (FILES
         ${CMAKE_SOURCE_DIR}/patches/oda_panel.pd
         ${CMAKE_SOURCE_DIR}/patches/oda_sound.pd
         DESTINATION patches
         CONFIGURATIONS Debug Release)
# Export cmake target
install (EXPORT ${PROJECT_EXPORT}
         DESTINATION cmake)

add_subdirectory (src/lua)

