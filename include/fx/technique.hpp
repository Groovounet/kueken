#ifndef KUEKEN_FX_PASS_INCLUDED
#define KUEKEN_FX_PASS_INCLUDED

#include "../core/detail/detail.hpp"
#include "../core/renderer.hpp"

namespace fx{
namespace pass
{
	class object
	{
	public:
		void load(kueken::renderer & Renderer, void const * const Element);

	private:
	};

}//namespace effect
}//namespace fx

//#include "pass.inl"

#endif//KUEKEN_FX_PASS_INCLUDED
