#include <load/blend.hpp>

namespace load{
namespace detail
{
	static kueken::blend::function func_cast(char const * String)
	{
		if(!strcmp("zero", String))
			return kueken::blend::ZERO;
		else if(!strcmp("one", String))
			return kueken::blend::ONE;
		else if(!strcmp("src_color", String))
			return kueken::blend::SRC_COLOR;
		else if(!strcmp("one_minus_src_color", String))
			return kueken::blend::ONE_MINUS_SRC_COLOR;
		else if(!strcmp("dst_color", String))
			return kueken::blend::DST_COLOR;
		else if(!strcmp("one_minus_dst_color", String))
			return kueken::blend::ONE_MINUS_DST_COLOR;
		else if(!strcmp("src_alpha", String))
			return kueken::blend::SRC_ALPHA;
		else if(!strcmp("one_minus_src_alpha", String))
			return kueken::blend::ONE_MINUS_SRC_ALPHA;
		else if(!strcmp("dst_alpha", String))
			return kueken::blend::DST_ALPHA;
		else if(!strcmp("one_minus_dst_alpha", String))
			return kueken::blend::ONE_MINUS_DST_ALPHA;
		else if(!strcmp("constant_color", String))
			return kueken::blend::CONSTANT_COLOR;
		else if(!strcmp("one_minus_constant_color", String))
			return kueken::blend::ONE_MINUS_CONSTANT_COLOR;
		else if(!strcmp("constant_alpha", String))
			return kueken::blend::CONSTANT_ALPHA;
		else if(!strcmp("one_minus_constant_alpha", String))
			return kueken::blend::ONE_MINUS_CONSTANT_ALPHA;
		else if(!strcmp("src_alpha_saturate", String))
			return kueken::blend::SRC_ALPHA_SATURATE;
		else 
		{
			return kueken::blend::ONE;
			assert(0);
		}
	}

	static kueken::blend::equation equa_cast(char const * String)
	{
		if(!strcmp("add", String))
			return kueken::blend::ADD;
		else if(!strcmp("sub", String))
			return kueken::blend::SUB;
		else if(!strcmp("rev", String))
			return kueken::blend::REV;
		else if(!strcmp("min", String))
			return kueken::blend::MIN;
		else if(!strcmp("max", String))
			return kueken::blend::MAX;
		else 
		{
			return kueken::blend::ADD;
			assert(0);
		}
	}

}//namespace detail

	kueken::blend::name blend
	(
		kueken::renderer & Renderer,
		void * Pointer
	)
	{
		kueken::blend::creator Creator();
		glm::uint Id = 0;

		TiXmlElement* Blend = reinterpret_cast<TiXmlElement*>(Pointer);

	    TiXmlElement* Source = Blend->FirstChildElement("source");
		if(Source)
		{
			kueken::blend::function Color;
			kueken::blend::function Alpha;

			TiXmlAttribute* Attribute = Source->FirstAttribute();
			do
			{
				if(!strcmp("color", Attribute->Name()))
					Color = detail::func_cast(Attribute->Value());
				else if(!strcmp("alpha", Attribute->Name()))
					Alpha = detail::func_cast(Attribute->Value());
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());

			Creator.setSource(Color, Alpha);
		}

	    TiXmlElement* Destination = Blend->FirstChildElement("destination");
		if(Destination)
		{
			kueken::blend::function Color;
			kueken::blend::function Alpha;

			TiXmlAttribute* Attribute = Destination->FirstAttribute();
			do
			{
				if(!strcmp("color", Attribute->Name()))
					Color = detail::func_cast(Attribute->Value());
				else if(!strcmp("alpha", Attribute->Name()))
					Alpha = detail::func_cast(Attribute->Value());
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());

			Creator.setDestination(Color, Alpha);
		}

	    TiXmlElement* Equation = Blend->FirstChildElement("equation");
		if(Equation)
		{
			kueken::blend::equation Color;
			kueken::blend::equation Alpha;

			TiXmlAttribute* Attribute = Equation->FirstAttribute();
			do
			{
				if(!strcmp("color", Attribute->Name()))
					Color = detail::equa_cast(Attribute->Value());
				else if(!strcmp("alpha", Attribute->Name()))
					Alpha = detail::equa_cast(Attribute->Value());
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());

			Creator.setEquation(Color, Alpha);
		}

		TiXmlElement* ColorMask = Blend->FirstChildElement("color-mask");
		if(ColorMask)
		//do
		{
			kueken::blend::slot Slot = kueken::blend::SLOT0;
			glm::bvec4 Mask(false);
			TiXmlAttribute* Attribute = ColorMask->FirstAttribute();
			do
			{
				if(!strcmp("slot", Attribute->Name()))
					Slot = kueken::blend::slot(atoi(Attribute->Value()));
				else if(!strcmp("r", Attribute->Name()))
					Mask.r = std::string(Attribute->Value()) == std::string("true") ? true : false;
				else if(!strcmp("g", Attribute->Name()))
					Mask.g = std::string(Attribute->Value()) == std::string("true") ? true : false;
				else if(!strcmp("b", Attribute->Name()))
					Mask.b = std::string(Attribute->Value()) == std::string("true") ? true : false;
				else if(!strcmp("a", Attribute->Name()))
					Mask.a = std::string(Attribute->Value()) == std::string("true") ? true : false;
				else
					assert(0);
			}    
			while(Attribute = Attribute->Next());

			Creator.setColorMask(Slot, Mask);
		}
		//while(ColorMask = ColorMask->NextSiblingElement());

		TiXmlElement* DepthMask = Blend->FirstChildElement("depth-mask");
		if(DepthMask)
		{
			bool Depth = false;
			TiXmlAttribute* Attribute = DepthMask->FirstAttribute();
			do
			{
				if(!strcmp("depth", Attribute->Name()))
					Depth = std::string(Attribute->Value()) == std::string("true") ? true : false;
				else
					assert(0);
			}    
			while(Attribute = Attribute->Next());

			Creator.setDepthMask(Depth);
		}

		TiXmlElement* StencilMask = Blend->FirstChildElement("stencil-mask");
		if(StencilMask)
		{
			glm::uint Front = 0;
			glm::uint Back = 0;
			TiXmlAttribute* Attribute = StencilMask->FirstAttribute();
			do
			{
				if(!strcmp("front", Attribute->Name()))
					Front = atoi(Attribute->Value());
				else if(!strcmp("back", Attribute->Name()))
					Back = atoi(Attribute->Value());
				else
					assert(0);
			}    
			while(Attribute = Attribute->Next());

			Creator.setStencilMask(Front, Back);
		}

		TiXmlElement* AlphaToCoverage = Blend->FirstChildElement("alpha-to-coverage");
		if(AlphaToCoverage)
		{
			bool Enabled = false;
			TiXmlAttribute* Attribute = AlphaToCoverage->FirstAttribute();
			do
			{
				if(!strcmp("enabled", Attribute->Name()))
					Enabled = std::string(Attribute->Value()) == std::string("true") ? true : false;
				else
					assert(0);
			}    
			while(Attribute = Attribute->Next());

			Creator.setAlphaToCoverage(Enabled);
		}

		TiXmlElement* BlendEnable = Blend->FirstChildElement("blend");
		if(BlendEnable)
		//do
		{
			bool Enabled = false;
			kueken::blend::slot Slot = kueken::blend::SLOT0;

			TiXmlAttribute* Attribute = BlendEnable->FirstAttribute();
			do
			{
				if(!strcmp("enabled", Attribute->Name()))
					Enabled = std::string(Attribute->Value()) == std::string("true") ? true : false;
				else if(!strcmp("slot", Attribute->Name()))
					Slot = kueken::blend::slot(atoi(Attribute->Value()));
				else
					assert(0);
			}    
			while(Attribute = Attribute->Next());

			Creator.setBlend(Slot, Enabled);
		}
		//while(BlendEnable = BlendEnable->NextSiblingElement());

		return Renderer.create(Creator);
	}

	kueken::blend::name blend
	(
		kueken::renderer & Renderer,
		std::string const & Filename
	)
	{
		TiXmlDocument Document(Filename.c_str());
		Document.LoadFile();
		TiXmlElement* Root = Document.RootElement();
		if(std::string(Root->Value()) != std::string("blend"))
			return kueken::blend::name::Null();

		return blend(Renderer, (void*)Root);
	}

}//namespace load
