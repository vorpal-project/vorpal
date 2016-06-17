
# Find the Lua package
# This sets the variables LUA_INCLUDE_DIR and LUA_LIBRARIES
find_package (Lua51 REQUIRED)

include_directories (${LUA_INCLUDE_DIR})

add_library(oda_lua SHARED EXCLUDE_FROM_ALL oda_lua.cxx)

target_link_libraries(
  oda_lua
  PUBLIC
  ${PROJECT_NAME}
  ${LUA_LIBRARIES}
)

# Set a few properties
#   - Linking flag
#   - Remove "lib"prefix
#   - Set output directory
set_target_properties (
  oda_lua PROPERTIES
  COMPILE_FLAGS "-W -Wall -fPIC -std=c++11"
  PREFIX ""
  OUTPUT_NAME "oda"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Install the library target
#install (TARGETS oda_lua
#         DESTINATION lua
#         CONFIGURATIONS Debug Release)
install (FILES ${CMAKE_BINARY_DIR}/lib/oda.so
         DESTINATION lua
         CONFIGURATIONS Debug Release)

