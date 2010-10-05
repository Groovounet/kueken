#include <core/manager.hpp>

namespace
{

}

namespace kueken
{
	std::unique_ptr<manager> manager::Manager(nullptr);

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
		if(Manager.get() == nullptr)
			Manager.reset(new manager);
		return * Manager;
	}

	void manager::destroy()
	{
		Manager.reset();
	}

}//namespace kueken
