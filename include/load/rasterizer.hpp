#ifndef KUEKEN_LOAD_RASTERIZER_INCLUDED
#define KUEKEN_LOAD_RASTERIZER_INCLUDED

#include "../core/renderer.hpp"

namespace load
{
	kueken::rasterizer::name rasterizer(
		kueken::renderer & Renderer, 
		std::string const & Filename);

}//namespace load

#endif//KUEKEN_LOAD_RASTERIZER_INCLUDED
