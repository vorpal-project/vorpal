
cmake_minimum_required (VERSION 2.8)

set (PROJECT_NAME dga)
project (${PROJECT_NAME} C CXX)

find_package (OpenAL REQUIRED)

set (PROJECT_SRC src/dga.cxx)
set (PROJECT_HEADERS src/dga.h)

include_directories (${OPENAL_INCLUDE_DIR})
include_directories (src)

add_library (${PROJECT_NAME} SHARED ${PROJECT_SRC} ${PROJECT_HEADERS})

target_link_libraries (${PROJECT_NAME} LINK_PUBLIC ${OPENAL_LIBRARY})

set_target_properties (
  ${PROJECT_NAME} PROPERTIES
  COMPILE_FLAGS "-std=c++11"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

set (PROJECT_EXPORT "lib${PROJECT_NAME}")
# Is there a better way to do this?
install (TARGETS ${PROJECT_NAME}
         EXPORT ${PROJECT_EXPORT}
         DESTINATION lib
         CONFIGURATIONS Debug Release)
install (FILES ${PROJECT_HEADERS}
         DESTINATION include
         CONFIGURATIONS Debug Release)
install (EXPORT ${PROJECT_EXPORT}
         DESTINATION cmake)

