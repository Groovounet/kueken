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
	if(MSVC)
		add_definitions(/MP${Count})
	endif(MSVC)
endmacro(add_parallel_build_GTC)

##########################
# Project

project(kueken)

add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
if(NOT MSVC)
	add_definitions(-std=gnu++0x)
endif(NOT MSVC)

##########################
# Setup binary directory

if(MSVC)
	if(MSVC_VERSION EQUAL 1600)
		set(BINARY_DIRECTORY "win32-vc2010")
	endif(MSVC_VERSION EQUAL 1600)
else(MSVC)
	set(BINARY_DIRECTORY "win32-vc2008")
endif(MSVC)

set(GLF_LIBRARY_BINARY_FILE_SDL "tinyxml")
set(GLF_LIBRARY_BINARY_FILE_OGL "opengl32" "glew32s" "freeglut_static")
set(GLF_LIBRARY_BINARY_FILE ${GLF_LIBRARY_BINARY_FILE_OGL})

############
# Boost 

set(BOOST_VERSION "boost-1.43.0") 
set(BOOST_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/${BOOST_VERSION})

include_directories("${BOOST_DIRECTORY}")
link_directories("${BOOST_DIRECTORY}/binary/${BINARY_DIRECTORY}")

############
# TinyXML 

set(TINYXML_VERSION "tinyxml-2.6.1") 
set(TINYXML_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/${TINYXML_VERSION})

include_directories("${TINYXML_DIRECTORY}/include")
link_directories("${TINYXML_DIRECTORY}/library/${BINARY_DIRECTORY}")

############
# Add GLUT 

set(GLUT_VERSION "freeglut-2.6.0")
set(GLUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/${GLUT_VERSION})

add_definitions(-DFREEGLUT_STATIC)
include_directories("${GLUT_DIRECTORY}")
link_directories("${GLUT_DIRECTORY}/lib/${BINARY_DIRECTORY}")

############
# Add GLEW 

set(GLEW_VERSION "glew-1.5.6")
set(GLEW_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/${GLEW_VERSION})

add_definitions(-DGLEW_STATIC)
include_directories(${GLEW_DIRECTORY}/include)
link_directories(${GLEW_DIRECTORY}/lib/${BINARY_DIRECTORY})

############
# Add GLEXT 

set(GL_VERSION "GL") 
set(GL_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/${GL_VERSION})

###########
# Add GLF 

set(GLF_VERSION "glf")
set(GLF_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/${GLF_VERSION})

include_directories("${GLF_DIRECTORY}")

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

set(KUEKEN_TEST_DATA DATA_COPY)
add_custom_target(
	${KUEKEN_TEST_DATA}
	COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/sample/data ${CMAKE_CURRENT_BINARY_DIR}/sample/data)

###########

set(KUEKEN_CORE_NAME kueken-core)
set(KUEKEN_LOAD_NAME kueken-load)
set(KUEKEN_IO_NAME kueken-io)
set(KUEKEN_FX_NAME kueken-fx)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(source)
add_subdirectory(test)

