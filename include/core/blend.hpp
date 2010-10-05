#ifndef KUEKEN_BLEND_INCLUDED
#define KUEKEN_BLEND_INCLUDED

#include "detail/detail.hpp"

namespace kueken
{
	class renderer;
}//namespace kueken

namespace kueken{
namespace blend
{
	enum target
	{
		BLEND
	};

	enum slot
	{
		SLOT0,
		SLOT1,
		SLOT2,
		SLOT3,
		SLOT4,
		SLOT5,
		SLOT6,
		SLOT7,
		SLOT_MAX
	};

	enum equation
	{
		ADD,
		SUB,
		REV,
		MIN,
		MAX,
		EQUATION_MAX
	};

	enum function
	{
		ZERO,						// GL_ZERO
		ONE,						// GL_ONE
		SRC_COLOR,					// GL_SRC_COLOR
		ONE_MINUS_SRC_COLOR,		// GL_ONE_MINUS_SRC_COLOR
		DST_COLOR,					// GL_DST_COLOR
		ONE_MINUS_DST_COLOR,		// GL_ONE_MINUS_DST_COLOR
		SRC_ALPHA,					// GL_SRC_ALPHA
		ONE_MINUS_SRC_ALPHA,		// GL_ONE_MINUS_SRC_ALPHA
		DST_ALPHA,					// GL_DST_ALPHA
		ONE_MINUS_DST_ALPHA,		// GL_ONE_MINUS_DST_ALPHA
		CONSTANT_COLOR,				// GL_CONSTANT_COLOR
		ONE_MINUS_CONSTANT_COLOR,	// GL_ONE_MINUS_CONSTANT_COLOR
		CONSTANT_ALPHA,				// GL_CONSTANT_ALPHA
		ONE_MINUS_CONSTANT_ALPHA,	// GL_ONE_MINUS_CONSTANT_ALPHA
		SRC_ALPHA_SATURATE,			// GL_SRC_ALPHA_SATURATE
		FUNCTION_MAX
	};

namespace detail
{
	struct data
	{
		data();

		bool AlphaToCoverage;
		GLenum ColorEquation;
		GLenum AlphaEquation;
		GLenum ColorSrc;
		GLenum AlphaSrc;
		GLenum ColorDst;
		GLenum AlphaDst;
		GLboolean RedMask[SLOT_MAX];
		GLboolean GreenMask[SLOT_MAX];
		GLboolean BlueMask[SLOT_MAX];
		GLboolean AlphaMask[SLOT_MAX];
		GLboolean DepthMask;
		GLuint StencilMaskFront;
		GLuint StencilMaskBack;
		bool Blend[SLOT_MAX];
	};

}//namespace detail

	class creator : public kueken::detail::creator
	{
		friend class object;

	public:
		creator(renderer & Renderer);
		void setSource(function Color, function Alpha);
		void setDestination(function Color, function Alpha);
		void setEquation(equation Color, equation Alpha);
		void setColorMask(slot Slot, glm::bvec4 Color);
		void setDepthMask(bool Depth);
		void setStencilMask(glm::uint Front, glm::uint Back);
		void setAlphaToCoverage(bool Enabled);
		void setBlend(slot Slot, bool Enabled);

		virtual bool validate(){assert(0); return true;}

	private:
		detail::data Data;
	};

	class object
	{
	public:
		object(creator const & Creator);
		~object();

		void bind();

	private:
		void run();

		detail::data Data;
		GLuint Name;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace blend
}//namespace kueken

#include "blend.inl"

#endif//KUEKEN_BLEND_INCLUDED
