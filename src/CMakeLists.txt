set(URBI_ROOT_DIR "/usr" CACHE PATH "Urbi root directory.")

find_package (SDL REQUIRED)
find_package (Urbi REQUIRED)
find_package (Boost REQUIRED)

include_directories (${SDL_INCLUDE_DIR} ${URBI_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

add_library (uplayer MODULE UPlayer.cpp)
target_link_libraries (uplayer ${URBI_LIBRARIES} ${SDL_LIBRARY})

set_target_properties (uplayer PROPERTIES
  VERSION 0.0.1
  SOVERSION 0.0.1)

install (TARGETS uplayer DESTINATION lib/gostai/uobjects COMPONENT libraries)
