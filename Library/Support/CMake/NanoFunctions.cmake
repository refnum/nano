#==============================================================================
#		nano_target_source_make_groups : Create source groupings for IDEs.
#------------------------------------------------------------------------------
function(nano_target_source_make_groups theTarget)

	get_target_property(theSources "${theTarget}" SOURCES)

	foreach (theFile ${theSources})

		get_filename_component(thePath "${theFile}" ABSOLUTE)
		get_filename_component(thePath "${thePath}" PATH)

		if ("${theFile}" MATCHES "${PROJECT_BINARY_DIR}")
			file(RELATIVE_PATH "${thePath}" "${PROJECT_BINARY_DIR}" "${thePath}")
		else()
			file(RELATIVE_PATH "${thePath}" "${PROJECT_SOURCE_DIR}" "${thePath}")
		endif()

		string(REGEX REPLACE "/" "\\\\" winPath "${thePath}")
		source_group("${winPath}" REGULAR_EXPRESSION "${thePath}/[^/\\]+\\..*")

	endforeach()

endfunction()





#==============================================================================
#		nano_target_compile_set_defaults : Set the compiler's default options.
#------------------------------------------------------------------------------
function(nano_target_compile_set_defaults theTarget)

	# Features
	target_compile_features("${theTarget}" PUBLIC "cxx_std_17")
	target_compile_features("${theTarget}" PUBLIC "c_std_11")


	# Flags
	target_compile_options("${theTarget}" PRIVATE ${NN_COMPILER_FLAGS})
	target_compile_options("${theTarget}" PRIVATE ${NN_COMPILER_WARNINGS})
	target_link_options("${theTarget}"    PRIVATE ${NN_LINKER_FLAGS})

endfunction()





#==============================================================================
#		nano_target_compile_set_prefix : Set the compiler prefix header.
#------------------------------------------------------------------------------
function(nano_target_compile_set_prefix theTarget thePrefix)

	if (NN_COMPILER_MSVC)
		target_compile_options("${theTarget}" PRIVATE /FI"${thePrefix}")

	else()
		target_compile_options("${theTarget}" PRIVATE -include "${thePrefix}")
	endif()

endfunction()





#==============================================================================
#		nano_target_compile_disable_warnings : Disable compiler warnings.
#------------------------------------------------------------------------------
function(nano_target_compile_disable_warnings theTarget thePattern)

	get_target_property(theSources "${theTarget}" SOURCES)

	if (NN_COMPILER_MSVC)
		set(flagNoWarnings "/W0")
	else()
		set(flagNoWarnings "-w")
	endif()

	foreach (theFile ${theSources})

		if ("${theFile}" MATCHES "${thePattern}")
			SET_SOURCE_FILES_PROPERTIES("${theFile}" PROPERTIES COMPILE_FLAGS "${flagNoWarnings}")
		endif()

 	endforeach()

endfunction()





#==============================================================================
#		nano_target_build_library : Build a library target.
#------------------------------------------------------------------------------
function(nano_target_build_library theTarget)

	nano_target_compile_set_defaults(	"${theTarget}")
	nano_target_source_make_groups(		"${theTarget}")

endfunction()





#==============================================================================
#		nano_target_build_app : Build the current project as an app.
#------------------------------------------------------------------------------
function(nano_target_build_app theTarget)

	nano_target_compile_set_defaults(	"${theTarget}")
	nano_target_source_make_groups(		"${theTarget}")
	target_link_libraries(				"${theTarget}" PRIVATE "Nano")


	if (NN_TARGET_ANDROID)
		target_link_libraries("${theTarget}" PRIVATE "android")
		target_link_libraries("${theTarget}" PRIVATE "log")

	elseif (NN_TARGET_IOS)
		target_link_libraries("${theTarget}" PRIVATE "-framework CoreFoundation")
		target_link_libraries("${theTarget}" PRIVATE "-framework Foundation")
		target_link_libraries("${theTarget}" PRIVATE "-framework Security")

	elseif (NN_TARGET_LINUX)
		target_link_libraries("${theTarget}" PRIVATE "pthread")

	elseif (NN_TARGET_MACOS)
		target_link_libraries("${theTarget}" PRIVATE "-framework CoreFoundation")
		target_link_libraries("${theTarget}" PRIVATE "-framework Foundation")
		target_link_libraries("${theTarget}" PRIVATE "-framework Security")

	elseif (NN_TARGET_TVOS)
		target_link_libraries("${theTarget}" PRIVATE "-framework CoreFoundation")
		target_link_libraries("${theTarget}" PRIVATE "-framework Foundation")
		target_link_libraries("${theTarget}" PRIVATE "-framework Security")

	elseif (NN_TARGET_WINDOWS)
		target_link_libraries("${theTarget}" PRIVATE "bcrypt")

	endif()

endfunction()



