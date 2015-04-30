
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

# Define our build target as a shared lib
add_library (${PROJECT_NAME} SHARED ${PROJECT_SRC} ${PROJECT_HEADERS})

# Link with OpenAL
target_link_libraries (${PROJECT_NAME} LINK_PUBLIC ${OPENAL_LIBRARY})

# Set a few properties:
#  - Use C++11
#  - Output the lib file to <build directory>/lib
set_target_properties (
  ${PROJECT_NAME} PROPERTIES
  COMPILE_FLAGS "-std=c++11"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
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
install (FILES ${PROJECT_HEADERS}
         DESTINATION include/oda
         CONFIGURATIONS Debug Release)
install (EXPORT ${PROJECT_EXPORT}
         DESTINATION cmake)

