#ifndef KUEKEN_FX_TEST_INCLUDED
#define KUEKEN_FX_TEST_INCLUDED

#include "../core/renderer.hpp"

namespace fx{
namespace test
{
	//kueken::test::name load(kueken::renderer & Renderer, void * Pointer);
	//kueken::test::name load(kueken::renderer & Renderer, std::string const & Filename);

	class command : public kueken::detail::command
	{
	public:
		command();
		command(
			kueken::renderer* Renderer,
			kueken::test::name Test);
		virtual void exec();

	private:
		kueken::renderer* Renderer;
		kueken::test::name Test;
	};

}//namespace test
}//namespace fx

#include "test.inl"

#endif//KUEKEN_FX_TEST_INCLUDED

