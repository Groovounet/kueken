#ifndef KUEKEN_DETAIL_COMMON_INCLUDED
#define KUEKEN_DETAIL_COMMON_INCLUDED

// OpenGL
#include <GL/glew.h>
#define GL_DOUBLE_MAT2x3                                   0x8F49
#define GL_DOUBLE_MAT2x4                                   0x8F4A
#define GL_DOUBLE_MAT3x2                                   0x8F4B
#define GL_DOUBLE_MAT3x4                                   0x8F4C
#define GL_DOUBLE_MAT4x2                                   0x8F4D
#define GL_DOUBLE_MAT4x3                                   0x8F4E

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/half_float.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLI
#include <gli/gli.hpp>
#include <gli/gtx/loader.hpp>

// STL
#include <cassert>
#include <stack>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <memory>
#include <array>

// Boost
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/array.hpp>
#include <boost/operators.hpp>
#include <boost/format.hpp>
#include <boost/noncopyable.hpp>

// TinyXML
#include <tinyxml.h>

#ifdef KUKEN_DLL
#define KUEKEN_LINK 
//__declspec(dllexport)
#else
#define KUEKEN_LINK 
//__declspec(dllimport)
#endif//KUKEN_DLL

typedef unsigned int flag;

#if(GLM_COMPILER != GLM_COMPILER_VC2010)
#define nullptr NULL
#endif//(GLM_COMPILER != GLM_COMPILER_VC2010)

#endif//KUEKEN_DETAIL_COMMON_INCLUDED
