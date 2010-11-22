#ifndef KUEKEN_CLEAR_INCLUDED
#define KUEKEN_CLEAR_INCLUDED

#include "buffer.hpp"
#include "rasterizer.hpp"

namespace kueken
{
	class renderer;
}//namespace kueken

namespace kueken{
namespace clear{

	enum colorbuffer
	{
		COLORBUFFER,
		COLORBUFFER0 = COLORBUFFER,
		COLORBUFFER1,
		COLORBUFFER2,
		COLORBUFFER3,
		COLORBUFFER4,
		COLORBUFFER5,
		COLORBUFFER6,
		COLORBUFFER7,
		COLORBUFFER_MAX
	};

	enum depthbuffer
	{
		DEPTHBUFFER,
		DEPTHBUFFER_MAX
	};

	enum stencilbuffer
	{
		STENCILBUFFER,
		STENCILBUFFER_MAX
	};

	enum drawbuffer
	{
		COLORBUFFER0_BIT = (1 << COLORBUFFER0),
		COLORBUFFER1_BIT = (1 << COLORBUFFER1),
		COLORBUFFER2_BIT = (1 << COLORBUFFER2),
		COLORBUFFER3_BIT = (1 << COLORBUFFER3),
		COLORBUFFER4_BIT = (1 << COLORBUFFER4),
		COLORBUFFER5_BIT = (1 << COLORBUFFER5),
		COLORBUFFER6_BIT = (1 << COLORBUFFER6),
		COLORBUFFER7_BIT = (1 << COLORBUFFER7),
		DEPTHBUFFER_BIT = (1 << (COLORBUFFER_MAX + DEPTHBUFFER)),
		STENCILBUFFER_BIT = (1 << (COLORBUFFER_MAX + DEPTHBUFFER_MAX + STENCILBUFFER))
	};

namespace detail
{
	struct data
	{
		data();

		std::array<glm::vec4, COLORBUFFER_MAX> Color;
		GLfloat Depth;
		GLint Stencil;
		glm::uint32 Drawbuffers;
	};

}//namespace detail

	enum mask
	{
		COLOR,
		DEPTH,
		STENCIL,
		MASK_MAX
	};

	class creator : public kueken::detail::creator
	{
		friend class object;

	public:
		creator(renderer & Renderer);
		void setColor(colorbuffer const & Colorbuffer, glm::vec4 const & Color);
		void setDepth(float Depth);
		void setStencil(int Stencil);

		virtual bool validate();

	private:
		detail::data Data;
	};

	class object
	{
	public:
		object(creator const & Creator);
		~object(){}

		void exec();

	private:
		detail::data Data;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace clear
}//namespace kueken

#include "clear.inl"

#endif//KUEKEN_CLEAR_INCLUDED
