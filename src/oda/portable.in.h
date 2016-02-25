
#cmakedefine _OSX @APPLE@

#ifdef _OSX
# define ODA_OPENAL_DIR(header) <OpenAL/header>
#else
# define ODA_OPENAL_DIR(header) <AL/header>
#endif

