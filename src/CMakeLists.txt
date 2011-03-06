find_package (SDL REQUIRED)
find_package (SDL_sound REQUIRED)

include_directories(${SDL_INCLUDE_DIR} ${SDL_SOUND_INCLUDE_DIR})

add_executable(sdlwave sdlwave.c)

target_link_libraries(sdlwave ${SDL_LIBRARY} ${SDL_SOUND_LIBRARY})
