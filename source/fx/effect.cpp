#include <fx/effect.hpp>

namespace fx{
namespace effect
{
	object::object
	(
		kueken::renderer & Renderer, 
		std::string const Filename
	) : 
		Renderer(&Renderer)
	{
		TiXmlDocument Document(Filename.c_str());
		Document.LoadFile();
		TiXmlElement* Root = Document.RootElement();
		if(std::string(Root->Value()) != std::string("effect"))
			return;

		load((void*)Root);
	}

	void object::load(void* Pointer)
	{
		TiXmlElement* Effect = reinterpret_cast<TiXmlElement*>(Pointer);

	    TiXmlElement* Child = Effect->FirstChildElement();
		do
		{
			if(std::string(Child->Value()) == std::string("assets"))
			{
				loadAssets(Child);
			}
			//else if(std::string(Child->Value()) == std::string("pass"))
			//{
			//	loadPass(Child);
			//}
			else
			{
				assert(0);
			}
		}
		while (Child = Child->NextSiblingElement());
	}

	void object::loadAssets(void* Pointer)
	{
		TiXmlElement* Assets = reinterpret_cast<TiXmlElement*>(Pointer);

	    TiXmlElement* Child = Assets->FirstChildElement();
		do
		{
			if(std::string(Child->Value()) == std::string("blend"))
			{
				kueken::blend::name Name = loadBlend(Child);
			}
			else if(std::string(Child->Value()) == std::string("image"))
			{
				loadImage(Child);
			}
			else if(std::string(Child->Value()) == std::string("program"))
			{
				kueken::program::name Name = loadProgram(Child);
			}
			else if(std::string(Child->Value()) == std::string("rasterizer"))
			{
				kueken::rasterizer::name Name = loadRasterizer(Child);
				//Rasterizers.insert(std::make_pair(Name.id(), Name));
			}
			else if(std::string(Child->Value()) == std::string("sampler"))
			{
				loadSampler(Child);
			}
			else if(std::string(Child->Value()) == std::string("test"))
			{
				kueken::test::name Name = loadTest(Child);
				//Tests.insert(std::make_pair(Name.id(), Name));
			}
			else
			{
				assert(0);
			}
		}
		while (Child = Child->NextSiblingElement());
	}

	kueken::assembler::name object::findAssembler(glm::uint Id) const
	{
		std::map<glm::uint, kueken::assembler::name>::const_iterator it = Assemblers.find(Id);
		if(it == Assemblers.end())
			return kueken::assembler::name::Null();
		else
			return it->second;
	}

	kueken::blend::name object::findBlend(glm::uint Id) const
	{
		std::map<glm::uint, kueken::blend::name>::const_iterator it = Blends.find(Id);
		if(it == Blends.end())
			return kueken::blend::name::Null();
		else
			return it->second;
	}

	kueken::blit::name object::findBlit(glm::uint Id) const
	{
		std::map<glm::uint, kueken::blit::name>::const_iterator it = Blits.find(Id);
		if(it == Blits.end())
			return kueken::blit::name::Null();
		else
			return it->second;
	}

	kueken::buffer::name object::findBuffer(glm::uint Id) const
	{
		std::map<glm::uint, kueken::buffer::name>::const_iterator it = Buffers.find(Id);
		if(it == Buffers.end())
			return kueken::buffer::name::Null();
		else
			return it->second;
	}

	kueken::clear::name object::findClear(glm::uint Id) const
	{
		std::map<glm::uint, kueken::clear::name>::const_iterator it = Clears.find(Id);
		if(it == Clears.end())
			return kueken::clear::name::Null();
		else
			return it->second;
	}

	kueken::draw::name object::findDraw(glm::uint Id) const
	{
		std::map<glm::uint, kueken::draw::name>::const_iterator it = Draws.find(Id);
		if(it == Draws.end())
			return kueken::draw::name::Null();
		else
			return it->second;
	}

	kueken::image::name object::findImage(glm::uint Id) const
	{
		std::map<glm::uint, kueken::image::name>::const_iterator it = Images.find(Id);
		if(it == Images.end())
			return kueken::image::name::Null();
		else
			return it->second;
	}

	kueken::program::name object::findProgram(glm::uint Id) const
	{
		std::map<glm::uint, kueken::program::name>::const_iterator it = Programs.find(Id);
		if(it == Programs.end())
			return kueken::program::name::Null();
		else
			return it->second;
	}

	kueken::query::name object::findQuery(glm::uint Id) const
	{
		std::map<glm::uint, kueken::query::name>::const_iterator it = Querys.find(Id);
		if(it == Querys.end())
			return kueken::query::name::Null();
		else
			return it->second;
	}

	kueken::rasterizer::name object::findRasterizer(glm::uint Id) const
	{
		std::map<glm::uint, kueken::rasterizer::name>::const_iterator it = Rasterizers.find(Id);
		if(it == Rasterizers.end())
			return kueken::rasterizer::name::Null();
		else
			return it->second;
	}

	kueken::readpixels::name object::findReadpixel(glm::uint Id) const
	{
		std::map<glm::uint, kueken::readpixels::name>::const_iterator it = Readpixels.find(Id);
		if(it == Readpixels.end())
			return kueken::readpixels::name::Null();
		else
			return it->second;
	}

	kueken::renderbuffer::name object::findRenderbuffer(glm::uint Id) const
	{
		std::map<glm::uint, kueken::renderbuffer::name>::const_iterator it = Renderbuffers.find(Id);
		if(it == Renderbuffers.end())
			return kueken::renderbuffer::name::Null();
		else
			return it->second;
	}

	kueken::rendertarget::name object::findRendertarget(glm::uint Id) const
	{
		std::map<glm::uint, kueken::rendertarget::name>::const_iterator it = Rendertargets.find(Id);
		if(it == Rendertargets.end())
			return kueken::rendertarget::name::Null();
		else
			return it->second;
	}

	kueken::sampler::name object::findSampler(glm::uint Id) const
	{
		std::map<glm::uint, kueken::sampler::name>::const_iterator it = Samplers.find(Id);
		if(it == Samplers.end())
			return kueken::sampler::name::Null();
		else
			return it->second;
	}

	kueken::test::name object::findTest(glm::uint Id) const
	{
		std::map<glm::uint, kueken::test::name>::const_iterator it = Tests.find(Id);
		if(it == Tests.end())
			return kueken::test::name::Null();
		else
			return it->second;
	}

}//namespace effect
}//namespace fx
