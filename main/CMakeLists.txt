# 'main' component
idf_component_register(
#	INCLUDE_DIRS "include"
		
	SRCS "test_main.c"

	PRIV_REQUIRES unity
)

# We can append compiler options - only apply to this component.

#target_compile_options(${COMPONENT_LIB} PRIVATE -Werror)	# Unfortunately, this doesn't work.
target_compile_options(${COMPONENT_LIB} PRIVATE -fanalyzer)	# Run the static analyzer for this specific component
