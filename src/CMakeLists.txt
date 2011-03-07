find_package (SDL REQUIRED)
find_package (SDL_sound REQUIRED)

if(NOT ENV{URBI_ROOT})
    set(ENV{URBI_ROOT} /opt/urbi-sdk)
endif()
set (URBI_ROOT_DIR $ENV{URBI_ROOT} CACHE PATH "Path to the Urbi root directory.")
find_package (Urbi REQUIRED)

include_directories (${SDL_INCLUDE_DIR} ${SDL_SOUND_INCLUDE_DIR} ${URBI_INCLUDE_DIR})

add_library (urbisound SHARED UrbiSound.cpp)
target_link_libraries (urbisound ${URBI_LIBRARIES})

add_executable (sdlwave sdlwave.c)
target_link_libraries (sdlwave ${SDL_LIBRARY} ${SDL_SOUND_LIBRARY})

