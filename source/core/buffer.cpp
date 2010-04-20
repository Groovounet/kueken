#include <core/buffer.hpp>

namespace
{
	GLenum buffer_usage_cast(kueken::buffer::usage Usage)
	{
		static GLenum const CastBufferUsage[kueken::buffer::USAGE_MAX] = 
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

		return CastBufferUsage[Usage];
	}

	GLenum buffer_target_cast(kueken::buffer::target Target)
	{
		static GLenum const CastBufferTarget[kueken::buffer::TARGET_MAX] =
		{
			GL_ARRAY_BUFFER,			// ARRAY
			GL_ELEMENT_ARRAY_BUFFER,	// ELEMENT
			GL_UNIFORM_BUFFER_EXT,		// UNIFORM
			GL_TEXTURE_BUFFER_EXT,		// TEXTURE
			GL_PIXEL_PACK_BUFFER,		// PIXEL_PACK 
			GL_PIXEL_UNPACK_BUFFER,		// PIXEL_UNPACK
			GL_TRANSFORM_FEEDBACK_BUFFER_EXT// FEEDBACK
		};

		return CastBufferTarget[Target];
	}

}//namespace

namespace kueken{
namespace buffer
{
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

	void object::bind(target Target)
	{
		glBindBuffer(buffer_target_cast(Target), Name);
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
