#include <fx/texture.hpp>
#include <fx/effect.hpp>

namespace fx{
namespace effect{
namespace detail
{
	kueken::blend::function func_cast(char const * String)
	{
		if(!strcmp("zero", String))
			return kueken::blend::ZERO;
		else if(!strcmp("one", String))
			return kueken::blend::ONE;
		else if(!strcmp("src_color", String))
			return kueken::blend::SRC_COLOR;
		else if(!strcmp("one_minus_src_color", String))
			return kueken::blend::ONE_MINUS_SRC_COLOR;
		else if(!strcmp("dst_color", String))
			return kueken::blend::DST_COLOR;
		else if(!strcmp("one_minus_dst_color", String))
			return kueken::blend::ONE_MINUS_DST_COLOR;
		else if(!strcmp("src_alpha", String))
			return kueken::blend::SRC_ALPHA;
		else if(!strcmp("one_minus_src_alpha", String))
			return kueken::blend::ONE_MINUS_SRC_ALPHA;
		else if(!strcmp("dst_alpha", String))
			return kueken::blend::DST_ALPHA;
		else if(!strcmp("one_minus_dst_alpha", String))
			return kueken::blend::ONE_MINUS_DST_ALPHA;
		else if(!strcmp("constant_color", String))
			return kueken::blend::CONSTANT_COLOR;
		else if(!strcmp("one_minus_constant_color", String))
			return kueken::blend::ONE_MINUS_CONSTANT_COLOR;
		else if(!strcmp("constant_alpha", String))
			return kueken::blend::CONSTANT_ALPHA;
		else if(!strcmp("one_minus_constant_alpha", String))
			return kueken::blend::ONE_MINUS_CONSTANT_ALPHA;
		else if(!strcmp("src_alpha_saturate", String))
			return kueken::blend::SRC_ALPHA_SATURATE;
		else 
		{
			return kueken::blend::ONE;
			assert(0);
		}
	}

	kueken::blend::equation equa_cast(char const * String)
	{
		if(!strcmp("add", String))
			return kueken::blend::ADD;
		else if(!strcmp("sub", String))
			return kueken::blend::SUB;
		else if(!strcmp("rev", String))
			return kueken::blend::REV;
		else if(!strcmp("min", String))
			return kueken::blend::MIN;
		else if(!strcmp("max", String))
			return kueken::blend::MAX;
		else 
		{
			return kueken::blend::ADD;
			assert(0);
		}
	}

}//namespace detail
}//namespace effect

}//namespace fx
