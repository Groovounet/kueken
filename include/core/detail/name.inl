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
	inline name<T>::name(std::size_t Value) :
		value(Value)
	{}

	template <typename T>
	inline name<T>::name(const name& Name) :
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
	inline std::size_t name<T>::operator()() const
	{
		return value;
	}

	template <typename T>
	inline name<T>& name<T>::operator=(const name<T>& Name)
	{
		value = Name.value;
		return *this;
	}

	template <typename T>
	inline name<T> name<T>::Null()
	{
		return name(true);
	}

	template <typename T>
	inline bool name<T>::operator==(const name<T>& Name) const
	{
		return value == Name.value;
	}

	template <typename T>
	inline bool name<T>::operator<(const name<T>& Name) const
	{
		return value < Name.value;
	}

}//namespace detail
}//namespace kueken
