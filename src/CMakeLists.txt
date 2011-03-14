find_package (SDL REQUIRED)

if(NOT ENV{URBI_ROOT})
    set(ENV{URBI_ROOT} /opt/urbi-sdk)
endif()
set (URBI_ROOT_DIR $ENV{URBI_ROOT} CACHE PATH "Path to the Urbi root directory.")
find_package (Urbi REQUIRED)

# Set BOOST_ROOT to URBI_ROOT_DIR to use Urbi Boost distribution
set (BOOST_ROOT ${URBI_ROOT_DIR})
find_package (Boost REQUIRED)

include_directories (${SDL_INCLUDE_DIR} ${URBI_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

add_library (urbisound SHARED UrbiSound.cpp)
target_link_libraries (urbisound ${URBI_LIBRARIES} ${SDL_LIBRARY} ${Boost_LIBRARIES})
