#ifndef KUEKEN_FRAMEBUFFER_INCLUDED
#define KUEKEN_FRAMEBUFFER_INCLUDED

#include "detail/detail.hpp"
#include "texture.hpp"
#include "renderbuffer.hpp"

namespace kueken{
namespace framebuffer
{
	enum slot
	{
		DEPTH,
		COLOR0,
		COLOR1,
		COLOR2,
		COLOR3,
		COLOR4,
		COLOR5,
		COLOR6,
		COLOR7,
		SLOT_MAX
	};

	enum target
	{
		READ,
		DRAW,
		EXEC,
		TARGET_MAX
	};

	enum type
	{
		CUSTOM,
		MULTISAMPLE,
		FRAMEBUFFER,
		LAYERED
	};

namespace detail
{
	enum type
	{
		TEXTURE,
		RENDERBUFFER,
		FRAMEBUFFER
	};

	struct slot
	{
		type Type;
		renderbuffer::name Renderbuffer;
		texture::name Texture;
		glm::uint Level;
		GLenum Attachment;
	};

	struct data
	{
		slot Slot[SLOT_MAX];
		glm::uvec2 Size;
	};
}//namespace detail

	class renderer;

	template <type Type>
	class creator
	{};

	template <>
	class creator<FRAMEBUFFER> : public kueken::detail::creator
	{
		friend class object;

	public:
		creator();

		virtual bool validate(){assert(0); return false;}

	private:
		detail::data Data;
	};

	template <>
	class creator<CUSTOM> : public kueken::detail::creator
	{
		friend class object;

	public:
		creator(){}
		void setTexture(slot const & Slot, texture::name const & Texture, glm::uint Level);
		void setRenderbuffer(slot const & Slot, renderbuffer::name const & Renderbuffer);
		void setFramebuffer();

		virtual bool validate(){assert(0); return false;}

	private:
		detail::data Data;
	};

	template <>
	class creator<MULTISAMPLE> : public kueken::detail::creator
	{
		friend class object;

	public:
		creator();
		void setSamples(std::size_t Samples);
		void setSize(glm::uvec2 const & Size);
		//void setSlot(std::size_t Color, std::);

		virtual bool validate(){assert(0); return false;}

	private:
		detail::data Data;
	};

	class object
	{
		friend class renderer;

	public:
		object(creator<FRAMEBUFFER> const & Creator);
		object(creator<CUSTOM> const & Creator);
		~object();

		void bind(target const & Target);

	private:
		GLuint Name;
		std::vector<GLuint> Renderbuffer;
		detail::data Data;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace framebuffer
}//namespace kueken

#include "framebuffer.inl"

#endif//KUEKEN_FRAMEBUFFER_INCLUDED
