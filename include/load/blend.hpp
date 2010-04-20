#ifndef KUEKEN_LOAD_BLEND_INCLUDED
#define KUEKEN_LOAD_BLEND_INCLUDED

#include "../core/renderer.hpp"

namespace load
{
	kueken::blend::name blend(
		kueken::renderer & Renderer, 
		std::string const & Filename);

}//namespace load

#endif//KUEKEN_LOAD_BLEND_INCLUDED
