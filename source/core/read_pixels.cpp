#include <core/read_pixels.hpp>

namespace
{
	GLenum readpixels_format_cast(kueken::readpixels::format Format)
	{
		static GLenum const Cast[kueken::readpixels::FORMAT_MAX] =
		{
			GL_RGBA,		// RGBA8U
			GL_BGRA			// BGRA8U
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::readpixels::FORMAT_MAX,
			"Cast array size mismatch");

		return Cast[Format];
	}

	GLenum readpixels_type_cast(kueken::readpixels::format Format)
	{
		static GLenum const Cast[kueken::readpixels::FORMAT_MAX] =
		{
			GL_UNSIGNED_BYTE,		// RGBA8U
			GL_UNSIGNED_BYTE		// BGRA8U
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::readpixels::FORMAT_MAX,
			"Cast array size mismatch");

		return Cast[Format];
	}

}//namespace

namespace kueken{
namespace readpixels{
namespace detail{

	data::data() :
		Rect(0)
	{}

}//namespace detail

	void creator::setRect(glm::uvec4 const & Rect)
	{
		Data.Rect = Rect;
	}

	void creator::setFormat(format const & Format)
	{
		Data.Format = readpixels_format_cast(Format);
		Data.Type = readpixels_type_cast(Format);
	}

	bool creator::validate()
	{
		return true;
	}

	object::object
	(
		creator const & Creator
	) :
		Data(Creator.Data)
	{}

	void object::exec()
	{
		glReadBuffer(GL_BACK);
		glReadPixels(
			Data.Rect.x, Data.Rect.y, Data.Rect.z, Data.Rect.w, 
			Data.Format, Data.Type, 0);
	}

}//namespace readpixels
}//namespace kueken
