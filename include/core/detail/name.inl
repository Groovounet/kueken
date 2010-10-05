namespace kueken{
namespace detail
{
	template <typename T>
	inline name<T>::name() :
		value(0xFFFFFFFF)
	{}

	template <typename T>
	inline name<T>::name(bool Null) :
		value(0xFFFFFFFF)
	{}

	template <typename T>
	inline name<T>::name(typename name<T>::size_type const & Value) :
		value(Value)
	{}

	template <typename T>
	inline name<T>::name(name const & Name) :
		value(Name.value)
	{}

	template <typename T>
	inline name<T>::~name()
	{}

	template <typename T>
	inline bool name<T>::operator!() const
	{
		return value == 0xFFFFFFFF;
	}

	template <typename T>
	inline typename name<T>::size_type name<T>::operator()() const
	{
		return value;
	}

	template <typename T>
	inline name<T> & name<T>::operator=(name<T> const & Name)
	{
		value = Name.value;
		return *this;
	}

	template <typename T>
	inline name<T> name<T>::null()
	{
		return name(true);
	}

	template <typename T>
	inline bool name<T>::operator==(name<T> const & Name) const
	{
		return value == Name.value;
	}

	template <typename T>
	inline bool name<T>::operator<(name<T> const & Name) const
	{
		return value < Name.value;
	}

}//namespace detail
}//namespace kueken
