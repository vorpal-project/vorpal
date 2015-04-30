
# Specify cmake version - make sure we have the needed features
cmake_minimum_required (VERSION 2.8)

# Define the project name and language
set (PROJECT_NAME oda)
project (${PROJECT_NAME} C CXX)

# Find the OpenAL package
# This sets the variables OPENAL_INCLUDE_DIR and OPENAL_LIBRARY
find_package (OpenAL REQUIRED)

# Define source code files
set (PROJECT_SRC
  ${CMAKE_SOURCE_DIR}/src/oda/oda.cxx
  ${CMAKE_SOURCE_DIR}/src/oda/system.cxx
)

# Define source code headers
set (PROJECT_HEADERS
  ${CMAKE_SOURCE_DIR}/src/oda/oda.h
  ${CMAKE_SOURCE_DIR}/src/oda/system.h
  ${CMAKE_SOURCE_DIR}/src/oda/status.h
)

# Where to look for when using #include <...>
include_directories (${OPENAL_INCLUDE_DIR})
include_directories (${CMAKE_SOURCE_DIR}/src)
include_directories (${CMAKE_BINARY_DIR}/externals/include)

# Define our build target as a shared lib
add_library (${PROJECT_NAME} SHARED ${PROJECT_SRC} ${PROJECT_HEADERS})

# Link with OpenAL
target_link_libraries (
  ${PROJECT_NAME} LINK_PUBLIC
  ${OPENAL_LIBRARY}
  ${CMAKE_BINARY_DIR}/externals/lib/libpd.so
)

# Set a few properties:
#  - Use C++11
#  - Output the lib file to <build directory>/lib
set_target_properties (
  ${PROJECT_NAME} PROPERTIES
  COMPILE_FLAGS "-std=c++11"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Build libpd as a dependency of liboda
add_custom_command (
  TARGET ${PROJECT_NAME}
  PRE_BUILD
  #OUTPUT ${LIBPD_INSTALLED_HEADERS} ${LIBPD_INSTALLED_LIB}
  COMMAND git submodule update --init
  COMMAND make libpd
  COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/externals/lib
  COMMAND make install prefix=${CMAKE_BINARY_DIR}/externals
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/externals/libpd
  COMMENT "Building libpd"
)

# Define a exporting name
set (PROJECT_EXPORT "lib${PROJECT_NAME}")
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
         DESTINATION include/oda
         CONFIGURATIONS Debug Release)
# Install libpd
install (FILES ${CMAKE_BINARY_DIR}/externals/lib/libpd.so
         DESTINATION lib
         CONFIGURATIONS Debug Release)
# Export cmake target
install (EXPORT ${PROJECT_EXPORT}
         DESTINATION cmake)

