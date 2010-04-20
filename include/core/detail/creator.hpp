#ifndef KUEKEN_DETAIL_CREATOR_INCLUDED
#define KUEKEN_DETAIL_CREATOR_INCLUDED

#include "common.hpp"

namespace kueken{
namespace detail{

	class creator
	{
	public:
		virtual bool validate() = 0;

	private:
		std::string log;
	};

}//namespace detail
}//namespace kueken

#endif//KUEKEN_DETAIL_CREATOR_INCLUDED
