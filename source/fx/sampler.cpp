#include <fx/sampler.hpp>
#include <fx/effect.hpp>

namespace fx{
namespace effect{
namespace detail
{
	kueken::sampler::filter filter_cast(char const * String)
	{
		if(!strcmp("nearest", String))
			return kueken::sampler::NEAREST;
		else if(!strcmp("bilinear", String))
			return kueken::sampler::BILINEAR;
		else if(!strcmp("trilinear", String))
			return kueken::sampler::TRILINEAR;
		else 
		{
			return kueken::sampler::NEAREST;
			assert(0);
		}
	}

	kueken::sampler::wrap wrap_cast(char const * String)
	{
		if(!strcmp("clamp-to-border", String))
			return kueken::sampler::CLAMP_TO_BORDER;
		else if(!strcmp("clamp-to-edge", String))
			return kueken::sampler::CLAMP_TO_EDGE;
		else if(!strcmp("mirrored-repeat", String))
			return kueken::sampler::MIRRORED_REPEAT;
		else if(!strcmp("repeat", String))
			return kueken::sampler::REPEAT;
		else 
		{
			return kueken::sampler::REPEAT;
			assert(0);
		}
	}

	kueken::sampler::swizzle swizzle_cast(char const * String)
	{
		if(!strcmp("zero", String))
			return kueken::sampler::ZERO;
		else if(!strcmp("one", String))
			return kueken::sampler::ONE;
		else if(!strcmp("r", String))
			return kueken::sampler::RED;
		else if(!strcmp("g", String))
			return kueken::sampler::GREEN;
		else if(!strcmp("b", String))
			return kueken::sampler::BLUE;
		else if(!strcmp("a", String))
			return kueken::sampler::ALPHA;
		else 
		{
			return kueken::sampler::ONE;	
			assert(0);
		}
	}

}//namespace detail

	kueken::sampler::name object::loadSampler
	(
		void * Pointer
	)
	{
		kueken::sampler::creator Creator;
		glm::uint Id = 0;

		TiXmlElement* Sampler = reinterpret_cast<TiXmlElement*>(Pointer);
		{
			TiXmlAttribute* Attribute = Sampler->FirstAttribute();
			do
			{
				if(!strcmp("id", Attribute->Name()))
					Id = atoi(Attribute->Value());
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());
		}

	    TiXmlElement* Child = Sampler->FirstChildElement();
		do
		{
			if(std::string(Child->Value()) == std::string("filter"))
			{
				kueken::sampler::filter Filter = kueken::sampler::NEAREST;

				TiXmlAttribute* Attribute = Child->FirstAttribute();
				do
				{
					if(!strcmp("value", Attribute->Name()))
						Filter = detail::filter_cast(Attribute->Value());
					else
						assert(0);
				}    
				while (Attribute = Attribute->Next());

				Creator.setFilter(Filter);
			}
			else if(std::string(Child->Value()) == std::string("wrap"))
			{
				kueken::sampler::wrap WrapS = kueken::sampler::REPEAT;
				kueken::sampler::wrap WrapT = kueken::sampler::REPEAT;
				kueken::sampler::wrap WrapQ = kueken::sampler::REPEAT;

				TiXmlAttribute* Attribute = Child->FirstAttribute();
				do
				{
					if(!strcmp("s", Attribute->Name()))
						WrapS = detail::wrap_cast(Attribute->Value());
					else if(!strcmp("t", Attribute->Name()))
						WrapT = detail::wrap_cast(Attribute->Value());
					else if(!strcmp("q", Attribute->Name()))
						WrapQ = detail::wrap_cast(Attribute->Value());
					else
						assert(0);
				}    
				while (Attribute = Attribute->Next());

				Creator.setWrap(WrapS, WrapT, WrapQ);
			}
			else if(std::string(Child->Value()) == std::string("anisotropy"))
			{
				float Anisotropy = 1.0f;

				TiXmlAttribute* Attribute = Child->FirstAttribute();
				do
				{
					if(!strcmp("value", Attribute->Name()))
						Anisotropy = float(atof(Attribute->Value()));
					else
						assert(0);
				}    
				while (Attribute = Attribute->Next());

				Creator.setAnisotropy(Anisotropy);
			}
			else if(std::string(Child->Value()) == std::string("lod"))
			{
				float Min = -1000.f;
				float Max =  1000.f;
				float Bias = 0.0f;

				TiXmlAttribute* Attribute = Child->FirstAttribute();
				do
				{
					if(!strcmp("min", Attribute->Name()))
						Min = float(atof(Attribute->Value()));
					else if(!strcmp("max", Attribute->Name()))
						Max = float(atof(Attribute->Value()));
					else if(!strcmp("bias", Attribute->Name()))
						Bias = float(atof(Attribute->Value()));
					else
						assert(0);
				}    
				while (Attribute = Attribute->Next());

				Creator.setLod(Min, Max, Bias);
			}
			else
			{
				assert(0);
			}
		}
		while (Child = Child->NextSiblingElement());

		kueken::sampler::name Name = Renderer->create(Creator);
		Samplers.insert(std::make_pair(Id, Name));
		return Name;
	}

	kueken::sampler::name object::loadSampler
	(
		std::string const & Filename
	)
	{
		TiXmlDocument Document(Filename.c_str());
		Document.LoadFile();
		TiXmlElement* Root = Document.RootElement();
		if(std::string(Root->Value()) != std::string("sampler"))
			return kueken::sampler::name::Null();

		return loadSampler((void*)Root);
	}

	//command::command() :
	//	Renderer(0),
	//	Sampler(kueken::sampler::name::Null())
	//{}

	//command::command
	//(
	//	kueken::renderer* Renderer,
	//	kueken::sampler::name Sampler
	//) :
	//	Renderer(Renderer),
	//	Sampler(Sampler)
	//{}

	//void command::exec()
	//{
	//	Renderer->bind(Sampler);
	//}

}//namespace effect
}//namespace fx
