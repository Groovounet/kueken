#include <core/buffer.hpp>

namespace
{
	GLenum buffer_usage_cast(kueken::buffer::usage Usage)
	{
		static GLenum const Cast[] = 
		{
			GL_STREAM_DRAW,		// STREAM_DRAW
			GL_STREAM_READ,		// STREAM_READ
			GL_STREAM_COPY,		// STREAM_COPY
			GL_STATIC_DRAW,		// STATIC_DRAW
			GL_STATIC_READ,		// STATIC_READ
			GL_STATIC_COPY,		// STATIC_COPY
			GL_DYNAMIC_DRAW,	// DYNAMIC_DRAW
			GL_DYNAMIC_READ,	// DYNAMIC_READ
			GL_DYNAMIC_COPY		// DYNAMIC_COPY
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::buffer::USAGE_MAX,
			"Cast array size mismatch");

		return Cast[Usage];
	}

	GLenum buffer_access_cast(glm::uint32 const & Access)
	{
		assert(!(
			Access & kueken::buffer::READ_BIT && 
			Access != kueken::buffer::READ_BIT));
		assert(!(
			(Access & kueken::buffer::WRITE_BIT) && 
			(Access & kueken::buffer::FLUSH_EXPLICIT_BIT) && 
			(Access != (kueken::buffer::WRITE_BIT | kueken::buffer::FLUSH_EXPLICIT_BIT))));
		assert(
			(Access & kueken::buffer::READ_BIT) || 
			(Access & kueken::buffer::WRITE_BIT));

		if(Access & kueken::buffer::READ_BIT)
			return GL_MAP_READ_BIT;

		GLbitfield Result = GL_MAP_WRITE_BIT;

		if(Access & kueken::buffer::INVALIDATE_RANGE_BIT)
			Result |= GL_MAP_INVALIDATE_RANGE_BIT;
		if(Access & kueken::buffer::INVALIDATE_BUFFER_BIT)
			Result |= GL_MAP_INVALIDATE_BUFFER_BIT;
		if(Access & kueken::buffer::FLUSH_EXPLICIT_BIT)
			Result |= GL_MAP_FLUSH_EXPLICIT_BIT;
		if(Access & kueken::buffer::UNSYNCHRONIZED_BIT)
			Result |= GL_MAP_UNSYNCHRONIZED_BIT;

		return Result;
	}

	GLenum buffer_target_cast(kueken::buffer::target Target)
	{
		static GLenum const Cast[] =
		{
			GL_TEXTURE_BUFFER,				// TEXTURE
			GL_PIXEL_PACK_BUFFER,			// PIXEL_PACK 
			GL_PIXEL_UNPACK_BUFFER,			// PIXEL_UNPACK
			GL_DRAW_INDIRECT_BUFFER,		// INDIRECT
			GL_ELEMENT_ARRAY_BUFFER,		// ELEMENT
			GL_ARRAY_BUFFER,				// ARRAY0
			GL_ARRAY_BUFFER,				// ARRAY1
			GL_ARRAY_BUFFER,				// ARRAY2
			GL_ARRAY_BUFFER,				// ARRAY3
			GL_ARRAY_BUFFER,				// ARRAY4
			GL_ARRAY_BUFFER,				// ARRAY5
			GL_ARRAY_BUFFER,				// ARRAY6
			GL_ARRAY_BUFFER,				// ARRAY7
			GL_ARRAY_BUFFER,				// ARRAY8
			GL_ARRAY_BUFFER,				// ARRAY9
			GL_ARRAY_BUFFER,				// ARRAY10
			GL_ARRAY_BUFFER,				// ARRAY11
			GL_ARRAY_BUFFER,				// ARRAY12
			GL_ARRAY_BUFFER,				// ARRAY13
			GL_ARRAY_BUFFER,				// ARRAY14
			GL_ARRAY_BUFFER,				// ARRAY15
			GL_UNIFORM_BUFFER,				// UNIFORM0
			GL_UNIFORM_BUFFER,				// UNIFORM1
			GL_UNIFORM_BUFFER,				// UNIFORM2
			GL_UNIFORM_BUFFER,				// UNIFORM3
			GL_UNIFORM_BUFFER,				// UNIFORM4
			GL_UNIFORM_BUFFER,				// UNIFORM5
			GL_UNIFORM_BUFFER,				// UNIFORM6
			GL_UNIFORM_BUFFER,				// UNIFORM7
			GL_TRANSFORM_FEEDBACK_BUFFER,	// FEEDBACK0
			GL_TRANSFORM_FEEDBACK_BUFFER,	// FEEDBACK1
			GL_TRANSFORM_FEEDBACK_BUFFER,	// FEEDBACK2
			GL_TRANSFORM_FEEDBACK_BUFFER,	// FEEDBACK3
			GL_TRANSFORM_FEEDBACK_BUFFER,	// FEEDBACK4
			GL_TRANSFORM_FEEDBACK_BUFFER,	// FEEDBACK5
			GL_TRANSFORM_FEEDBACK_BUFFER,	// FEEDBACK6
			GL_TRANSFORM_FEEDBACK_BUFFER	// FEEDBACK7
		};

		std::size_t const Size = sizeof(Cast) / sizeof(GLenum);
		static_assert(
			Size == kueken::buffer::TARGET_MAX,
			"Cast array size mismatch");

		return Cast[Target];
	}

}//namespace

namespace kueken{
namespace buffer
{
	creator::creator
	(
		renderer & Renderer
	)
	{}

	void creator::setUsage(usage Usage)
	{
		Data.Usage = buffer_usage_cast(Usage);
	}

	void creator::setSize(std::size_t Size)
	{
		Data.Size = Size;
	}

	void creator::setData(void const * const Pointer)
	{
		Data.Pointer = Pointer;
	}

	bool creator::validate()
	{
		return true;
	}

	object::object(creator const & Creator) :
		Data(Creator.Data),
		PointerIndex(0)
	{
		glGenBuffers(1, &this->Name);
		glNamedBufferDataEXT(
			this->Name, 
			this->Data.Size, 
			this->Data.Pointer, 
			this->Data.Usage);

		assert(glGetError() == GL_NO_ERROR);
	}

	object::~object()
	{
		glDeleteBuffers(1, &this->Name);
		assert(glGetError() == GL_NO_ERROR);
	}

	void object::bind(target const & Target)
	{
		if(Target >= UNIFORM && Target <= UNIFORM_MAX)
			glBindBufferBase(buffer_target_cast(Target), Target - UNIFORM, this->Name);
		else if(Target >= FEEDBACK && Target <= FEEDBACK_MAX)
			glBindBufferBase(buffer_target_cast(Target), Target - FEEDBACK, this->Name);
		else
			glBindBuffer(buffer_target_cast(Target), this->Name);
		assert(glGetError() == GL_NO_ERROR);
	}

	GLuint object::GetName() const
	{
		return this->Name;
	}

	stream object::map
	(
		std::size_t const & Offset, 
		std::size_t const & Length,
		glm::uint32 const & Access
	)
	{
		this->Mappings[PointerIndex].Offset = Offset;
		this->Mappings[PointerIndex].Length = Length;
		this->Mappings[PointerIndex].BitField = Access;
        this->Mappings[PointerIndex].Pointer = glMapNamedBufferRangeEXT(
			this->Name, Offset, Length, buffer_access_cast(Access));
		stream Result = stream(PointerIndex);
		++PointerIndex;
		
		assert(PointerIndex < STREAM_MAX);
		assert(glGetError() == GL_NO_ERROR);

		return Result;
	}

	void object::unmap()
	{
        GLboolean Result = glUnmapNamedBufferEXT(this->Name);
		for(std::size_t i = STREAM; i < STREAM_MAX; ++i)
			this->Mappings[i] = mapping();

		assert(Result == GL_TRUE);
		assert(glGetError() == GL_NO_ERROR);
	}

	void object::flush
	(
		std::size_t const & Offset, 
		std::size_t const & Length
	)
	{
		//assert(this->Mappings[Stream].BitField == WRITE_BIT | FLUSH_EXPLICIT_BIT);

		glFlushMappedNamedBufferRangeEXT(this->Name, Offset, Length);

		assert(glGetError() == GL_NO_ERROR);
	}

	void object::set
	(
		std::size_t Offset, 
		std::size_t Size, 
		void const * const Pointer
	)
	{
		assert((Size + Offset) <= std::size_t(this->Data.Size));

		glNamedBufferSubDataEXT(this->Name, Offset, Size, Pointer);

		assert(glGetError() == GL_NO_ERROR);
	}

	void object::set
	(
		stream const & Stream,
		std::size_t Offset, 
		std::size_t Size, 
		void const * const Pointer
	)
	{
		assert((Size + Offset) <= std::size_t(this->Mappings[Stream].Length));
		assert(!(this->Mappings[Stream].BitField & READ_BIT));
		
		memcpy(reinterpret_cast<glm::byte*>(this->Mappings[Stream].Pointer) + Offset, Pointer, Size);
	}

	void const * const object::get
	(
		stream const & Stream
	)
	{
		assert(this->Mappings[Stream].BitField == READ_BIT);

		return this->Mappings[Stream].Pointer;
	}

	void object::copy
	(
		object const & ObjectSrc,
		std::size_t const & OffsetSrc, 
		std::size_t const & OffsetDst, 
		std::size_t const & Size
	)
	{
		assert((OffsetSrc + Size) <= std::size_t(Data.Size) - OffsetDst);

		glNamedCopyBufferSubDataEXT(
			ObjectSrc.Name, this->Name, 
			GLintptr(OffsetSrc), GLintptr(OffsetDst), GLsizeiptr(Size));

		assert(glGetError() == GL_NO_ERROR);
	}

}//namespace buffer
}//namespace kueken
