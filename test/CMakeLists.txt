get_target_property(UPLAYER_LIB uplayer LOCATION)

configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/playsounds.urbi.in"
	"${CMAKE_CURRENT_BINARY_DIR}/playsounds.urbi" @ONLY)

add_custom_target(test_uplayer
	${URBI_EXECUTABLE} --file=${CMAKE_CURRENT_BINARY_DIR}/playsounds.urbi
	DEPENDS uplayer)

