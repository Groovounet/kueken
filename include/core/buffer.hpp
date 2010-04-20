#ifndef KUEKEN_BUFFER_INCLUDED
#define KUEKEN_BUFFER_INCLUDED

#include "detail/detail.hpp"

namespace kueken{
namespace buffer{
namespace detail{

	struct data
	{
		data() :
			Size(0),
			Usage(GL_NONE),
			Pointer(0)
		{}

		GLsizeiptr Size;
		GLenum Usage;
		GLvoid const * Pointer;
	};

}//namespace detail

	enum mode
	{
		INTERLEAVED,
		SEPARATED,
		MODE_MAX
	};

	enum target
	{
		ARRAY,
		ELEMENT,
		UNIFORM,
		TEXTURE,
		PIXEL_PACK,
        PIXEL_UNPACK,
		FEEDBACK,
		TARGET_MAX
	};

	enum usage
	{
		STREAM_DRAW, 
		STREAM_READ, 
		STREAM_COPY, 
		STATIC_DRAW, 
		STATIC_READ, 
		STATIC_COPY, 
		DYNAMIC_DRAW, 
		DYNAMIC_READ, 
		DYNAMIC_COPY,
		USAGE_MAX
	};

	class creator// : public kueken::detail::creator
	{
		friend class object;

	public:
		void setUsage(usage Usage);
		void setSize(std::size_t Size);
		void setData(void const * const Pointer); // shared_array?

		virtual bool validate();

	private:
		detail::data Data;
	};

	class object
	{
	public:
		object(creator const & Creator);
		~object();

		void bind(target Target);
		//void* map();
		//void unmap();
		//void flush();
		void set(std::size_t Offset, std::size_t Size, void const * const Data);

		GLuint GetName() const;

	private:
		detail::data Data;
		GLuint Name;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace buffer
}//namespace kueken

#include "buffer.inl"

#endif//KUEKEN_BUFFER_INCLUDED
