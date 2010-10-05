#ifndef KUEKEN_BLIT_INCLUDED
#define KUEKEN_BLIT_INCLUDED

#include "detail/detail.hpp"

namespace kueken
{
	class renderer;
}//namespace kueken

namespace kueken{
namespace blit
{
	enum filter
	{
		NEAREST,
		LINEAR,
		FILTER_MAX
	};

namespace detail
{
	struct data
	{
		data();

		glm::ivec4 SrcRect;
		glm::ivec4 DstRect;
		GLbitfield Mask;
		GLenum Filter;
	};
}//namespace detail

	class creator : public kueken::detail::creator
	{
		friend class object;

	public:
		creator(renderer & Renderer);
		void setRectSrc(glm::ivec4 const & SrcRect);
		void setRectDst(glm::ivec4 const & DstRect);
		void setMask(bool Color, bool Depth, bool Stencil);
		void setFilter(filter const & Filter);

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

#include "blit.inl"

#endif//KUEKEN_BLIT_INCLUDED
