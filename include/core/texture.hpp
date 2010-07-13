#ifndef KUEKEN_TEXTURE_INCLUDED
#define KUEKEN_TEXTURE_INCLUDED

#include "detail/detail.hpp"
#include "program.hpp"
#include "image.hpp"
#include "sampler.hpp"
#include "assembler.hpp"

namespace kueken{
namespace texture{

	enum type
	{
		buffer,
		image,
		multisample
	};

	enum format
	{
		R8,
		RG8,
		RGBA8,

		R16,
		RG16,
		RGBA16,

		R16F,
		RG16F,
		RGBA16F,

		R32F,
		RG32F,
		RGBA32F,

		R8I,
		RG8I,
		RGBA8I,

		R16I,
		RG16I,
		RGBA16I,

		R32I,
		RG32I,
		RGBA32I,

		R8U,
		RG8U,
		RGBA8U, 

		R16U,
		RG16U,
		RGBA16U,

		R32U,
		RG32U,
		RGBA32U,

		FORMAT_MAX
	};

namespace detail{

	struct data
	{
		program::name Program;
		program::variable Variable;	
		image::name Image;
		sampler::name Sampler;
		GLenum Buffer;
		GLenum Format;
	};

	class objectBuffer;
	class objectImage;

}//namespace detail

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

	template <type TYPE>
	class creator
	{};

	template <>
	class creator<buffer>// : public kueken::detail::creator
	{
		friend class detail::objectBuffer;

	public:
		void setVariable(program::variable const & Variable);
		void setBuffer(buffer::name const & Buffer);
		void setFormat(format const & Format);

		virtual bool validate(){assert(0); return false;}

	private:
		detail::data Data;
	};

	template <>
	class creator<image>// : public kueken::detail::creator
	{
		friend class detail::objectImage;

	public:
		void setVariable(program::variable const & Variable);
		void setImage(image::name const & Image);
		void setSampler(sampler::name const & Sampler);

		virtual bool validate(){assert(0); return false;}

	private:
		detail::data Data;
	};

	struct object : boost::noncopyable
	{
		virtual ~object(){}
		virtual void bind(slot Slot) = 0;
		virtual GLuint getName() const = 0;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

namespace detail
{
	class objectBuffer : public object
	{
		friend class renderer;

	public:
		objectBuffer(creator<buffer> const & Creator);
		virtual ~objectBuffer();

		virtual void bind(slot Slot);
		virtual GLuint getName() const;

	private:
		void run();

		detail::data Data;
		GLenum Name;
	};

	class objectImage : public object
	{
		friend class renderer;

	public:
		objectImage(creator<image> const & Creator);
		virtual ~objectImage();

		virtual void bind(slot Slot);
		virtual GLuint getName() const;

	private:
		void run();

		detail::data Data;
		GLenum Name;
	};

}//namespace detail

}//namespace texture
}//namespace kueken

#include "texture.inl"

#endif//KUEKEN_TEXTURE_INCLUDED
