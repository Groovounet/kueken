#include <core/texture.hpp>
#include <core/renderer.hpp>

namespace
{
	GLenum image_target_cast(kueken::texture::target const & Target)
	{
		static GLenum const Cast[] =
		{
			GL_TEXTURE_1D,						// TEXTURE_1D
			GL_TEXTURE_2D,						// TEXTURE_2D
			GL_TEXTURE_3D,						// TEXTURE_3D
			GL_TEXTURE_1D_ARRAY,				// ARRAY_1D
			GL_TEXTURE_2D_ARRAY,				// ARRAY_2D
			GL_TEXTURE_RECTANGLE,				// RECT
			GL_TEXTURE_BUFFER,					// TEXTURE_BUFFER
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,		// CUBE_POS_X
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,		// CUBE_NEG_X
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,		// CUBE_POS_Y
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,		// CUBE_NEG_Y
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z,		// CUBE_POS_Z
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z		// CUBE_NEG_Z
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::texture::TARGET_MAX,
			"Cast array size mismatch");

		return Cast[Target];
	}

	GLenum image_internal_cast(kueken::texture::format const & Format)
	{
		static GLenum const Cast[] =
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
			GL_RGB10_A2UI,				//RGB10_A2UI
			GL_R11F_G11F_B10F,			//R11FG11FB10F
			GL_SRGB8,					//SRGB8
			GL_SRGB8_ALPHA8,			//SRGB8A8

			GL_COMPRESSED_RGB_S3TC_DXT1_EXT,	//RGB_DXT1
			GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,	//RGBA_DXT1
			GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,	//RGBA_DXT3
			GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,	//RGBA_DXT5

			GL_COMPRESSED_RED_RGTC1,					//COMPRESSED8_R_UNORM			// BC4_UNORM
			GL_COMPRESSED_SIGNED_RED_RGTC1,			//COMPRESSED8_R_SNORM			// BC4_SNORM
			GL_COMPRESSED_RG_RGTC2,					//COMPRESSED16_RG_UNORM			// BC5_UNORM
			GL_COMPRESSED_SIGNED_RG_RGTC2,				//COMPRESSED16_RG_SNORM			// BC5_SNORM
			GL_COMPRESSED_RGBA_BPTC_UNORM_ARB,				//COMPRESSED16_RGBA_UNORM		// BC7
			GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB,		//COMPRESSED16_SRGB_ALPHA_UNORM	// BC7
			GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB,		//COMPRESSED16_RGB_SFLOAT		// BC6H
			GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB,		//COMPRESSED16_RGB_UFLOAT		// BC6H
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::texture::FORMAT_MAX,
			"Cast array size mismatch");

		assert(Cast[Format] != GL_NONE);

		return Cast[Format];
	}

	bool image_compressed_cast(kueken::texture::format const & Format)
	{
		static bool const Cast[] =
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

			true,			//RGB9E5
			true,			//RGB10_A2UI
			true,			//R11FG11FB10F
			true,			//SRGB8
			true,			//SRGB8A8

			true,	//RGB_DXT1
			true,	//RGBA_DXT1
			true,	//RGBA_DXT3
			true,	//RGBA_DXT5
			true,	//COMPRESSED_RED_RGTC1
			true,	//COMPRESSED_SIGNED_RED_RGTC1
			true,	//COMPRESSED_RG_RGTC2
			true,	//COMPRESSED_SIGNED_RG_RGTC2
			true,	//COMPRESSED_RGBA_BPTC_UNORM
			true,	//COMPRESSED_SRGB_ALPHA_BPTC_UNORM
			true,	//COMPRESSED_RGB_BPTC_SIGNED_FLOAT
			true	//COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT
		};

		static_assert(
			sizeof(Cast) / sizeof(bool) == kueken::texture::FORMAT_MAX,
			"Cast array size mismatch");

		return Cast[Format];
	}

	GLenum image_format_cast(kueken::texture::format Format)
	{
		static GLenum const Cast[] =
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
			GL_RGBA,				//RGB10_A2UI,
			GL_RGB,					//R11FG11FB10F,
			GL_RGB,					//SRGB8,
			GL_RGBA,				//SRGB8A8,

			GL_RGB,					//RGB_DXT1
			GL_RGBA,				//RGBA_DXT1
			GL_RGBA,				//RGBA_DXT3
			GL_RGBA,				//RGBA_DXT5

			GL_RED,					//COMPRESSED_RED_RGTC1
			GL_RED,				//COMPRESSED_SIGNED_RED_RGTC1
			GL_RG,						//COMPRESSED_RG_RGTC2
			GL_RG,				//COMPRESSED_SIGNED_RG_RGTC2
			GL_RGBA,				//COMPRESSED_RGBA_BPTC_UNORM
			GL_RGBA,		//COMPRESSED_SRGB_ALPHA_BPTC_UNORM
			GL_RGB,		//COMPRESSED_RGB_BPTC_SIGNED_FLOAT
			GL_RGB,		//COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::texture::FORMAT_MAX,
			"Cast array size mismatch");

		return Cast[Format];
	}

	GLenum image_type_cast(kueken::texture::format const & Format)
	{
		static GLenum const Cast[kueken::texture::FORMAT_MAX] =
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

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::texture::FORMAT_MAX,
			"Cast array size mismatch");

		GLenum Value = Cast[Format];

		//assert(Value != GL_NONE);

		return Value;
	}

	GLenum swizzle_cast(kueken::texture::swizzle const & Swizzle)
	{
		static GLenum const Cast[] =
		{
			GL_RED,		//RED
			GL_GREEN,	//GREEN
			GL_BLUE,	//BLUE
			GL_ALPHA,	//ALPHA
			GL_ZERO,	//ZERO
			GL_ONE,		//ONE
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::texture::SWIZZLE_MAX,
			"Cast array size mismatch");

		return Cast[Swizzle];
	}

}//namespace

namespace kueken{
namespace texture
{
	creator<IMAGE>::creator(renderer & Renderer) :
		Renderer(Renderer)
	{}

	void creator<IMAGE>::setTarget(target const & Target)
	{
		Data.Target	= image_target_cast(Target);
	}

	void creator<IMAGE>::setFormat(format const & Format)
	{
		Data.Type = image_type_cast(Format);
		Data.InternalFormat = image_internal_cast(Format);
		Data.Format = image_format_cast(Format);
		Data.Compressed = image_compressed_cast(Format);
	}

	void creator<IMAGE>::setSwizzle
	(
		swizzle const & Red, 
		swizzle const & Green, 
		swizzle const & Blue, 
		swizzle const & Alpha
	)
	{
		this->Data.Swizzle[0] = swizzle_cast(Red);
		this->Data.Swizzle[1] = swizzle_cast(Green);
		this->Data.Swizzle[2] = swizzle_cast(Blue);
		this->Data.Swizzle[3] = swizzle_cast(Alpha);
	}

	void creator<IMAGE>::setLevel
	(
		glm::int32 const & BaseLevel, 
		glm::int32 const & MaxLevel
	)
	{
		this->Data.BaseLevel = GLint(BaseLevel);
		this->Data.MaxLevel = GLint(MaxLevel);
	}

	void creator<IMAGE>::setImage
	(
		level const & Level, 
		glm::uvec3 const & Size,
		void const * const Pointer // shared_array?
	)
	{
		if(this->Data.Mipmaps.size() <= Level)
			this->Data.Mipmaps.resize(Level + 1);
		this->Data.Mipmaps[Level].Data = Pointer;
		this->Data.Mipmaps[Level].Width = Size.x;
		this->Data.Mipmaps[Level].Height = Size.y;
		this->Data.Mipmaps[Level].Depth = Size.z;
	}

	bool creator<IMAGE>::validate()
	{
		return true;
	}

	creator<BUFFER>::creator(renderer & Renderer) :
		Renderer(Renderer)
	{}

	void creator<BUFFER>::setBuffer(buffer::name const & Name)
	{
		this->Buffer = Name;
	}

	void creator<BUFFER>::setFormat(texture::format const & Format)
	{
		this->InternalFormat = image_internal_cast(Format);
	}

	bool creator<BUFFER>::validate()
	{
		return true;
	}

	objectImage::objectImage(creator<IMAGE> const & Creator) :
		object(Creator.Renderer),
		Data(Creator.Data)
	{
		glGenTextures(1, &Name);

		glTextureParameteriEXT(
			Name, 
			Data.Target, 
			GL_TEXTURE_MIN_FILTER, 
			Data.Mipmaps.size() > 1 ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);

		glTextureParameteriEXT(
			Name, 
			Data.Target, 
			GL_TEXTURE_MAG_FILTER, 
			GL_NEAREST);

        glTextureParameterivEXT(
			Name, 
			Data.Target, 
			GL_TEXTURE_SWIZZLE_RGBA, 
			&Data.Swizzle[0]);

		assert(glGetError() == GL_NO_ERROR);

		if(Data.Compressed)
		{
			glTextureParameteriEXT(
				Name, 
				Data.Target, 
				GL_TEXTURE_BASE_LEVEL, 
				Data.BaseLevel);

			glTextureParameteriEXT(
				Name, 
				Data.Target, 
				GL_TEXTURE_MAX_LEVEL, 
				glm::min(GLint(Data.MaxLevel), GLint(Data.Mipmaps.size())));

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
		}

		assert(glGetError() == GL_NO_ERROR);
	}

	objectImage::~objectImage()
	{
		glDeleteTextures(1, &Name);
	}

	void objectImage::bind
	(
		slot const & Slot
	)
	{
		glBindMultiTextureEXT(
			GL_TEXTURE0 + GLenum(Slot), 
			this->Data.Target, 
			this->Name);
	}

	void objectImage::set
	(
		level const & Level,
		glm::uvec2 const & Position,
		glm::uvec2 const & Size,
		void const * const Pointer
	)
	{
		glTextureSubImage2DEXT(
			Name, 
			Data.Target,
			Level, 
			Position.x,
			Position.y,
			Size.x, 
			Size.y, 
			Data.Format, 
			Data.Type, 
			Pointer);
	}

	void objectImage::generateMipmaps()
	{
		glGenerateTextureMipmapEXT(Name, Data.Target);
	}

	objectBuffer::objectBuffer
	(
		creator<BUFFER> const & Creator
	) :
		object(Creator.Renderer)
	{
		assert(Creator.InternalFormat != GL_NONE);

		kueken::buffer::object const & Buffer = this->Renderer.map(Creator.Buffer);

		glGenTextures(1, &this->Name);

		glTextureBufferEXT(
			this->Name, 
			GL_TEXTURE_BUFFER, 
			Creator.InternalFormat, 
			Buffer.GetName());

		this->Renderer.unmap(Creator.Buffer);
	}

	objectBuffer::~objectBuffer()
	{
		glDeleteTextures(1, &Name);
	}

	void objectBuffer::bind
	(
		slot const & Slot
	)
	{
		glBindMultiTextureEXT(
			GL_TEXTURE0 + GLenum(Slot), 
			GL_TEXTURE_BUFFER, 
			this->Name);
	}

}//namespace texture
}//namespace kueken

