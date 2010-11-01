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
		Data(Creator.Data)
	{
		glGenBuffers(1, &Name);

		// Reserve buffer memory
		glNamedBufferDataEXT(
			Name, 
			Data.Size, 
			Data.Pointer, 
			Data.Usage);
	}

	object::~object()
	{
		glDeleteBuffers(1, &Name);
	}

	void object::bind(target const & Target)
	{
		if(Target >= UNIFORM && Target <= UNIFORM_MAX)
			glBindBufferBase(buffer_target_cast(Target), Target - UNIFORM, this->Name);
		else if(Target >= FEEDBACK && Target <= FEEDBACK_MAX)
			glBindBufferBase(buffer_target_cast(Target), Target - FEEDBACK, this->Name);
		else
			glBindBuffer(buffer_target_cast(Target), this->Name);
	}

	GLuint object::GetName() const
	{
		return Name;
	}

	//void* object::map()
	//{
	//	return glMapBuffer(GL_ARRAY_BUFFER);
	//}

	//void object::unmap()
	//{
	//	glUnmapBuffer(GL_ARRAY_BUFFER);
	//}

	//void object::flush()
	//{
	//	
	//}

	void object::set
	(
		std::size_t Offset, 
		std::size_t Size, 
		void const * const Pointer
	)
	{
		assert((Offset + Size) <= std::size_t(Data.Size));
		glNamedBufferSubDataEXT(Name, Offset, Size, Pointer);
	}

}//namespace buffer
}//namespace kueken
