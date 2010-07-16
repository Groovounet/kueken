#include <core/texture.hpp>
#include <core/manager.hpp>

namespace
{
	GLenum texture_format_cast(kueken::texture::format Format)
	{
		static std::size_t const Cast[kueken::texture::FORMAT_MAX] =
		{
			GL_R8,		//R8,
			GL_RG8,		//RG8,
			GL_RGBA8,	//RGBA8,

			GL_R16,		//R16,
			GL_RG16,	//RG16,
			GL_RGBA16,	//RGBA16,

			GL_R16F,	//R16F,
			GL_RG16F,	//RG16F,
			GL_RGBA16F,	//RGBA16F,

			GL_R32F,	//R32F,
			GL_RG32F,	//RG32F,
			GL_RGBA32F,	//RGBA32F,

			GL_R8I,		//R8I,
			GL_RG8I,	//RG8I,
			GL_RGBA8I,	//RGBA8I,

			GL_R16I,	//R16I,
			GL_RG16I,	//RG16I,
			GL_RGBA16I,	//RGBA16I,

			GL_R32I,	//R32I,
			GL_RG32I,	//RG32I,
			GL_RGBA32I,	//RGBA32I,

			GL_R8UI,	//R8UI,
			GL_RG8UI,	//RG8UI,
			GL_RGBA8UI,	//RGBA8UI,

			GL_R16UI,	//R16UI,
			GL_RG16UI,	//RG16UI,
			GL_RGBA16UI,//RGBA16UI,

			GL_R32UI,	//R32UI,
			GL_RG32UI,	//RG32UI,
			GL_RGBA32UI //RGBA32UI,
		};

		return Cast[Format];
	}
}//namespace

namespace kueken{
namespace texture{

	/////////////////////////////
	// creator<image>

	void creator<image>::setVariable(program::variable const & Variable)
	{
		assert(Variable.Type == program::SAMPLER);
		Data.Variable = Variable;
	}

	void creator<image>::setImage(image::name const & Image)
	{
		Data.Image = Image;
	}

	void creator<image>::setSampler(sampler::name const & Sampler)
	{
		Data.Sampler = Sampler;
	}

	/////////////////////////////
	// buffer

	void creator<buffer>::setVariable(program::variable const & Variable)
	{
		Data.Variable = Variable;
	}

	void creator<buffer>::setBuffer(buffer::name const & Buffer)
	{
		kueken::manager& Manager = kueken::manager::instance();

		Data.Buffer = Manager.Buffer.getObject(Buffer).GetName();
	}

	void creator<buffer>::setFormat(format const & Format)
	{
		Data.Format = texture_format_cast(Format);
	}

namespace detail
{
	/////////////////////////////
	// objectBuffer

	objectBuffer::objectBuffer
	(
		creator<buffer> const & Creator
	) :
		Data(Creator.Data)
	{
		// Crash with nVidia drivers 182.05
		//glGenTextures(1, &Name);
		//glTextureBufferEXT(
		//	Name, 
		//	GL_TEXTURE_BUFFER_EXT, 
		//	Data.Format,
		//	Data.Buffer);

		//GLuint Previous = 0;
		//glGetIntegerv(GL_TEXTURE_BUFFER_EXT, &Previous);

		//glGenTextures(1, &Name);
		//glActiveTexture(GL_TEXTURE0 + 0);

		//glBindTexture(
		//	GL_TEXTURE_BUFFER_EXT, 
		//	Name);

		//glTexBuffer(
		//	GL_TEXTURE_BUFFER, 
		//	Data.Format, 
		//	Data.Buffer);

		glGenTextures(1, &Name);
		glTextureBufferEXT(
			Name, 
			GL_TEXTURE_BUFFER, 
			Data.Format,
			Data.Buffer);

		assert(glGetError() == GL_NO_ERROR);
	}

	objectBuffer::~objectBuffer()
	{
		glDeleteTextures(1, &Name); 
	}

	void objectBuffer::bind(slot Slot)
	{
		Data.Variable.set(Slot);

		glBindMultiTextureEXT(
			GL_TEXTURE0 + GLenum(Slot),
			GL_TEXTURE_BUFFER_EXT,
			Name);
	}

	GLuint objectBuffer::getName() const
	{
		return Name;
	}

	/////////////////////////////
	// objectImage

	objectImage::objectImage
	(
		creator<image> const & Creator
	) :
		Data(Creator.Data)
	{
#		if KUEKEN_STATE_OBJECTS
			Name = glGenLists(1);

			glNewList(Name, GL_COMPILE);
				run();
			glEndList();
#		endif//KUEKEN_STATE_OBJECTS	
	}

	objectImage::~objectImage()
	{
#		if KUEKEN_STATE_OBJECTS
			glDeleteLists(Name, 1); 
#		endif//KUEKEN_STATE_OBJECTS
	}

	void objectImage::bind(slot Slot)
	{
		assert(glGetError() == GL_NO_ERROR);

		Data.Variable.set(Slot);

		assert(glGetError() == GL_NO_ERROR);

		glActiveTexture(GL_TEXTURE0 + GLenum(Slot));

		assert(glGetError() == GL_NO_ERROR);

#		if KUEKEN_STATE_OBJECTS
			glCallList(Name);
#		else
			run();
#		endif//KUEKEN_STATE_OBJECTS

		assert(glGetError() == GL_NO_ERROR);

		//glActiveTexture(GL_TEXTURE0);
	}

	void objectImage::run()
	{
		kueken::manager& Manager = kueken::manager::instance();

		//Manager.Image.getObject(Data.Image).bind(Data.Sampler);
	}

	GLuint objectImage::getName() const
	{
		return kueken::manager::instance().Image.getObject(Data.Image).GetName();
	}

}//namespace detail

}//namespace texture
}//namespace kueken

