#ifndef KUEKEN_FX_RASTERIZER_INCLUDED
#define KUEKEN_FX_RASTERIZER_INCLUDED

#include "../core/renderer.hpp"

namespace fx{
namespace rasterizer
{
	//kueken::rasterizer::name load(kueken::renderer & Renderer, void * Pointer);
	//kueken::rasterizer::name load(kueken::renderer & Renderer, std::string const & Filename);

	class command : public kueken::detail::command
	{
	public:
		command();
		command(
			kueken::renderer* Renderer,
			kueken::rasterizer::name Rasterizer);
		virtual void exec();

	private:
		kueken::renderer* Renderer;
		kueken::rasterizer::name Rasterizer;
	};

}//namespace rasterizer
}//namespace fx

#include "rasterizer.inl"

#endif//KUEKEN_FX_RASTERIZER_INCLUDED
