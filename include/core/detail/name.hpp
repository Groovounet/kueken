#ifndef KUEKEN_DETAIL_NAME_INCLUDED
#define KUEKEN_DETAIL_NAME_INCLUDED

#include "common.hpp"

namespace kueken{
namespace detail
{
	template <typename T>
	class name : public boost::totally_ordered<name<T> >
	{
	public:
		typedef T object_type;
		typedef std::size_t size_type;

	public:
		name();
		name(name<T> const & Name);
		virtual ~name();

		bool operator<(name<T> const & Name) const;
		bool operator==(name<T> const & Name) const;
		bool operator!() const;

		name<T> & operator=(name<T> const & Name);

		static name<T> null();

	//private:
		explicit name(size_type const & Value);
		explicit name(bool Null); // Detail for static Null function

		size_type operator()() const; 

	private:
		size_type value;
	};

}//namespace detail
}//namespace kueken

#include "name.inl"

#endif//KUEKEN_DETAIL_NAME_INCLUDED
