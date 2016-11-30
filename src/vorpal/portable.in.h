
#cmakedefine VORPAL_OSX

#ifdef VORPAL_OSX
# define VORPAL_OPENAL_DIR(header) <OpenAL/header>
#else
# define VORPAL_OPENAL_DIR(header) <AL/header>
#endif
