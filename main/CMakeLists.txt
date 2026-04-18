# 'main' component
if(BUILD_UNIT_TEST_APP)
	idf_component_register(
		INCLUDE_DIRS "include"
		
        SRCS "test_main.c"

		PRIV_REQUIRES unity
	)
else()
	idf_component_register(
		INCLUDE_DIRS "include"
		
        SRCS "main.c"

        REQUIRES

		PRIV_REQUIRES
			esp_event
		
        # Shouldn't necessary here, but for some reason it is...
		PRIV_REQUIRES unity

	)
	# Necessary due to circular dependency - see documentation on esp-idf build system
	set_property(TARGET ${COMPONENT_LIB} APPEND PROPERTY LINK_INTERFACE_MULTIPLICITY 3)

endif()

# Append compiler options, for this component only!

#target_compile_options(${COMPONENT_LIB} PRIVATE -Werror)	# Unfortunately, doesn't work
target_compile_options(${COMPONENT_LIB} PRIVATE -fanalyzer)	# Run the static analyzer for this specific component
