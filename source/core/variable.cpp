#include <core/variable.hpp>

namespace
{


}//namespace

namespace kueken{
namespace variable
{

	void creator::setName(std::string const & Name)
	{
		this->Name = Name;
	}

	void creator::setType(type Type)
	{
		this->Type = Type;
	}

	void creator::setSize(std::size_t Size)
	{
		this->Size = Size;
	}

	object::object
	(
		creator const & Creator
	) :
		Name(0)
	{

	}

	object::~object()
	{

	}

}//namespace variable
}//namespace kueken
