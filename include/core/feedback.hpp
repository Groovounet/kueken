#ifndef KUEKEN_FEEDBACK_INCLUDED
#define KUEKEN_FEEDBACK_INCLUDED

#include "detail/detail.hpp"

namespace kueken
{
	class renderer;
}//namespace kueken

namespace kueken{
namespace feedback{
namespace detail
{
	struct data
	{
		data();

		GLuint Variable;
		GLuint Buffer;
		GLintptr Offset;
		GLsizeiptr Size;
	};

}//namespace detail

	class creator// : public kueken::detail::creator
	{
		friend class object;

	public:
		creator(renderer & Renderer);
		virtual bool validate(){assert(0); return false;}

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
		detail::data Data;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace feedback
}//namespace kueken

#include "feedback.inl"

#endif//KUEKEN_FEEDBACK_INCLUDED
