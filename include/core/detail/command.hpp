#ifndef KUEKEN_DETAIL_COMMAND_INCLUDED
#define KUEKEN_DETAIL_COMMAND_INCLUDED

namespace kueken{
namespace detail
{
	struct command
	{
		virtual void exec() = 0;
	};

}//namespace detail
}//namespace kueken

#endif//KUEKEN_DETAIL_COMMAND_INCLUDED
