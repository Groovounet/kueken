#ifndef KUEKEN_RENDERBUFFER_INCLUDED
#define KUEKEN_RENDERBUFFER_INCLUDED

#include "detail/detail.hpp"

namespace kueken
{
	class renderer;
}//namespace kueken

namespace kueken{
namespace renderbuffer
{
	struct info
	{
		// GL_MAX_RENDERBUFFER_SIZE // ???
	};

	enum format
	{
		DEPTH24,
		RGBA,
		FORMAT_MAX
	};

	namespace detail
	{
		struct data
		{
			GLenum Attachment;
			GLenum Format;
			GLsizei Width; 
			GLsizei Height;
			GLsizei Samples;
		};
	}//namespace detail

	class creator : public kueken::detail::creator
	{
		friend class object;

	public:
		creator(renderer & Renderer);
		void setFormat(format const & Format);
		void setSize(glm::uvec2 const & Size);
		void setSamples(glm::uint Sampels);

		virtual bool validate(){assert(0); return false;}

	private:
		detail::data Data;
	};

	class object
	{
		friend class renderer;

	public:
		object(creator const & Creator);
		~object();

		GLuint GetName() const;

	private:
		detail::data Data;
		GLuint Name;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace renderbuffer
}//namespace kueken

#include "renderbuffer.inl"

#endif//KUEKEN_RENDERBUFFER_INCLUDED
