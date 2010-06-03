cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_policy(VERSION 2.6)

##########################
# Macros

macro(add_group_target_GTC Target)
	add_custom_target( 
		${Target} ALL
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Group target..." )
endmacro(add_group_target_GTC)

macro(add_parallel_build_GTC Count)
	add_definitions(/MP${Count})
endmacro(add_parallel_build_GTC)

macro(add_sample_target_GTC SampleName GroupName DataName SourceFiles)

	add_executable(${SampleName} ${SourceFiles})
	target_link_libraries(${SampleName} ${GLF_LIBRARY_BINARY_FILE})

	source_group("Shader Files" FILES "*.vert" "*.geom" "*.frag")

	add_dependencies(${SampleName} ${KUEKEN_CORE_NAME})
	add_dependencies(${GroupName} ${SampleName})
	
	copy_data_GTC(${DataName} "${CMAKE_CURRENT_SOURCE_DIR}/../data" "${CMAKE_CURRENT_BINARY_DIR}/../data")
	copy_binary_GTC(${SampleName})	
	
endmacro(add_sample_target_GTC)

##########################
# Project

project(kueken)

add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)

##########################
# Setup binary directory

if(MSVC)
	if(MSVC_VERSION EQUAL 1400)
		set(BINARY_DIRECTORY "win32-vc2005")
	endif(MSVC_VERSION EQUAL 1400)
	if(MSVC_VERSION EQUAL 1500)
		set(BINARY_DIRECTORY "win32-vc2008")
	endif(MSVC_VERSION EQUAL 1500)	
	if(MSVC_VERSION EQUAL 1600)
		set(BINARY_DIRECTORY "win32-vc2010")
	endif(MSVC_VERSION EQUAL 1600)
else(MSVC)
	set(BINARY_DIRECTORY "unknowned")
endif(MSVC)

set(GLF_LIBRARY_BINARY_FILE_SDL "SDL" "SDLmain" "tinyxml")
set(GLF_LIBRARY_BINARY_FILE_OGL "glew32s" "OpenGL32" "glu32")
set(GLF_LIBRARY_BINARY_FILE ${GLF_LIBRARY_BINARY_FILE_SDL} ${GLF_LIBRARY_BINARY_FILE_OGL})

############
# Boost 

set(BOOST_VERSION "boost-1.43.0") 
set(BOOST_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/${BOOST_VERSION})

include_directories("${BOOST_DIRECTORY}")
link_directories("${BOOST_DIRECTORY}/binary/${BINARY_DIRECTORY}")

############
# TinyXML 

set(TINYXML_VERSION "tinyxml-2.5.3") 
set(TINYXML_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/${TINYXML_VERSION})

include_directories("${TINYXML_DIRECTORY}/include")
link_directories("${TINYXML_DIRECTORY}/lib/${BINARY_DIRECTORY}")

###########
# Add SDL 

set(SDL_VERSION "sdl-1.3.0-5206") 
set(SDL_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/${SDL_VERSION})

if(MSVC)
	link_directories("${SDL_DIRECTORY}/binary/${BINARY_DIRECTORY}")
	execute_process(COMMAND cmake -E copy ${SDL_DIRECTORY}/binary/${BINARY_DIRECTORY}/SDL.dll ${CMAKE_CURRENT_BINARY_DIR}/build/release/SDL.dll)
	execute_process(COMMAND cmake -E copy ${SDL_DIRECTORY}/binary/${BINARY_DIRECTORY}/SDL.dll ${CMAKE_CURRENT_BINARY_DIR}/build/debug/SDL.dll)
	execute_process(COMMAND cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/build/release)
	execute_process(COMMAND cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/build/debug)
else(MSVC)
	if(DEFINED CMAKE_BUILD_TYPE)

	else()
		set(CMAKE_BUILD_TYPE Release)
	endif()
	
	set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")

    execute_process(COMMAND sh ./autogen.sh	WORKING_DIRECTORY ${SDL_DIRECTORY})
    execute_process(COMMAND ./configure --prefix=${SDL_DIRECTORY} WORKING_DIRECTORY ${SDL_DIRECTORY})
    execute_process(COMMAND make WORKING_DIRECTORY ${SDL_DIRECTORY})
    execute_process(COMMAND make install WORKING_DIRECTORY ${SDL_DIRECTORY})
    link_directories("${SDL_DIRECTORY}/lib")
endif(MSVC)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/external/${SDL_VERSION}/include")

############
# Add GLEW 

set(GLEW_VERSION "glew-1.5.4") 
set(GLEW_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/${GLEW_VERSION})

if(MSVC)
	include_directories("${GLEW_DIRECTORY}/include")
	link_directories("${GLEW_DIRECTORY}/lib/${BINARY_DIRECTORY}")
endif(MSVC)

############
# Add GLEXT 

set(GL_VERSION "GL") 
set(GL_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/${GL_VERSION})

###########
# Add GLM 

set(GLM_VERSION "glm")
set(GLM_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/${GLM_VERSION})

include_directories("${GLM_DIRECTORY}")

###########
# Add GLV

set(GLV_VERSION "glv-0.1.0.0")
set(GLV_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/${GLV_VERSION})

include_directories("${GLV_DIRECTORY}")

###########
# Add GLI

set(GLI_VERSION "gli")
set(GLI_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/${GLI_VERSION})

include_directories("${GLI_DIRECTORY}")

################################
# Add custom target to copy all data

set(KUEKEN_SAMPLE_DATA DATA_COPY)
add_custom_target(
	${KUEKEN_SAMPLE_DATA}
	COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/sample/data ${CMAKE_CURRENT_BINARY_DIR}/sample/data)

###########

set(KUEKEN_CORE_NAME kueken-core)
set(KUEKEN_LOAD_NAME kueken-load)
set(KUEKEN_IO_NAME kueken-io)
set(KUEKEN_FX_NAME kueken-fx)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(source)
add_subdirectory(sample)

