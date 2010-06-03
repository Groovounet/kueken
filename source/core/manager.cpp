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
		Buffer(buffer::TARGET_MAX),
		Clear(0),
		Draw(0),
		Image(image::SLOT_MAX),
		Program(1),
		Query(0),
		Rasterizer(1),
		ReadPixels(0),
		Renderbuffer(0),
		Rendertarget(1),
		Sampler(sampler::SLOT_MAX),
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
