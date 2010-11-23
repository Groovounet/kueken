#include <core/framebuffer.hpp>
#include <core/manager.hpp>

namespace
{
	GLenum framebuffer_format_cast(kueken::framebuffer::target Target)
	{
		static GLenum const Cast[] =
		{
			GL_READ_FRAMEBUFFER,		// READ
			GL_DRAW_FRAMEBUFFER,		// DRAW
			GL_FRAMEBUFFER				// RENDERTARGET
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::framebuffer::TARGET_MAX,
			"Cast array size mismatch");

		return Cast[Target];
	}

	GLenum framebuffer_attachment_cast(kueken::framebuffer::slot Slot)
	{
		static GLenum const Cast[] =
		{
			GL_DEPTH_ATTACHMENT,	//DEPTH,
			GL_COLOR_ATTACHMENT0,	//COLOR0,
			GL_COLOR_ATTACHMENT1,	//COLOR1,
			GL_COLOR_ATTACHMENT2,	//COLOR2,
			GL_COLOR_ATTACHMENT3,	//COLOR3,
			GL_COLOR_ATTACHMENT4,	//COLOR4,
			GL_COLOR_ATTACHMENT5,	//COLOR5,
			GL_COLOR_ATTACHMENT6,	//COLOR6,
			GL_COLOR_ATTACHMENT7	//COLOR7
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::framebuffer::SLOT_MAX,
			"Cast array size mismatch");

		return Cast[Slot];
	}

}//namespace

namespace kueken{
namespace framebuffer
{
	enum format
	{
		DEPTH24,
		STENCIL
	};

	////////////////////////////////////
	// creator<FRAMEBUFFER>

	creator<FRAMEBUFFER>::creator(renderer & Renderer)
	{
		Data.Slot[COLOR0].Type = detail::FRAMEBUFFER;
		Data.Slot[COLOR0].Renderbuffer = renderbuffer::name::null();
		Data.Slot[COLOR0].Texture = texture::name::null();
		Data.Slot[COLOR0].Attachment = framebuffer_attachment_cast(COLOR0);
	}

	////////////////////////////////////
	// creator<CUSTOM>

	creator<CUSTOM>::creator
	(
		renderer & Renderer
	)
	{}

	void creator<CUSTOM>::setTexture
	(
		slot const & Slot, 
		texture::name const & Texture,
		glm::uint Level
	)
	{
		Data.Slot[Slot].Type = detail::TEXTURE;
		Data.Slot[Slot].Renderbuffer = renderbuffer::name::null();
		Data.Slot[Slot].Texture = Texture;
		Data.Slot[Slot].Level = Level;
		Data.Slot[Slot].Attachment = framebuffer_attachment_cast(Slot);
	}

	void creator<CUSTOM>::setRenderbuffer
	(
		slot const & Slot, 
		renderbuffer::name const & Renderbuffer
	)
	{
		Data.Slot[Slot].Type = detail::RENDERBUFFER;
		Data.Slot[Slot].Renderbuffer = Renderbuffer;
		Data.Slot[Slot].Texture = texture::name::null();
		Data.Slot[Slot].Attachment = framebuffer_attachment_cast(Slot);
	}

	void creator<CUSTOM>::setFramebuffer()
	{
		Data.Slot[COLOR0].Type = detail::FRAMEBUFFER;
		Data.Slot[COLOR0].Renderbuffer = renderbuffer::name::null();
		Data.Slot[COLOR0].Texture = texture::name::null();
		Data.Slot[COLOR0].Attachment = framebuffer_attachment_cast(COLOR0);
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
		Renderbuffer(8, 0),
		Data(Creator.Data),
		Name(0)
	{
		if(Data.Slot[COLOR0].Type == detail::FRAMEBUFFER)
			return;

		kueken::manager & Manager = kueken::manager::instance();

		glGenFramebuffers(1, &Name);

		for(int i = 0; i < SLOT_MAX; ++i)
		{
			switch(Data.Slot[i].Type)
			{
				case detail::TEXTURE:
				{
					texture::object & Object = Manager.Texture.getObject(
						Data.Slot[i].Texture);

					glNamedFramebufferTextureEXT(
						Name, 
						Data.Slot[i].Attachment, 
						Object.Name, 
						Data.Slot[i].Level); // Level


					//glNamedFramebufferDrawBufferEXT(Name, GL_COLOR_ATTACHMENT0);
					//glNamedFramebufferReadBufferEXT(Name, GL_COLOR_ATTACHMENT0);

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
						GL_RENDERBUFFER,
						Object.GetName());
				}
				break;

			case detail::FRAMEBUFFER:
				break;
			}
		}

		bool Error = glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;

		assert(!Error);
		assert(glGetError() == GL_NO_ERROR);
	}

	object::~object()
	{
		glDeleteFramebuffers(1, &Name);
	}

	void object::bind(target const & Target)
	{
		assert(glGetError() == GL_NO_ERROR);

		glBindFramebuffer(framebuffer_format_cast(Target), Name);

		assert(glGetError() == GL_NO_ERROR);
	}

}//namespace framebuffer
}//namespace kueken
