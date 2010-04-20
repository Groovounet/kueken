#ifndef KUEKEN_LOAD_PROGRAM_INCLUDED
#define KUEKEN_LOAD_PROGRAM_INCLUDED

#include "../core/renderer.hpp"

namespace load
{
	kueken::program::name program(
		kueken::renderer & Renderer, 
		std::string const & Filename);

}//namespace load

#endif//KUEKEN_LOAD_PROGRAM_INCLUDED
