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
		Buffer(buffer::TARGET_MAX),
		Clear(0),
		Draw(0),
		Layout(1),
		Program(1),
		Query(0),
		Rasterizer(1),
		ReadPixels(0),
		Renderbuffer(0),
		Framebuffer(1),
		Sampler(sampler::SLOT_MAX),
		Test(1),
		Texture(texture::SLOT_MAX)
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
