#ifndef KUEKEN_TEXTURE_INCLUDED
#define KUEKEN_TEXTURE_INCLUDED

#include "detail/detail.hpp"
#include "program.hpp"

namespace kueken{
namespace framebuffer
{
	class object;
}//namespace framebuffer

namespace texture
{
	enum swizzle
	{
		RED,
		GREEN,
		BLUE,
		ALPHA,
		ZERO,
		ONE,
		SWIZZLE_MAX
	};

	enum type
	{
		buffer,
		image,
		multisample
	};

	enum target
	{
		IMAGE1D,
		IMAGE2D,
		IMAGE3D,
		ARRAY1D,
		ARRAY2D,
		RECT,
		CUBE_POS_X,
		CUBE_NEG_X,
		CUBE_POS_Y,
		CUBE_NEG_Y,
		CUBE_POS_Z,
		CUBE_NEG_Z,
		TARGET_MAX
	};

	enum format
	{
		R8,
		RG8,
		RGB8,
		RGBA8,

		R16,
		RG16,
		RGB16,
		RGBA16,

		R16F,
		RG16F,
		RGB16F,
		RGBA16F,

		R32F,
		RG32F,
		RGB32F,
		RGBA32F,

		R8I,
		RG8I,
		RGB8I,
		RGBA8I,

		R16I,
		RG16I,
		RGB16I,
		RGBA16I,

		R32I,
		RG32I,
		RGB32I,
		RGBA32I,

		R8U,
		RG8U,
		RGB8U,
		RGBA8U, 

		R16U,
		RG16U,
		RGB16U,
		RGBA16U,

		R32U,
		RG32U,
		RGB32U,
		RGBA32U,

		D16,
		D24,
		D32F,
		D24_S8,
		D32F_S8,

		RGB9_E5,
		RGB10_A2UI,
		R11F_G11F_B10F,
		SRGB8,
		SRGB8_A8,

		RGB_DXT1,								//COMPRESSED8_RGB_UNORM8 COMPRESSED_SRGB_UNORM8				// DXGI_FORMAT_BC1_UNORM / DXGI_FORMAT_BC1_UNORM_SRGB / D3DFMT_DXT1
		RGBA_DXT1,								//COMPRESSED8_RGBA_UNORM8 COMPRESSED_SRGB_ALPHA_UNORM8		// DXGI_FORMAT_BC1_UNORM / DXGI_FORMAT_BC1_UNORM_SRGB / D3DFMT_DXT1
		RGBA_DXT3,								//COMPRESSED16_RGBA_UNORM16 COMPRESSED_SRGB_ALPHA_UNORM16	// DXGI_FORMAT_BC2_TYPELESS, DXGI_FORMAT_BC2_UNORM, or DXGI_BC2_UNORM_SRGB, D3DFMT_DXT2 and D3DFMT_DXT3
		RGBA_DXT5,								//COMPRESSED16_RGBA_UNORM16 COMPRESSED_SRGB_ALPHA_UNORM16	// D3DFMT_DXT4 and D3DFMT_DXT5 /  DXGI_FORMAT_BC3_UNORM or DXGI_FORMAT_BC3_UNORM_SRGB
        COMPRESSED_RED_RGTC1,					//COMPRESSED8_R_UNORM			// BC4_UNORM
        COMPRESSED_SIGNED_RED_RGTC1,			//COMPRESSED8_R_SNORM			// BC4_SNORM
        COMPRESSED_RG_RGTC2,					//COMPRESSED16_RG_UNORM			// BC5_UNORM
        COMPRESSED_SIGNED_RG_RGTC2,				//COMPRESSED16_RG_SNORM			// BC5_SNORM
        COMPRESSED_RGBA_BPTC_UNORM,				//COMPRESSED16_RGBA_UNORM		// BC7 DXGI_FORMAT_BC7_UNORM DXGI_FORMAT_BC7_TYPELESS
        COMPRESSED_SRGB_ALPHA_BPTC_UNORM,		//COMPRESSED16_SRGB_ALPHA_UNORM	// BC7 DXGI_FORMAT_BC7_UNORM_SRGB DXGI_FORMAT_BC7_TYPELESS
        COMPRESSED_RGB_BPTC_SIGNED_FLOAT,		//COMPRESSED16_RGB_SFLOAT		// BC6H DXGI_FORMAT_BC6H_SF16 DXGI_FORMAT_BC6H_TYPELESS
        COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,		//COMPRESSED16_RGB_UFLOAT		// BC6H DXGI_FORMAT_BC6H_UF16 DXGI_FORMAT_BC6H_TYPELESS

		FORMAT_MAX
	};

/*
    DXGI_FORMAT_R32G32B32A32_TYPELESS       = 1,
    DXGI_FORMAT_R32G32B32A32_FLOAT          = 2,
    DXGI_FORMAT_R32G32B32A32_UINT           = 3,
    DXGI_FORMAT_R32G32B32A32_SINT           = 4,
    DXGI_FORMAT_R32G32B32_TYPELESS          = 5,
    DXGI_FORMAT_R32G32B32_FLOAT             = 6,
    DXGI_FORMAT_R32G32B32_UINT              = 7,
    DXGI_FORMAT_R32G32B32_SINT              = 8,
    DXGI_FORMAT_R16G16B16A16_TYPELESS       = 9,
    DXGI_FORMAT_R16G16B16A16_FLOAT          = 10,
    DXGI_FORMAT_R16G16B16A16_UNORM          = 11,
    DXGI_FORMAT_R16G16B16A16_UINT           = 12,
    DXGI_FORMAT_R16G16B16A16_SNORM          = 13,
    DXGI_FORMAT_R16G16B16A16_SINT           = 14,
    DXGI_FORMAT_R32G32_TYPELESS             = 15,
    DXGI_FORMAT_R32G32_FLOAT                = 16,
    DXGI_FORMAT_R32G32_UINT                 = 17,
    DXGI_FORMAT_R32G32_SINT                 = 18,
    DXGI_FORMAT_R32G8X24_TYPELESS           = 19,
    DXGI_FORMAT_D32_FLOAT_S8X24_UINT        = 20,
    DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS    = 21,
    DXGI_FORMAT_X32_TYPELESS_G8X24_UINT     = 22,
    DXGI_FORMAT_R10G10B10A2_TYPELESS        = 23,
    DXGI_FORMAT_R10G10B10A2_UNORM           = 24,
    DXGI_FORMAT_R10G10B10A2_UINT            = 25,
    DXGI_FORMAT_R11G11B10_FLOAT             = 26,
    DXGI_FORMAT_R8G8B8A8_TYPELESS           = 27,
    DXGI_FORMAT_R8G8B8A8_UNORM              = 28,
    DXGI_FORMAT_R8G8B8A8_UNORM_SRGB         = 29,
    DXGI_FORMAT_R8G8B8A8_UINT               = 30,
    DXGI_FORMAT_R8G8B8A8_SNORM              = 31,
    DXGI_FORMAT_R8G8B8A8_SINT               = 32,
    DXGI_FORMAT_R16G16_TYPELESS             = 33,
    DXGI_FORMAT_R16G16_FLOAT                = 34,
    DXGI_FORMAT_R16G16_UNORM                = 35,
    DXGI_FORMAT_R16G16_UINT                 = 36,
    DXGI_FORMAT_R16G16_SNORM                = 37,
    DXGI_FORMAT_R16G16_SINT                 = 38,
    DXGI_FORMAT_R32_TYPELESS                = 39,
    DXGI_FORMAT_D32_FLOAT                   = 40,
    DXGI_FORMAT_R32_FLOAT                   = 41,
    DXGI_FORMAT_R32_UINT                    = 42,
    DXGI_FORMAT_R32_SINT                    = 43,
    DXGI_FORMAT_R24G8_TYPELESS              = 44,
    DXGI_FORMAT_D24_UNORM_S8_UINT           = 45,
    DXGI_FORMAT_R24_UNORM_X8_TYPELESS       = 46,
    DXGI_FORMAT_X24_TYPELESS_G8_UINT        = 47,
    DXGI_FORMAT_R8G8_TYPELESS               = 48,
    DXGI_FORMAT_R8G8_UNORM                  = 49,
    DXGI_FORMAT_R8G8_UINT                   = 50,
    DXGI_FORMAT_R8G8_SNORM                  = 51,
    DXGI_FORMAT_R8G8_SINT                   = 52,
    DXGI_FORMAT_R16_TYPELESS                = 53,
    DXGI_FORMAT_R16_FLOAT                   = 54,
    DXGI_FORMAT_D16_UNORM                   = 55,
    DXGI_FORMAT_R16_UNORM                   = 56,
    DXGI_FORMAT_R16_UINT                    = 57,
    DXGI_FORMAT_R16_SNORM                   = 58,
    DXGI_FORMAT_R16_SINT                    = 59,
    DXGI_FORMAT_R8_TYPELESS                 = 60,
    DXGI_FORMAT_R8_UNORM                    = 61,
    DXGI_FORMAT_R8_UINT                     = 62,
    DXGI_FORMAT_R8_SNORM                    = 63,
    DXGI_FORMAT_R8_SINT                     = 64,
    DXGI_FORMAT_A8_UNORM                    = 65,
    DXGI_FORMAT_R1_UNORM                    = 66,
    DXGI_FORMAT_R9G9B9E5_SHAREDEXP          = 67,
    DXGI_FORMAT_R8G8_B8G8_UNORM             = 68,
    DXGI_FORMAT_G8R8_G8B8_UNORM             = 69,
    DXGI_FORMAT_BC1_TYPELESS                = 70,
    DXGI_FORMAT_BC1_UNORM                   = 71,
    DXGI_FORMAT_BC1_UNORM_SRGB              = 72,
    DXGI_FORMAT_BC2_TYPELESS                = 73,
    DXGI_FORMAT_BC2_UNORM                   = 74,
    DXGI_FORMAT_BC2_UNORM_SRGB              = 75,
    DXGI_FORMAT_BC3_TYPELESS                = 76,
    DXGI_FORMAT_BC3_UNORM                   = 77,
    DXGI_FORMAT_BC3_UNORM_SRGB              = 78,
    DXGI_FORMAT_BC4_TYPELESS                = 79,
    DXGI_FORMAT_BC4_UNORM                   = 80,
    DXGI_FORMAT_BC4_SNORM                   = 81,
    DXGI_FORMAT_BC5_TYPELESS                = 82,
    DXGI_FORMAT_BC5_UNORM                   = 83,
    DXGI_FORMAT_BC5_SNORM                   = 84,
    DXGI_FORMAT_B5G6R5_UNORM                = 85,
    DXGI_FORMAT_B5G5R5A1_UNORM              = 86,
    DXGI_FORMAT_B8G8R8A8_UNORM              = 87,
    DXGI_FORMAT_B8G8R8X8_UNORM              = 88,
    DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM  = 89,
    DXGI_FORMAT_B8G8R8A8_TYPELESS           = 90,
    DXGI_FORMAT_B8G8R8A8_UNORM_SRGB         = 91,
    DXGI_FORMAT_B8G8R8X8_TYPELESS           = 92,
    DXGI_FORMAT_B8G8R8X8_UNORM_SRGB         = 93,
    DXGI_FORMAT_BC6H_TYPELESS               = 94,
    DXGI_FORMAT_BC6H_UF16                   = 95,
    DXGI_FORMAT_BC6H_SF16                   = 96,
    DXGI_FORMAT_BC7_TYPELESS                = 97,
    DXGI_FORMAT_BC7_UNORM                   = 98,
    DXGI_FORMAT_BC7_UNORM_SRGB              = 99,
*/

namespace detail
{
	struct mipmap
	{
		GLsizei Width;
		GLsizei Height;
		GLsizei Depth;
		GLvoid const * Data;
	};

	struct data
	{
		data() :
			Target(GL_TEXTURE_2D),
			InternalFormat(GL_RGBA),
			Format(GL_RGBA),
			Type(GL_UNSIGNED_BYTE),
			Compressed(false),
			BaseLevel(0),
			MaxLevel(1000)
		{
			Swizzle[0] = GL_RED;
			Swizzle[1] = GL_GREEN;
			Swizzle[2] = GL_BLUE;
			Swizzle[3] = GL_ALPHA;
		}

		GLenum Target;
		GLint InternalFormat;
		GLenum Format;
		GLenum Type;
		bool Compressed;
		GLint Swizzle[4];
		GLint BaseLevel;
		GLint MaxLevel;

		std::vector<mipmap> Mipmaps;
	};

}//namespace detail
/*
	enum slot
	{
		SLOT0,
		SLOT1,
		SLOT2,
		SLOT3,
		SLOT4,
		SLOT5,
		SLOT6,
		SLOT7,
		SLOT_MAX
	};
*/
	typedef std::size_t slot;
	typedef std::size_t level;
/*
	class creator
	{
	public:
		void setVariable(program::variable const & Variable);
		void setImage(image::name const & Image);
		void setSampler(sampler::name const & Sampler);

		virtual bool validate(){assert(0); return false;}

	private:
		detail::data Data;
	};
*/
	class creator
	{
		friend class object;

	public:
		creator(renderer & Renderer);
		void setTarget(
			target const & Target);
		void setFormat(
			format const & Format);
		void setSwizzle(
			swizzle const & Red, 
			swizzle const & Green, 
			swizzle const & Blue, 
			swizzle const & Alpha);
		void setLevel(
			glm::int32 const & BaseLevel, 
			glm::int32 const & MaxLevel);
		void setImage(
			level const & Level, 
			glm::uvec3 const & Size,
			void const * const Pointer); // gli::image?

		virtual bool validate();

	private:
		renderer & Renderer;
		detail::data Data;
	};

	class object : boost::noncopyable
	{
		friend class framebuffer::object;

	public:
		object(creator const & Creator);
		~object();

		void bind(
			slot const & Slot,
			target const & Target);
		//void* map();
		//void unmap();
		//void flush();
		void set(
			level const & Level,
			glm::uvec2 const & Position,
			glm::uvec2 const & Size,
			void const * const Pointer);

		void generateMipmaps();

	private:
		void run();

		detail::data Data;
		GLenum Name;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace texture
}//namespace kueken

#include "texture.inl"

#endif//KUEKEN_TEXTURE_INCLUDED
