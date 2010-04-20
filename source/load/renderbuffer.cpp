#include <core/renderbuffer.hpp>
#include <core/sampler.hpp>

namespace
{
	GLenum renderbuffer_format_cast(kueken::renderbuffer::format Format)
	{
		static GLenum const CastRenderbufferFormat[kueken::renderbuffer::FORMAT_MAX] =
		{
			GL_DEPTH_COMPONENT24,		// DEPTH24
			GL_RGBA						// RGBA
		};

		return CastRenderbufferFormat[Format];
	}

}//namespace

namespace kueken{
namespace renderbuffer{

	///////////////////////////////////
	// object

	creator::creator()
	{
		Data.Attachment = GL_NONE;
		Data.Format = GL_NONE;
		Data.Width = 0;
		Data.Height = 0;
		Data.Samples = 0;
	}

	void creator::setFormat(format const & Format)
	{
		Data.Format = renderbuffer_format_cast(Format);
	}

	void creator::setSize(glm::uvec2 const & Size)
	{
		Data.Width = GLsizei(Size.x);
		Data.Height = GLsizei(Size.y);
	}

	void creator::setSamples(glm::uint Samples)
	{
		Data.Samples = GLsizei(Samples);
	}

	///////////////////////////////////
	// object

	object::object(creator const & Creator) :
		Data(Creator.Data)
	{
		glGenRenderbuffersEXT(1, &Name);

		glNamedRenderbufferStorageMultisampleEXT(
			Name, 
			Data.Samples, 
			Data.Format, 
			Data.Width, 
			Data.Height);
	}

	object::~object()
	{
		glDeleteRenderbuffersEXT(1, &Name);
	}

	GLuint object::GetName() const
	{
		return Name;
	}

}//namespace renderbuffer
}//namespace kueken
