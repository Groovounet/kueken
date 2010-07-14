#include <core/manager.hpp>

namespace
{

}

namespace kueken
{
	manager* manager::Manager = 0;

	manager::manager() :
 		Assembler(1),
		Blend(1),
		Blit(0),
		Buffer(8),
		Clear(0),
		Draw(0),
		Image(8),
		Program(1),
		Query(0),
		Rasterizer(1),
		ReadPixels(0),
		Renderbuffer(0),
		Rendertarget(1),
		Sampler(8),
		Test(1)
	{}

	manager& manager::instance()
	{
		if(Manager == 0)
			Manager = new manager;
		return *Manager;
	}

	void manager::destroy()
	{
		delete Manager;
		Manager = 0;
	}

}//namespace kueken
