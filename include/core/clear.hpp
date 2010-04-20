#ifndef KUEKEN_CLEAR_INCLUDED
#define KUEKEN_CLEAR_INCLUDED

#include "buffer.hpp"
#include "rasterizer.hpp"

namespace kueken{
namespace clear{
namespace detail{

	struct data
	{
		data();

		GLbitfield BitField;
		GLclampf Red;
		GLclampf Green;
		GLclampf Blue;
		GLclampf Alpha;
		GLfloat Depth;
		GLint Stencil;
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
		void setColor(glm::vec4 const & Color);
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
