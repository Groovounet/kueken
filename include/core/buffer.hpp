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

	enum stream
	{
		STREAM,
		STREAM0 = STREAM,
		STREAM1,
		STREAM2,
		STREAM3,
		STREAM4,
		STREAM5,
		STREAM6,
		STREAM7,
		STREAM_MAX
	};

	enum access
	{
		READ,
		WRITE,
		INVALIDATE_RANGE,
		INVALIDATE_BUFFER,
		FLUSH_EXPLICIT,
		UNSYNCHRONIZED,
		ACCESS_MAX
	};

	enum
	{
		READ_BIT = (1 << READ),
		WRITE_BIT = (1 << WRITE),
		INVALIDATE_RANGE_BIT = (1 << INVALIDATE_RANGE),
		INVALIDATE_BUFFER_BIT = (1 << INVALIDATE_BUFFER),
		FLUSH_EXPLICIT_BIT = (1 << FLUSH_EXPLICIT),
		UNSYNCHRONIZED_BIT = (1 << UNSYNCHRONIZED),
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
			mapping() :
				Pointer(nullptr),
				Offset(0),
				Length(0),
				BitField(0)
			{}

			GLvoid* Pointer;
			std::size_t Offset;
			std::size_t Length;
			glm::uint32 BitField;
		};

	public:
		object(creator const & Creator);
		~object();

		void bind(target const & Target);
		stream map(
			std::size_t const & Offset, 
			std::size_t const & Length,
			glm::uint32 const & Access);
		void unmap();
		void flush(
			std::size_t const & Offset, 
			std::size_t const & Length);
		void set(
			std::size_t Offset, 
			std::size_t Size, 
			void const * const Data);
		void set(
			stream const & Stream,
			std::size_t Offset, 
			std::size_t Size, 
			void const * const Data);
		void const * const get(
			stream const & Stream);
		void copy(
			object const & ObjectSrc, 
			std::size_t const & OffsetSrc, 
			std::size_t const & OffsetDst, 
			std::size_t const & Size);
		GLuint GetName() const;

	private:
		detail::data Data;
		GLuint Name;
		std::array<mapping, STREAM_MAX> Mappings;
		std::size_t PointerIndex;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace buffer
}//namespace kueken

#include "buffer.inl"

#endif//KUEKEN_BUFFER_INCLUDED
