set(SAMPLES_NAME kueken-samples)
add_group_target_GTC(${SAMPLES_NAME})

function(glCreateSampleGTC NAME)
	set(SAMPLE_NAME kueken-sample-${NAME})
	set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)

	foreach(FILE ${GL_SHADER_GTC})
 		set(SHADER_PATH ${SHADER_PATH} ${SHADER_DIR}/${FILE})
	endforeach(FILE)

	source_group("Shader Files" FILES ${SHADER_PATH})
	add_executable(${SAMPLE_NAME} ${NAME}.cpp ${SHADER_PATH})
	target_link_libraries(${SAMPLE_NAME} ${KUEKEN_CORE_NAME} ${GLF_LIBRARY_BINARY_FILE})
	
	add_dependencies(${SAMPLE_NAME} ${KUEKEN_CORE_NAME})
	add_dependencies(${SAMPLE_NAME} ${KUEKEN_SAMPLE_DATA})
	add_dependencies(${SAMPLES_NAME} ${SAMPLE_NAME})
endfunction(glCreateSampleGTC)

set(GL_SHADER_GTC texture.vert texture.frag)
glCreateSampleGTC(01-core-basic)
set(GL_SHADER_GTC uniform-buffer.vert uniform-buffer.frag)
glCreateSampleGTC(02-core-uniformbuffer)
