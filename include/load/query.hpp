#ifndef KUEKEN_LOAD_QUERY_INCLUDED
#define KUEKEN_LOAD_QUERY_INCLUDED

#include "../core/renderer.hpp"

namespace load
{
	kueken::query::name query(
		kueken::renderer & Renderer, 
		std::string const & Filename);

}//namespace load

#endif//KUEKEN_LOAD_QUERY_INCLUDED
