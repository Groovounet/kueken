#ifndef KUEKEN_DETAIL_MANAGER_INCLUDED
#define KUEKEN_DETAIL_MANAGER_INCLUDED

#include "common.hpp"

namespace kueken{
namespace detail{

	template <typename NAME, typename OBJECT>
	class manager
	{
	public:
		manager(std::size_t Slot) :
			Binded(Slot)
		{}

		NAME reserve(OBJECT * object);
		void release(NAME const & Name);
		void clear();

		bool isCurrent(std::size_t Slot, NAME const & Name) const;
		NAME getCurrentName(std::size_t Slot = 0) const;
		OBJECT& getCurrentObject(std::size_t Slot = 0) const;
		OBJECT& setCurrentObject(std::size_t Slot, NAME const & Name);
		
		OBJECT& getObject(NAME const & Name) const;

		OBJECT* mapObject(NAME const & Name);
		void unmapObject(NAME const & Name);

	private:
		std::vector<OBJECT*> Created;
		std::vector<OBJECT*> Released;
		std::set<NAME> Mapped;
		std::vector<NAME> Binded;
		std::vector<NAME> Deleted;
	};

	template <typename NAME, typename OBJECT>
	inline OBJECT* manager<NAME, OBJECT>::mapObject(NAME const & Name)
	{
		Mapped.insert(Name);
		return Created[Name()];
	}

	template <typename NAME, typename OBJECT>
	inline void manager<NAME, OBJECT>::unmapObject(NAME const & Name)
	{
		Mapped.erase(Name);
	}

	template <typename NAME, typename OBJECT>
	inline OBJECT& manager<NAME, OBJECT>::getObject(NAME const & Name) const
	{
		assert(this->Created.size() > Name());

		return *Created[Name()];
	}

	template <typename NAME, typename OBJECT>
	inline OBJECT& manager<NAME, OBJECT>::setCurrentObject
	(
		std::size_t Slot, 
		NAME const & Name
	)
	{
		assert(Binded.size() > Slot);

		OBJECT* Object = Created[Name()];
		Binded[Slot] = Name;
		return *Object;
	}

	template <typename NAME, typename OBJECT>
	inline NAME manager<NAME, OBJECT>::reserve
	(
		OBJECT * Object
	)
	{
		NAME Name = NAME::null();

		if(Deleted.empty())
		{
			Name = NAME(Created.size());
			Created.push_back(Object);
		}
		else
		{
			Name = Deleted[Deleted.size() - 1];
			Deleted.pop_back();

			assert(Created.size() > Name() && Created[Name()] == NULL);

			Created[Name()] = Object;
		}

		return Name;
	}

	template <typename NAME, typename OBJECT>
	inline void manager<NAME, OBJECT>::release
	(
		NAME const & Name
	)
	{
		// Check if the object we when to deleted isn't mapped
		assert(std::find(Mapped.begin(), Mapped.end(), Name) == Mapped.end());
		// Check if the object we when to deleted isn't binded
		assert(std::find(Binded.begin(), Binded.end(), Name) == Binded.end());

		Released.push_back(Created[Name()]);
		Created[Name()] = NULL;
		Deleted.push_back(Name);
	}

	template <typename NAME, typename OBJECT>
	inline void manager<NAME, OBJECT>::clear()
	{
		for(std::size_t i = 0; i < Released.size(); ++i)
			delete Released[0];
		Released.clear();
	}

	template <typename NAME, typename OBJECT>
	inline bool manager<NAME, OBJECT>::isCurrent
	(
		std::size_t Slot, 
		NAME const & Name
	) const
	{
		assert(Slot < Binded.size());
		return this->Binded[Slot] == Name;
	}

	template <typename NAME, typename OBJECT>
	inline NAME manager<NAME, OBJECT>::getCurrentName(std::size_t Slot) const
	{
		assert(Slot < Binded.size());
		return this->Binded[Slot];
	}

}//namespace detail
}//namespace kueken

#include "manager.inl"

#endif//KUEKEN_DETAIL_MANAGER_INCLUDED
