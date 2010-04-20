#ifndef KUEKEN_LOAD_TEST_INCLUDED
#define KUEKEN_LOAD_TEST_INCLUDED

#include "../core/renderer.hpp"

namespace load
{
	kueken::test::name test(
		kueken::renderer & Renderer, 
		std::string const & Filename);

}//namespace load

#endif//KUEKEN_LOAD_TEST_INCLUDED
