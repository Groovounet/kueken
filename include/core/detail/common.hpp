#ifndef KUEKEN_DETAIL_COMMON_INCLUDED
#define KUEKEN_DETAIL_COMMON_INCLUDED

// OpenGL
#define GLEW_STATIC
#include <GL/glew.h>

// GLM
#define GLM_GTX_INCLUDED
#include <glm/glm.hpp>
#include <glm/gtx/matrix_projection.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/raw_data.hpp>
#include <glm/gtx/number_precision.hpp>
#include <glm/gtc/type_ptr.hpp>

// STL
#include <cassert>
#include <stack>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <memory>

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

#endif//KUEKEN_DETAIL_COMMON_INCLUDED
