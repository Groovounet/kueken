#include <core/test.hpp>

namespace
{
	GLenum test_function_cast(kueken::test::function Function)
	{
		static GLenum const Cast[] =
		{
			GL_NEVER,						// NEVER
			GL_LESS,						// LESS
			GL_EQUAL,						// EQUAL
			GL_LEQUAL,						// LEQUAL
			GL_GREATER,						// GREATER
			GL_NOTEQUAL,					// NOTEQUAL
			GL_GEQUAL,						// GEQUAL
			GL_ALWAYS,						// ALWAYS
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::test::FUNCTION_MAX,
			"Cast array size mismatch");

		return Cast[Function];
	}

	GLenum test_action_cast(kueken::test::action Action)
	{
		static GLenum const Cast[] =
		{
			GL_KEEP,					// KEEP
			GL_ZERO,					// ZERO
			GL_REPLACE,					// REPLACE
			GL_INCR,					// INCR
			GL_INCR_WRAP,				// INCR_WRAP
			GL_DECR,					// DECR
			GL_DECR_WRAP,				// DECR_WRAP
			GL_INVERT,					// INVERT
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::test::ACTION_MAX,
			"Cast array size mismatch");

		return Cast[Action];
	}

}//namespace

namespace kueken{
namespace test{
namespace detail{

	data::data() :
		DepthTest(false),
		DepthFunc(GL_LESS),
		DepthNear(0),
		DepthFar(1)
	{
		StencilFunc[FRONT] = GL_ALWAYS;
		StencilRef[FRONT] = 0;
		StencilMask[FRONT] = 0xFF;
		StencilSFail[FRONT] = GL_KEEP;
		StencilDFail[FRONT] = GL_KEEP;
		StencilDPass[FRONT] = GL_KEEP;

		StencilFunc[BACK] = GL_ALWAYS;
		StencilRef[BACK] = 0;
		StencilMask[BACK] = 0xFF;
		StencilSFail[BACK] = GL_KEEP;
		StencilDFail[BACK] = GL_KEEP;
		StencilDPass[BACK] = GL_KEEP;
	}

}//namespace detail

creator::creator
(
	renderer & Renderer
)
{}

void creator::setDepthEnable(bool Enabled)
{
	Data.DepthTest = Enabled;
}

void creator::setDepthFunc(function Func)
{
	Data.DepthFunc = test_function_cast(Func);
}

void creator::setDepthRange(float Near, float Far)
{
	Data.DepthNear = Near;
	Data.DepthFar = Far;
}

void creator::setStencilEnable(bool Enabled)
{
	Data.StencilTest = Enabled;
}

void creator::setStencilFunc(face Face, function Func, int Ref, glm::uint Mask)
{
	Data.StencilFunc[Face] = test_function_cast(Func);
	Data.StencilRef[Face] = GLint(Ref);
	Data.StencilMask[Face] = GLuint(Mask);
}

void creator::setStencilOp(face Face, action sfail, action dpfail, action dppass)
{
	Data.StencilSFail[Face] = test_action_cast(sfail);
	Data.StencilDFail[Face] = test_action_cast(dpfail);
	Data.StencilDPass[Face] = test_action_cast(dppass);
}

object::object(creator const & Creator) :
	Data(Creator.Data)
{
#if KUEKEN_STATE_OBJECTS
	Name = glGenLists(1);

	glNewList(Name, GL_COMPILE);
		run();
	glEndList();
#endif//KUEKEN_STATE_OBJECTS
}

object::~object()
{
#if KUEKEN_STATE_OBJECTS
	glDeleteLists(Name, 1); 
#endif//KUEKEN_STATE_OBJECTS
}

void object::bind()
{
#if KUEKEN_STATE_OBJECTS
	glCallList(Name);
#else
	run();
#endif//KUEKEN_STATE_OBJECTS
}

void object::run()
{
	if(Data.DepthTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	assert(glGetError() == GL_NO_ERROR);

	glDepthFunc(Data.DepthFunc);
	glDepthRange(Data.DepthNear, Data.DepthFar);
	assert(glGetError() == GL_NO_ERROR);

	if(Data.StencilTest)
		glEnable(GL_STENCIL_TEST);
	else
		glDisable(GL_STENCIL_TEST);
	assert(glGetError() == GL_NO_ERROR);

	glStencilFuncSeparate(	
		GL_FRONT,
 		Data.StencilFunc[FRONT],
 		Data.StencilRef[FRONT],
 		Data.StencilMask[FRONT]);
	assert(glGetError() == GL_NO_ERROR);

	glStencilFuncSeparate(	
		GL_BACK,
 		Data.StencilFunc[BACK],
 		Data.StencilRef[BACK],
 		Data.StencilMask[BACK]);
	assert(glGetError() == GL_NO_ERROR);

	glStencilOpSeparate(
		GL_FRONT, 
		Data.StencilSFail[FRONT],
		Data.StencilDFail[FRONT],
		Data.StencilDPass[FRONT]);
	assert(glGetError() == GL_NO_ERROR);

	glStencilOpSeparate(
		GL_BACK, 
		Data.StencilSFail[BACK],
		Data.StencilDFail[BACK],
		Data.StencilDPass[BACK]);

	assert(glGetError() == GL_NO_ERROR);
}

}//namespace test
}//namespace kueken
