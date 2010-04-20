#ifndef KUEKEN_FX_PROGRAM_INCLUDED
#define KUEKEN_FX_PROGRAM_INCLUDED

#include "../core/renderer.hpp"

namespace fx{
namespace program
{
	//kueken::program::name load(kueken::renderer & Renderer, void * Pointer);
	//kueken::program::name load(kueken::renderer & Renderer, std::string const & Filename);

	class command : public kueken::detail::command
	{
	public:
		command();
		command(
			kueken::renderer* Renderer,
			kueken::program::name Program);
		virtual void exec();

	private:
		kueken::renderer* Renderer;
		kueken::program::name Program;
	};

}//namespace program
}//namespace fx

//#include "program.inl"

#endif//KUEKEN_FX_PROGRAM_INCLUDED
