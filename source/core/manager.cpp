#include <core/manager.hpp>

namespace
{

}

namespace kueken
{
	manager * manager::Manager(nullptr);

	manager::manager() :
		Blend(1),
		Blit(0),
		Buffer(8),
		Clear(0),
		Draw(0),
		Layout(1),
		Program(1),
		Query(0),
		Rasterizer(1),
		ReadPixels(0),
		Renderbuffer(0),
		Framebuffer(1),
		Sampler(8),
		Test(1),
		Texture(8)
	{}

	manager & manager::instance()
	{
		if(Manager == 0)
			Manager = new manager;
		return * Manager;
	}

	void manager::destroy()
	{
		delete Manager;
		Manager = nullptr;
	}

}//namespace kueken
