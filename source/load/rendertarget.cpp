#include <core/rendertarget.hpp>
#include <core/manager.hpp>

namespace
{
	GLenum rendertarget_format_cast(kueken::rendertarget::target Target)
	{
		static GLenum const Cast[kueken::rendertarget::TARGET_MAX] =
		{
			GL_READ_FRAMEBUFFER_EXT,		// READ
			GL_DRAW_FRAMEBUFFER_EXT,		// DRAW
			GL_FRAMEBUFFER_EXT				// RENDERTARGET
		};

		return Cast[Target];
	}

	GLenum rendertarget_attachment_cast(kueken::rendertarget::slot Slot)
	{
		static GLenum const Cast[kueken::rendertarget::SLOT_MAX] =
		{
			GL_DEPTH_ATTACHMENT_EXT,	//DEPTH,
			GL_COLOR_ATTACHMENT0_EXT,	//COLOR0,
			GL_COLOR_ATTACHMENT1_EXT,	//COLOR1,
			GL_COLOR_ATTACHMENT2_EXT,	//COLOR2,
			GL_COLOR_ATTACHMENT3_EXT,	//COLOR3,
			GL_COLOR_ATTACHMENT4_EXT,	//COLOR4,
			GL_COLOR_ATTACHMENT5_EXT,	//COLOR5,
			GL_COLOR_ATTACHMENT6_EXT,	//COLOR6,
			GL_COLOR_ATTACHMENT7_EXT	//COLOR7
		};

		return Cast[Slot];
	}

}//namespace

namespace kueken{
namespace rendertarget
{
	enum format
	{
		DEPTH24,
		STENCIL
	};

	////////////////////////////////////
	// creator<FRAMEBUFFER>

	creator<FRAMEBUFFER>::creator()
	{
		Data.Slot[COLOR0].Type = detail::FRAMEBUFFER;
		Data.Slot[COLOR0].Renderbuffer = renderbuffer::name::Null();
		Data.Slot[COLOR0].Image = image::name::Null();
		Data.Slot[COLOR0].Attachment = rendertarget_attachment_cast(COLOR0);
	}

	////////////////////////////////////
	// creator<CUSTOM>

	void creator<CUSTOM>::setImage
	(
		slot const & Slot, 
		image::name const & Image,
		glm::uint Level
	)
	{
		Data.Slot[Slot].Type = detail::IMAGE;
		Data.Slot[Slot].Renderbuffer = renderbuffer::name::Null();
		Data.Slot[Slot].Image = Image;
		Data.Slot[Slot].Level = Level;
		Data.Slot[Slot].Attachment = rendertarget_attachment_cast(Slot);
	}

	void creator<CUSTOM>::setRenderbuffer
	(
		slot const & Slot, 
		renderbuffer::name const & Renderbuffer
	)
	{
		Data.Slot[Slot].Type = detail::RENDERBUFFER;
		Data.Slot[Slot].Renderbuffer = Renderbuffer;
		Data.Slot[Slot].Image = image::name::Null();
		Data.Slot[Slot].Attachment = rendertarget_attachment_cast(Slot);
	}

	void creator<CUSTOM>::setFramebuffer()
	{
		Data.Slot[COLOR0].Type = detail::FRAMEBUFFER;
		Data.Slot[COLOR0].Renderbuffer = renderbuffer::name::Null();
		Data.Slot[COLOR0].Image = image::name::Null();
		Data.Slot[COLOR0].Attachment = rendertarget_attachment_cast(COLOR0);
	}

	////////////////////////////////////
	// Object

	object::object
	(
		creator<FRAMEBUFFER> const & Creator
	) :
		Data(Creator.Data),
		Name(0)
	{}

	object::object
	(
		creator<CUSTOM> const & Creator
	) :
		Renderbuffer(kueken::texture::SLOT_MAX, 0),
		Data(Creator.Data),
		Name(0)
	{
		if(Data.Slot[COLOR0].Type == detail::FRAMEBUFFER)
			return;

		kueken::manager& Manager = kueken::manager::instance();

		glGenFramebuffersEXT(1, &Name);

		for(std::size_t i = 0; i < SLOT_MAX; ++i)
		{
			switch(Data.Slot[i].Type)
			{
				case detail::IMAGE:
				{
					image::object& Object = Manager.Image.getObject(
						Data.Slot[i].Image);

					glNamedFramebufferTexture2DEXT(
						Name, 
						Data.Slot[i].Attachment, 
						GL_TEXTURE_2D, 
						Object.GetName(), 
						Data.Slot[i].Level); // Level

					assert(glGetError() == GL_NO_ERROR);
				}
				break;
				
				case detail::RENDERBUFFER:
				{
					renderbuffer::object& Object = Manager.Renderbuffer.getObject(
						Data.Slot[i].Renderbuffer);

					glNamedFramebufferRenderbufferEXT(
						Name,
						Data.Slot[i].Attachment,
						GL_RENDERBUFFER_EXT,
						Object.GetName());
				}
				break;

			case detail::FRAMEBUFFER:
				break;
			}
		}

		bool Error = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT;
		
		assert(!Error);
		if(Error)
			return;
	}

	object::~object()
	{
		glDeleteFramebuffersEXT(1, &Name);
	}

	void object::bind(target const & Target)
	{
		glBindFramebufferEXT(rendertarget_format_cast(Target), Name);
	}

}//namespace rendertarget
}//namespace kueken
