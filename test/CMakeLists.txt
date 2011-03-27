enable_testing()

get_target_property(UPLAYER_LIB uplayer LOCATION)

configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/playsounds.urbi.in"
	"${CMAKE_CURRENT_BINARY_DIR}/playsounds.urbi" @ONLY)

add_test(NAME mytest
	COMMAND ${URBI_EXECUTABLE} --module=$<TARGET_FILE:uplayer> --file=${CMAKE_CURRENT_BINARY_DIR}/playsounds.urbi)
