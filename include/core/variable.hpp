#ifndef KUEKEN_VARIABLE_INCLUDED
#define KUEKEN_VARIABLE_INCLUDED

#include "detail/detail.hpp"

namespace kueken{
namespace variable{

namespace detail{

	struct data
	{
		data();

	};

}//namespace detail

	enum type
	{
		UNIFORM,
		ATTRIB,
		SAMPLER,
		TYPE_MAX
	};

	class creator// : public kueken::detail::creator
	{
		friend class object;

	public:
		void setProgram();
		void setName(std::string const & Name);
		void setType(type Type);
		void setSize(std::size_t Size);

		virtual bool validate(){}

	private:
		std::string Name;
		type Type;
		std::size_t Size;
	};

	class object
	{
	public:
		object(creator const & Creator);
		~object();

		void set(std::size_t Offset, std::size_t Size, void const * const Data);

	private:
		std::vector<glm::byte> Data;
		GLuint Name;
		std::string Log;
	};

	typedef kueken::detail::name<creator> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace variable
}//namespace kueken

#include "variable.inl"

#endif//KUEKEN_VARIABLE_INCLUDED
