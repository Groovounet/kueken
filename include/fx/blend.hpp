#ifndef KUEKEN_FX_BLEND_INCLUDED
#define KUEKEN_FX_BLEND_INCLUDED

#include "../core/renderer.hpp"

namespace fx{
namespace blend
{
	class command : public kueken::detail::command
	{
	public:
		command();
		command(
			kueken::renderer* Renderer,
			kueken::blend::name Blend);
		virtual void exec();

	private:
		kueken::renderer* Renderer;
		kueken::blend::name Blend;
	};

}//namespace blend
}//namespace fx

#include "blend.inl"

#endif//KUEKEN_FX_BLEND_INCLUDED
