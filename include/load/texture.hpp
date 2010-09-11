#ifndef KUEKEN_LOAD_IMAGE_INCLUDED
#define KUEKEN_LOAD_IMAGE_INCLUDED

#include "../core/renderer.hpp"

namespace load
{
	kueken::image::name image(
		kueken::renderer & Renderer, 
		std::string const & Filename);

}//namespace load

#endif//KUEKEN_LOAD_IMAGE_INCLUDED


