#include <core/blend.hpp>

namespace
{
	GLenum blend_equation_cast(kueken::blend::equation Equation)
	{
		static GLenum const Cast[] =
		{
			GL_FUNC_ADD,				// ADD
			GL_FUNC_SUBTRACT,			// SUB
			GL_FUNC_REVERSE_SUBTRACT,	// REV
			GL_MIN,						// MIN
			GL_MAX						// MAX
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::blend::EQUATION_MAX,
			"Cast array size mismatch");

		return Cast[Equation];
	}

	GLenum blend_function_cast(kueken::blend::function Function)
	{
		static GLenum const Cast[] =
		{
			GL_ZERO,						// ZERO
			GL_ONE,							// ONE
			GL_SRC_COLOR,					// SRC_COLOR
			GL_ONE_MINUS_SRC_COLOR,			// ONE_MINUS_SRC_COLOR
			GL_DST_COLOR,					// DST_COLOR
			GL_ONE_MINUS_DST_COLOR,			// ONE_MINUS_DST_COLOR
			GL_SRC_ALPHA,					// SRC_ALPHA
			GL_ONE_MINUS_SRC_ALPHA,			// ONE_MINUS_SRC_ALPHA
			GL_DST_ALPHA,					// DST_ALPHA
			GL_ONE_MINUS_DST_ALPHA,			// ONE_MINUS_DST_ALPHA
			GL_CONSTANT_COLOR,				// CONSTANT_COLOR
			GL_ONE_MINUS_CONSTANT_COLOR,	// ONE_MINUS_CONSTANT_COLOR
			GL_CONSTANT_ALPHA,				// CONSTANT_ALPHA
			GL_ONE_MINUS_CONSTANT_ALPHA,	// ONE_MINUS_CONSTANT_ALPHA
			GL_SRC_ALPHA_SATURATE,			// SRC_ALPHA_SATURATE
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::blend::FUNCTION_MAX,
			"Cast array size mismatch");

		return Cast[Function];
	}

}//namespace

namespace kueken{
namespace blend{
namespace detail{

	data::data() :
		AlphaToCoverage(false),
		ColorEquation(GL_FUNC_ADD),
		AlphaEquation(GL_FUNC_ADD),
		ColorSrc(GL_ONE),
		AlphaSrc(GL_ZERO),
		ColorDst(GL_ONE),
		AlphaDst(GL_ZERO),
		DepthMask(GL_TRUE),
		StencilMaskFront(0xFF),
		StencilMaskBack(0xFF)
	{
		for(std::size_t i = 0; i < SLOT_MAX; ++i)
		{
			RedMask[i] = GL_TRUE;
			GreenMask[i] = GL_TRUE;
			BlueMask[i] = GL_TRUE;
			AlphaMask[i] = GL_TRUE;
			Blend[i] = false;
		}
	}

}//namespace detail

creator::creator
(
	renderer & Renderer
)
{}

void creator::setSource(function Color, function Alpha)
{
	Data.ColorSrc = blend_function_cast(Color);
	Data.AlphaSrc = blend_function_cast(Alpha);
}

void creator::setDestination(function Color, function Alpha)
{
	Data.ColorDst = blend_function_cast(Color);
	Data.AlphaDst = blend_function_cast(Alpha);
}

void creator::setEquation(equation Color, equation Alpha)
{
	Data.ColorEquation = blend_equation_cast(Color);
	Data.AlphaEquation = blend_equation_cast(Alpha);
}

void creator::setColorMask(slot Slot, glm::bvec4 Color)
{
	Data.RedMask[Slot] = Color.r ? GL_TRUE : GL_FALSE;
	Data.GreenMask[Slot] = Color.g ? GL_TRUE : GL_FALSE;
	Data.BlueMask[Slot] = Color.b ? GL_TRUE : GL_FALSE;
	Data.AlphaMask[Slot] = Color.a ? GL_TRUE : GL_FALSE;
}

void creator::setDepthMask(bool Depth)
{
	Data.DepthMask = Depth;
}

void creator::setStencilMask(glm::uint Front, glm::uint Back)
{
	Data.StencilMaskFront = GLuint(Front);
	Data.StencilMaskBack = GLuint(Back);
}

void creator::setAlphaToCoverage(bool Enabled)
{
	Data.AlphaToCoverage = Enabled;
}

void creator::setBlend(slot Slot, bool Enabled)
{
	Data.Blend[Slot] = Enabled;
}

object::object(creator const & Creator) :
	Data(Creator.Data),
	Name(0)
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
	glStencilMaskSeparate(GL_FRONT, Data.StencilMaskFront);
	glStencilMaskSeparate(GL_BACK, Data.StencilMaskBack);
	glDepthMask(Data.DepthMask);

	assert(glGetError() == GL_NO_ERROR);

	for(GLuint i = 0; i < SLOT_MAX; ++i)
	{
		glColorMaski(i, 
			Data.RedMask[i], 
			Data.GreenMask[i], 
			Data.BlueMask[i], 
			Data.AlphaMask[i]);

		if(Data.Blend[i])
			glEnablei(GL_BLEND, i);
		else
			glDisablei(GL_BLEND, i);
	}

	//GL_SAMPLE_ALPHA_TO_ONE
	//GL_SAMPLE_COVERAGE

	if(Data.AlphaToCoverage)
		glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	else
		glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	glBlendFuncSeparate(
		Data.ColorSrc,
		Data.AlphaSrc,
		Data.ColorDst,
		Data.AlphaDst);

	glBlendEquationSeparate(
		Data.ColorEquation, 
		Data.AlphaEquation);
}

}//namespace blend
}//namespace kueken
