#include <core/blit.hpp>

namespace
{
	//GLenum blit_mask_cast(kueken::blit::mask Mask)
	//{
	//	static GLenum const Cast[kueken::blit::MASK_MAX] =
	//	{
	//		GL_COLOR_BUFFER_BIT,		// COLOR
	//		GL_DEPTH_BUFFER_BIT,		// DEPTH
	//		GL_STENCIL_BUFFER_BIT		// STENCIL
	//	};

	//	return Cast[Mask];
	//}

	GLenum blit_filter_cast(kueken::blit::filter Filter)
	{
		static GLenum const Cast[kueken::blit::FILTER_MAX] =
		{
			GL_NEAREST,		// NEAREST
			GL_LINEAR		// LINEAR
		};

		return Cast[Filter];
	}

}//namespace

namespace kueken{
namespace blit{
namespace detail{

	data::data() :
		SrcRect(0),
		DstRect(0),
		Mask(0),
		Filter(0)
	{}

}//namespace detail

	void creator::setRectSrc(glm::ivec4 const & SrcRect)
	{
		Data.SrcRect = SrcRect;
	}

	void creator::setRectDst(glm::ivec4 const & DstRect)
	{
		Data.DstRect = DstRect;
	}

	void creator::setMask(bool Color, bool Depth, bool Stencil)
	{
		Data.Mask = 0;
		Data.Mask |= Color ? GL_COLOR_BUFFER_BIT : 0;
		Data.Mask |= Depth ? GL_DEPTH_BUFFER_BIT : 0;
		Data.Mask |= Stencil ? GL_STENCIL_BUFFER_BIT : 0;
	}

	void creator::setFilter(filter const & Filter)
	{
		Data.Filter = blit_filter_cast(Filter);
	}

	bool creator::validate()
	{
		assert(0); return true;
	}

	object::object
	(
		creator const & Creator
	) :
		Data(Creator.Data)
	{}

	void object::exec()
	{
		//glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, Read);
		//glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, Draw);

		glBlitFramebufferEXT(
			Data.SrcRect.x, Data.SrcRect.y, Data.SrcRect.z, Data.SrcRect.w, 
			Data.DstRect.x, Data.DstRect.y, Data.DstRect.z, Data.DstRect.w, 
			Data.Mask, Data.Filter);
	}

}//namespace blit
}//namespace kueken
