#ifndef KUEKEN_TEXTURE_INCLUDED
#define KUEKEN_TEXTURE_INCLUDED

#include "detail/detail.hpp"
#include "program.hpp"

namespace kueken{
namespace rendertarget
{
	class object;
}//namespace rendertarget

namespace texture{

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
		R11F_G11F_B10F,
		SRGB8,
		SRGB8_A8,

		RGB_DXT1,
		RGBA_DXT1,
		RGBA_DXT3,
		RGBA_DXT5,

		FORMAT_MAX
	};

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
		void setTarget(target const & Target);
		void setFormat(format const & Format);
		void setMipmap(
			level const & Level, 
			glm::uvec3 const & Size,
			void const * const Pointer); // shared_array?

		virtual bool validate();

	private:
		detail::data Data;
	};

	class object : boost::noncopyable
	{
		friend class rendertarget::object;

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
