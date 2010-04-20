#ifndef KUEKEN_LOAD_RENDERTARGET_INCLUDED
#define KUEKEN_LOAD_RENDERTARGET_INCLUDED

#include "../core/renderer.hpp"

namespace load
{
	kueken::rendertarget::name rendertarget(
		kueken::renderer & Renderer, 
		std::string const & Filename);

}//namespace load

#endif//KUEKEN_LOAD_RENDERTARGET_INCLUDED
