#ifndef KUEKEN_BUFFER_INCLUDED
#define KUEKEN_BUFFER_INCLUDED

#include "detail/detail.hpp"

namespace kueken
{
	class renderer;
}//namespace kueken

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
		TEXTURE,
		PIXEL_PACK,
		PIXEL_UNPACK,
		INDIRECT,
		ELEMENT,
		ARRAY,
		ARRAY0 = ARRAY,
		ARRAY1,
		ARRAY2,
		ARRAY3,
		ARRAY4,
		ARRAY5,
		ARRAY6,
		ARRAY7,
		ARRAY8,
		ARRAY9,
		ARRAY10,
		ARRAY11,
		ARRAY12,
		ARRAY13,
		ARRAY14,
		ARRAY15,
		ARRAY_MAX = ARRAY15,
		UNIFORM,
		UNIFORM0 = UNIFORM,
		UNIFORM1,
		UNIFORM2,
		UNIFORM3,
		UNIFORM4,
		UNIFORM5,
		UNIFORM6,
		UNIFORM7,
		UNIFORM_MAX = UNIFORM7,
		FEEDBACK,
		FEEDBACK0 = FEEDBACK,
		FEEDBACK1,
		FEEDBACK2,
		FEEDBACK3,
		FEEDBACK4,
		FEEDBACK5,
		FEEDBACK6,
		FEEDBACK7,
		FEEDBACK_MAX = FEEDBACK7,
		TARGET_MAX
	};

	typedef std::size_t slot;

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

	enum address
	{
		ADDRESS,
		ADDRESS0 = ADDRESS,
		ADDRESS1,
		ADDRESS2,
		ADDRESS3,
		ADDRESS4,
		ADDRESS5,
		ADDRESS6,
		ADDRESS7,
		ADDRESS_MAX
	};

	enum access
	{
		READ_BIT,
		WRITE_BIT,
		INVALIDATE_RANGE_BIT,
		INVALIDATE_BUFFER_BIT,
		FLUSH_EXPLICIT_BIT,
		UNSYNCHRONIZED_BIT,
		ACCESS_MAX
	};

	class creator// : public kueken::detail::creator
	{
		friend class object;

	public:
		creator(renderer & Renderer);
		void setUsage(usage Usage);
		void setSize(std::size_t Size);
		void setData(void const * const Pointer); // shared_array?

		virtual bool validate();

	private:
		detail::data Data;
	};

	class object
	{
		struct mapping
		{
			GLvoid* Pointer;
			std::size_t Offset;
			std::size_t Length;
		};

	public:
		object(creator const & Creator);
		~object();

		void bind(target const & Target);
		address map(
			std::size_t const & Offset, 
			std::size_t const & Length,
			access const & Access);
		void unmap();
		void flush(
			std::size_t const & Offset, 
			std::size_t const & Length);
		void set(
			std::size_t Offset, 
			std::size_t Size, 
			void const * const Data);
		void set(
			address const & Address,
			std::size_t Offset, 
			std::size_t Size, 
			void const * const Data);
		void copy(
			object const & ObjectSrc, 
			std::size_t const & OffsetSrc, 
			std::size_t const & OffsetDst, 
			std::size_t const & Size);

		GLuint GetName() const;

	private:
		detail::data Data;
		GLuint Name;
		std::array<mapping, ADDRESS_MAX> Mappings;
		std::size_t PointerIndex;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace buffer
}//namespace kueken

#include "buffer.inl"

#endif//KUEKEN_BUFFER_INCLUDED
