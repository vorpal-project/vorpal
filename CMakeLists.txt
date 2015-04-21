
cmake_minimum_required (VERSION 2.8)
project (dga C CXX)

#set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package (OpenAL REQUIRED)

include_directories (${OPENAL_INCLUDE_DIR})
include_directories (src)

add_library (dga SHARED src/dga.cxx)
target_link_libraries (dga LINK_PUBLIC ${OPENAL_LIBRARY})
set_target_properties (dga PROPERTIES COMPILE_FLAGS -std=c++11)

