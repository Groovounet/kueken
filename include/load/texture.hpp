#ifndef KUEKEN_LOAD_IMAGE_INCLUDED
#define KUEKEN_LOAD_IMAGE_INCLUDED

#include "../core/renderer.hpp"

namespace load
{
	kueken::texture::name texture(
		kueken::renderer & Renderer, 
		std::string const & Filename);

}//namespace load

#endif//KUEKEN_LOAD_IMAGE_INCLUDED


