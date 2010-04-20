#ifndef KUEKEN_DETAIL_NAME_INCLUDED
#define KUEKEN_DETAIL_NAME_INCLUDED

#include "common.hpp"

namespace kueken{
namespace detail
{
	//template <typename OBJECT>
	//class CObjectManager;

	template <typename T>
	class name : public boost::totally_ordered<name<T> >
	{
		//friend class CObjectManager<T, CName<T> >;

	public:
		typedef T object_type;

	public:
		name();
		name(name<T> const & Name);
		virtual ~name();

		bool operator<(name<T> const & Name) const;
		bool operator==(name<T> const & Name) const;
		bool operator!() const;

		name<T>& operator=(name<T> const & Name);

		static name<T> Null();

	//private:
		explicit name(std::size_t Value);
		explicit name(bool Null); // Detail for static Null function

		std::size_t operator()() const; 

	private:
		std::size_t value;
	};

}//namespace detail
}//namespace kueken

#include "name.inl"

#endif//KUEKEN_DETAIL_NAME_INCLUDED
