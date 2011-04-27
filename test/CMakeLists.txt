get_target_property(UPLAYER_LIB uplayer LOCATION)

configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/playsounds.u.in"
	"${CMAKE_CURRENT_BINARY_DIR}/playsounds.u" @ONLY)

add_custom_target(test_uplayer
	${URBI_EXECUTABLE} --file=${CMAKE_CURRENT_BINARY_DIR}/playsounds.u
	DEPENDS uplayer)

