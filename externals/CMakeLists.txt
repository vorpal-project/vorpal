
# Build libpd as a dependency of liboda
add_custom_target (
  libpd
  COMMAND git submodule update --init
  COMMAND make libpd
  COMMAND make cpplib UTIL=true EXTRA=true
  COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/externals/lib
  COMMAND make install prefix=${CMAKE_BINARY_DIR}/externals
  COMMAND cmake -E copy libs/libpdcpp.so ${CMAKE_BINARY_DIR}/externals/lib
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/externals/libpd
  COMMENT "Building libpd"
)

add_dependencies (${PROJECT_NAME} libpd)

### PD helper patches! #########################################################

# Where patches should look for PD headers
set (PD_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/externals/include)

## OGGREAD~

# Find Vorbis (and Ogg) package 
find_package (Vorbis)

# Define patch build target for oggread~
add_library (oggread~ SHARED pdogg/oggread~.c)

# Link with libvorbis and libogg
target_link_libraries (oggread~ ${VORBIS_LIBRARIES})

# Set a few properties
#   - Warning flags
#   - Output directory, prefix and suffix
#   - Include directories
set_target_properties (
  oggread~ PROPERTIES
  COMPILE_FLAGS "-Wall -W -g"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/patches
  PREFIX ""
  SUFFIX ".pd_linux"
  INCLUDE_DIRECTORIES "${PD_INCLUDE_DIR};${PD_INCLUDE_DIR}/pd"
)

# Install ODA Panel patches
install (FILES
         ${CMAKE_BINARY_DIR}/patches/oggread~.pd_linux
         ${CMAKE_SOURCE_DIR}/externals/pdogg/oggread~-help.pd
         DESTINATION patches
         CONFIGURATIONS Debug Release)

