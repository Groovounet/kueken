#ifndef KUEKEN_FX_TEXTURE_INCLUDED
#define KUEKEN_FX_TEXTURE_INCLUDED

#include "effect.hpp"

namespace fx{
namespace texture
{
//	kueken::texture::name load(effect & Effect, void * Pointer);
//	kueken::texture::name load(effect & Effect, std::string const & Filename);

	class command : public kueken::detail::command
	{
	public:
		command();
		command(
			kueken::renderer* Renderer,
			kueken::texture::name Texture,
			kueken::texture::slot Slot);
		virtual void exec();

	private:
		kueken::renderer* Renderer;
		kueken::texture::name Texture;
		kueken::texture::slot Slot;
	};

}//namespace texture
}//namespace fx

#include "texture.inl"

#endif//KUEKEN_FX_TEXTURE_INCLUDED


