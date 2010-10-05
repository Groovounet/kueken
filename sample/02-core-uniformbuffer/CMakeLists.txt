set(KUEKEN_SAMPLE_NAME kueken-sample02-uniformbuffer)

set(SHADER_FILES ../data/uniform-buffer.vert ../data/uniform-buffer.frag)

file(GLOB SOURCE 
	*.hpp
	*.cpp)

add_executable( 
	${KUEKEN_SAMPLE_NAME} 
	${SOURCE}
	${SHADER_FILES})

source_group("Shader Files" FILES ${SHADER_FILES})

target_link_libraries(${KUEKEN_SAMPLE_NAME}
	${KUEKEN_CORE_NAME} 
	${GLF_LIBRARY_BINARY_FILE})

add_dependencies(${KUEKEN_SAMPLE_NAME} ${KUEKEN_CORE_NAME})
add_dependencies(${KUEKEN_SAMPLE_NAME} ${KUEKEN_SAMPLE_DATA})

add_dependencies(${KUEKEN_SAMPLES_NAME} ${KUEKEN_SAMPLE_NAME})

