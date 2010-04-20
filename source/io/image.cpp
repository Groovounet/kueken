#include <core/image.hpp>

namespace
{
	GLenum image_target_cast(kueken::image::target Target)
	{
		static GLenum const CastImageTarget[kueken::image::TARGET_MAX] =
		{
			GL_TEXTURE_1D,						// IMAGE_1D
			GL_TEXTURE_2D,						// IMAGE_2D
			GL_TEXTURE_3D,						// IMAGE_3D
			GL_TEXTURE_1D_ARRAY_EXT,			// ARRAY_1D
			GL_TEXTURE_2D_ARRAY_EXT,			// ARRAY_2D
			GL_TEXTURE_RECTANGLE_ARB,			// RECT
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,		// CUBE_POS_X
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,		// CUBE_NEG_X
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,		// CUBE_POS_Y
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,		// CUBE_NEG_Y
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z,		// CUBE_POS_Z
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z		// CUBE_NEG_Z
		};

		return CastImageTarget[Target];
	}

	GLenum image_internal_cast(kueken::image::format Format)
	{
		static GLenum const Cast[kueken::image::FORMAT_MAX] =
		{
			GL_R8,						// R8
			GL_RG8,						// RG8
			GL_RGB8,					// RGB8
			GL_RGBA8,					// RGBA8

			GL_R16,						//R16,
			GL_RG16,					//RG16,
			GL_RGB16,					//RGB16,
			GL_RGBA16,					//RGBA16,

			GL_R16F,					//R16F,
			GL_RG16F,					//RG16F,
			GL_RGB16F,					//RGB16F,
			GL_RGBA16F,					//RGBA16F,

			GL_R32F,					//R32F,
			GL_RG32F,					//RG32F,
			GL_RGB32F,					//RGB32F,
			GL_RGBA32F,					//RGBA32F,

			GL_R8I,						//R8I,
			GL_RG8I,					//RG8I,
			GL_RGB8I,					//RGB8I,
			GL_RGBA8I,					//RGBA8I,

			GL_R16I,					//R16I,
			GL_RG16I,					//RG16I,
			GL_RGB16I,					//RGB16I,
			GL_RGBA16I,					//RGBA16I,

			GL_R32I,					//R32I,
			GL_RG32I,					//RG32I,
			GL_RGB32I,					//RGB32I,
			GL_RGBA32I,					//RGBA32I,

			GL_R8UI,					//R8U,
			GL_RG8UI,					//RG8U,
			GL_RGB8UI,					//RGB8U,
			GL_RGBA8UI,					//RGBA8U, 

			GL_R16UI,					//R16U,
			GL_RG16UI,					//RG16U,
			GL_RGB16UI,					//RGB16U,
			GL_RGBA16UI,				//RGBA16U,

			GL_R32UI,					//R32U,
			GL_RG32UI,					//RG32U,
			GL_RGB32UI,					//RGB32U,
			GL_RGBA32UI,				//RGBA32U,

			GL_DEPTH_COMPONENT16,		//D16
			GL_DEPTH_COMPONENT24,		//D24
			GL_DEPTH_COMPONENT32F,		//D32F
			GL_DEPTH24_STENCIL8,		//D24S8
			GL_DEPTH32F_STENCIL8,		//D32FS8

			GL_RGB9_E5,					//RGB9E5
			GL_R11F_G11F_B10F,			//R11FG11FB10F
			GL_SRGB8,					//SRGB8
			GL_SRGB8_ALPHA8,			//SRGB8A8

			GL_COMPRESSED_RGB_S3TC_DXT1_EXT,	//RGB_DXT1
			GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,	//RGBA_DXT1
			GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,	//RGBA_DXT3
			GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,	//RGBA_DXT5
		};

		return Cast[Format];
	}

	bool image_compressed_cast(kueken::image::format Format)
	{
		static bool const Cast[kueken::image::FORMAT_MAX] =
		{
			false,						// R8
			false,						// RG8
			false,					// RGB8
			false,					// RGBA8

			false,						//R16,
			false,					//RG16,
			false,					//RGB16,
			false,					//RGBA16,

			false,					//R16F,
			false,					//RG16F,
			false,					//RGB16F,
			false,					//RGBA16F,

			false,					//R32F,
			false,					//RG32F,
			false,					//RGB32F,
			false,					//RGBA32F,

			false,						//R8I,
			false,					//RG8I,
			false,					//RGB8I,
			false,					//RGBA8I,

			false,					//R16I,
			false,					//RG16I,
			false,					//RGB16I,
			false,					//RGBA16I,

			false,					//R32I,
			false,					//RG32I,
			false,					//RGB32I,
			false,					//RGBA32I,

			false,					//R8U,
			false,					//RG8U,
			false,					//RGB8U,
			false,					//RGBA8U, 

			false,					//R16U,
			false,					//RG16U,
			false,					//RGB16U,
			false,				//RGBA16U,

			false,					//R32U,
			false,					//RG32U,
			false,					//RGB32U,
			false,				//RGBA32U,

			false,		//D16
			false,		//D24
			false,		//D32F
			false,		//D24S8
			false,		//D32FS8

			true,					//RGB9E5
			true,			//R11FG11FB10F
			true,					//SRGB8
			true,			//SRGB8A8

			true,	//RGB_DXT1
			true,	//RGBA_DXT1
			true,	//RGBA_DXT3
			true,	//RGBA_DXT5
		};

		return Cast[Format];
	}

	GLenum image_format_cast(kueken::image::format Format)
	{
		static GLenum const Cast[kueken::image::FORMAT_MAX] =
		{
			GL_RED,					// R8
			GL_RG,					// RG8
			GL_RGB,					// RGB8
			GL_RGBA,				// RGBA8

			GL_RED,					//R16,
			GL_RG,					//RG16,
			GL_RGB,					//RGB16,
			GL_RGBA,				//RGBA16,

			GL_RED,					//R16F,
			GL_RG,					//RG16F,
			GL_RGB,					//RGB16F,
			GL_RGBA,				//RGBA16F,

			GL_RED,					//R32F,
			GL_RG,					//RG32F,
			GL_RGB,					//RGB32F,
			GL_RGBA,				//RGBA32F,

			GL_RED,					//R8I,
			GL_RG,					//RG8I,
			GL_RGB,					//RGB8I,
			GL_RGBA,				//RGBA8I,

			GL_RED,					//R16I,
			GL_RG,					//RG16I,
			GL_RGB,					//RGB16I,
			GL_RGBA,				//RGBA16I,

			GL_RED,					//R32I,
			GL_RG,					//RG32I,
			GL_RGB,					//RGB32I,
			GL_RGBA,				//RGBA32I,

			GL_RED,					//R8U,
			GL_RG,					//RG8U,
			GL_RGB,					//RGB8U,
			GL_RGBA,				//RGBA8U, 

			GL_RED,					//R16U,
			GL_RG,					//RG16U,
			GL_RGB,					//RGB16U,
			GL_RGBA,				//RGBA16U,

			GL_RED,					//R32U,
			GL_RG,					//RG32U,
			GL_RGB,					//RGB32U,
			GL_RGBA,				//RGBA32U,

			GL_DEPTH_COMPONENT,		//D16,
			GL_DEPTH_COMPONENT,		//D24,
			GL_DEPTH_COMPONENT,		//D32F,
			GL_DEPTH_STENCIL,		//D24S8,
			GL_DEPTH_STENCIL,		//D32FS8,

			GL_RGB,					//RGB9E5,
			GL_RGBA,				//R11FG11FB10F,
			GL_RGB,					//SRGB8,
			GL_RGBA,				//SRGB8A8,

			GL_RGB,					//RGB_DXT1
			GL_RGBA,				//RGBA_DXT1
			GL_RGBA,				//RGBA_DXT3
			GL_RGBA					//RGBA_DXT5
		};

		return Cast[Format];
	}

	GLenum image_type_cast(kueken::image::format Format)
	{
		static GLenum const Cast[kueken::image::FORMAT_MAX] =
		{
			GL_UNSIGNED_BYTE,					// R8
			GL_UNSIGNED_BYTE,					// RG8
			GL_UNSIGNED_BYTE,					// RGB8
			GL_UNSIGNED_BYTE,					// RGBA8

			GL_UNSIGNED_SHORT,					//R16,
			GL_UNSIGNED_SHORT,					//RG16,
			GL_UNSIGNED_SHORT,					//RGB16,
			GL_UNSIGNED_SHORT,					//RGBA16,

			GL_HALF_FLOAT,						//R16F,
			GL_HALF_FLOAT,						//RG16F,
			GL_HALF_FLOAT,						//RGB16F,
			GL_HALF_FLOAT,						//RGBA16F,

			GL_FLOAT,							//R32F,
			GL_FLOAT,							//RG32F,
			GL_FLOAT,							//RGB32F,
			GL_FLOAT,							//RGBA32F,

			GL_BYTE,							//R8I,
			GL_BYTE,							//RG8I,
			GL_BYTE,							//RGB8I,
			GL_BYTE,							//RGBA8I,

			GL_SHORT,							//R16I,
			GL_SHORT,							//RG16I,
			GL_SHORT,							//RGB16I,
			GL_SHORT,							//RGBA16I,

			GL_INT,								//R32I,
			GL_INT,								//RG32I,
			GL_INT,								//RGB32I,
			GL_INT,								//RGBA32I,

			GL_UNSIGNED_BYTE,					//R8U,
			GL_UNSIGNED_BYTE,					//RG8U,
			GL_UNSIGNED_BYTE,					//RGB8U,
			GL_UNSIGNED_BYTE,					//RGBA8U, 

			GL_UNSIGNED_SHORT,					//R16U,
			GL_UNSIGNED_SHORT,					//RG16U,
			GL_UNSIGNED_SHORT,					//RGB16U,
			GL_UNSIGNED_SHORT,					//RGBA16U,

			GL_UNSIGNED_INT,					//R32U,
			GL_UNSIGNED_INT,					//RG32U,
			GL_UNSIGNED_INT,					//RGB32U,
			GL_UNSIGNED_INT,					//RGBA32U,

			GL_UNSIGNED_BYTE,					//D16,
			GL_UNSIGNED_BYTE,					//D24,
			GL_UNSIGNED_BYTE,					//D32F,
			GL_UNSIGNED_BYTE,					//D24_S8,
			GL_UNSIGNED_BYTE,					//D32F_S8,

			GL_NONE,							//RGB9_E5,
			GL_NONE,							//R11F_G11F_B10F,
			GL_NONE,							//SRGB8,
			GL_NONE,							//SRGB8_A8,

			GL_NONE,	//RGB_DXT1
			GL_NONE,	//RGBA_DXT1
			GL_NONE,	//RGBA_DXT3
			GL_NONE		//RGBA_DXT5
		};

		GLenum Value = Cast[Format];

		//assert(Value != GL_NONE);

		return Value;
	}

}//namespace

namespace kueken{
namespace image
{
	void creator::setTarget(target Target)
	{
		Data.Target	= image_target_cast(Target);
	}

	void creator::setFormat(format Format)
	{
		Data.Type = image_type_cast(Format);
		Data.InternalFormat = image_internal_cast(Format);
		Data.Format = image_format_cast(Format);
		Data.Compressed = image_compressed_cast(Format);
	}

	void creator::setGenerateMipmaps(bool GenerateMipmaps)
	{
		Data.GenerateMipmaps = GenerateMipmaps;
	}

	void creator::setMipmap
	(
		std::size_t Level, 
		glm::uvec3 const & Size,
		void const * const Pointer // shared_array?
	)
	{
		if(Data.Mipmaps.size() <= Level)
			Data.Mipmaps.resize(Level + 1);
		Data.Mipmaps[Level].Data = Pointer;
		Data.Mipmaps[Level].Width = Size.x;
		Data.Mipmaps[Level].Height = Size.y;
		Data.Mipmaps[Level].Depth = Size.z;
	}

	bool creator::validate()
	{
		return true;
	}

	object::object(creator const & Creator) :
		Data(Creator.Data)
	{
		glGenTextures(1, &Name);

		glTextureParameteriEXT(
			Name, 
			Data.Target, 
			GL_TEXTURE_MAG_FILTER, 
			GL_LINEAR);

		glTextureParameteriEXT(
			Name, 
			Data.Target, 
			GL_TEXTURE_MIN_FILTER, 
			GL_LINEAR);

		glTextureParameterfEXT(
			Name, 
			Data.Target, 
			GL_TEXTURE_WRAP_S, 
			GL_CLAMP_TO_EDGE);

		glTextureParameterfEXT(
			Name, 
			Data.Target, 
			GL_TEXTURE_WRAP_T, 
			GL_CLAMP_TO_EDGE);

		glTextureParameterfEXT(
			Name, 
			Data.Target, 
			GL_TEXTURE_WRAP_R, 
			GL_CLAMP_TO_EDGE);

		assert(glGetError() == GL_NO_ERROR);

		if(Data.Compressed)
		{
			glTextureParameteriEXT(
				Name, 
				Data.Target, 
				GL_TEXTURE_BASE_LEVEL, 
				0);

			glTextureParameteriEXT(
				Name, 
				Data.Target, 
				GL_TEXTURE_MAX_LEVEL, 
				GLint(Data.Mipmaps.size()));

			//! ToDo: To remove!
			glBindTexture(GL_TEXTURE_2D, Name);

			std::size_t BlockSize = Data.InternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8 : 16;

			for(GLint Level = 0; Level < GLint(Data.Mipmaps.size()); ++Level)
			{
				std::size_t Width = Data.Mipmaps[Level].Width;
				std::size_t Height = Data.Mipmaps[Level].Height;

				std::size_t Size = ((Width + 3) / 4)*((Height + 3) / 4) * BlockSize;

				glCompressedTextureImage2DEXT(
					Name, 
					Data.Target,
					Level,
					Data.InternalFormat,
					Data.Mipmaps[Level].Width, 
					Data.Mipmaps[Level].Height, 
					0, 
					Size, 
					Data.Mipmaps[Level].Data);

				assert(glGetError() == GL_NO_ERROR);
			}

			//! ToDo: To remove!
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			for(GLint Level = 0; Level < GLint(Data.Mipmaps.size()); ++Level)
			{
				glTextureImage2DEXT(
					Name, 
					Data.Target,
					Level, 
					Data.InternalFormat,
					Data.Mipmaps[Level].Width, 
					Data.Mipmaps[Level].Height, 
					0,
					Data.Format, 
					Data.Type, 
					Data.Mipmaps[Level].Data);
			}

			if(Data.GenerateMipmaps)
				glGenerateTextureMipmapEXT(Name, Data.Target);
		}

		assert(glGetError() == GL_NO_ERROR);
	}

	object::~object()
	{
		glDeleteTextures(1, &Name);
	}

	void object::bind()
	{
		glBindTexture(
			Data.Target, 
			Name);
	}

	void object::bind(std::size_t Slot)
	{
		glBindMultiTextureEXT(
			GL_TEXTURE0 + GLenum(Slot), 
			Data.Target, 
			Name);
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

	//void object::set(std::size_t Offset, std::size_t Size, void const * const Data)
	//{

	//}

}//namespace image
}//namespace kueken
