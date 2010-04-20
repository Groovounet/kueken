#ifndef KUEKEN_READ_PIXELS_INCLUDED
#define KUEKEN_READ_PIXELS_INCLUDED

#include "detail/detail.hpp"

namespace kueken{
namespace readpixels{

	enum format
	{
		RGBA8U,
		BGRA8U,
		FORMAT_MAX
	};

namespace detail{

	struct data
	{
		data();

		glm::uvec4 Rect;
		GLenum Format;
		GLenum Type;
	};

}//namespace detail

	class creator : public kueken::detail::creator
	{
		friend class object;

	public:
		void setRect(glm::uvec4 const & Rect);
		void setFormat(format const & Format);

		bool validate();

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

}//namespace draw
}//namespace kueken

//#include "readpixels.inl"

#endif//KUEKEN_READ_PIXELS_INCLUDED
