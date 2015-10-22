
# Build libpd as a dependency of liboda
#add_custom_target (
#  libpd
#  COMMAND git submodule update --init
#  COMMAND make libpd
#  COMMAND make cpplib UTIL=true EXTRA=true
#  COMMAND cmake -E make_directory ${CMAKE_BINARY_DIR}/externals/lib
#  COMMAND make install prefix=${CMAKE_BINARY_DIR}/externals
#  COMMAND cmake -E copy libs/libpdcpp.so ${CMAKE_BINARY_DIR}/externals/lib
#  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/externals/libpd
#  COMMENT "Building libpd"
#)

###############################################################################

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/libpd_wrapper 
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/libpd_wrapper/util
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/cpp
)


# Define source code files
set (PD_SRC_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/d_arithmetic.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/d_array.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/d_ctl.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/d_dac.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/d_delay.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/d_fft.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/d_fft_fftsg.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/d_filter.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/d_global.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/d_math.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/d_misc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/d_osc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/d_resample.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/d_soundfile.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/d_ugen.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_all_guis.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_array.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_bang.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_canvas.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_editor.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_graph.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_guiconnect.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_hdial.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_hslider.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_io.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_mycanvas.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_numbox.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_readwrite.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_rtext.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_scalar.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_template.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_text.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_toggle.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_traversal.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_vdial.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_vslider.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/g_vumeter.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/m_atom.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/m_binbuf.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/m_class.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/m_conf.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/m_glob.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/m_memory.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/m_obj.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/m_pd.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/m_sched.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/s_audio.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/s_audio_dummy.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/s_file.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/s_inter.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/s_loader.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/s_main.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/s_path.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/s_print.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/s_utf8.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/x_acoustics.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/x_arithmetic.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/x_array.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/x_connective.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/x_gui.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/x_interface.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/x_list.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/x_midi.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/x_misc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/x_net.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/x_scalar.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/x_text.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/src/x_time.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/libpd_wrapper/s_libpdmidi.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/libpd_wrapper/x_libpdreceive.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/libpd_wrapper/z_hooks.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/libpd_wrapper/z_libpd.c
)

set (PD_EXTRA_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/extra/bob~/bob~.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/extra/bonk~/bonk~.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/extra/choice/choice.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/extra/expr~/vexp_fun.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/extra/expr~/vexp_if.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/extra/expr~/vexp.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/extra/fiddle~/fiddle~.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/extra/loop~/loop~.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/extra/lrshift~/lrshift~.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/extra/pique/pique.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/extra/sigmund~/sigmund~.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/pure-data/extra/stdout/stdout.c
)

SET (LIBPD_UTILS_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/libpd_wrapper/util/z_print_util.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/libpd_wrapper/util/z_queued.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/libpd_wrapper/util/ringbuffer.c
)

SET (LIBPD_CPP_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/cpp/PdBase.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/libpd/cpp/PdTypes.cpp
)

set (LIBPD_CFLAGS
     "-fPIC -DPD -DHAVE_UNISTD_H -DHAVE_LIBDL -DUSEAPI_DUMMY -Wl,-Bsymbolic")

#add_library(pd    ${PD_SRC_FILES} ${PD_EXTRA_FILES} ${LIBPD_UTILS_FILES})
add_library(pdcpp SHARED ${PD_SRC_FILES} ${PD_EXTRA_FILES} ${LIBPD_UTILS_FILES}
                         ${LIBPD_CPP_FILES})

target_link_libraries (pdcpp PUBLIC dl m pthread)

set_target_properties (
  pdcpp PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/externals/lib
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/externals/lib
  COMPILE_FLAGS ${LIBPD_CFLAGS}
)

###############################################################################

install (TARGETS pdcpp
         EXPORT ${PROJECT_EXPORT}
         DESTINATION lib
         CONFIGURATIONS Debug Release
         INCLUDES DESTINATION include)

### PD helper patches! #########################################################

# Where patches should look for PD headers
set (PD_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/externals/include)

## OGGREAD~

# Find Vorbis (and Ogg) package 
find_package (Vorbis)

# Define patch build target for oggread~
add_library (oggread SHARED pdogg/oggread~.c)

# Link with libvorbis and libogg
target_link_libraries (
  oggread 
  PUBLIC
  ${VORBIS_LIBRARIES}
  pdcpp
)

# Set a few properties
#   - Warning flags
#   - Output directory, prefix and suffix
#   - Include directories
set_target_properties (
  oggread PROPERTIES
  COMPILE_FLAGS "-Wall -W -fPIC"
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/patches
  PREFIX ""
  SUFFIX "~.pd_linux"
  INCLUDE_DIRECTORIES "${PD_INCLUDE_DIR};${PD_INCLUDE_DIR}/pd"
)

# Install ODA Panel patches
install (TARGETS oggread
         EXPORT ${PROJECT_EXPORT}
         DESTINATION patches
         CONFIGURATIONS Debug Release)
install (FILES
         ${CMAKE_BINARY_DIR}/patches/oggread~.pd_linux
         ${CMAKE_SOURCE_DIR}/externals/pdogg/oggread~-help.pd
         DESTINATION patches
         CONFIGURATIONS Debug Release)

