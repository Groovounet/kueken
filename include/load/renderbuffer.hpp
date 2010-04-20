#ifndef KUEKEN_LOAD_RENDERBUFFER_INCLUDED
#define KUEKEN_LOAD_RENDERBUFFER_INCLUDED

#include "../core/renderer.hpp"

namespace load
{
	kueken::renderbuffer::name renderbuffer(
		kueken::renderer & Renderer, 
		std::string const & Filename);

}//namespace load

#endif//KUEKEN_LOAD_RENDERBUFFER_INCLUDED
