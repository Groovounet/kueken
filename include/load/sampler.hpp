#ifndef KUEKEN_LOAD_SAMPLER_INCLUDED
#define KUEKEN_LOAD_SAMPLER_INCLUDED

#include "../core/renderer.hpp"

namespace load
{
	kueken::sampler::name sampler(
		kueken::renderer & Renderer, 
		std::string const & Filename);

}//namespace load

#endif//KUEKEN_LOAD_SAMPLER_INCLUDED
