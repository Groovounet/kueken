#include <load/rasterizer.hpp>

//#include <xercesc/util/PlatformUtils.hpp>
//#include <xercesc/dom/DOM.hpp>
//#include <xercesc/framework/StdOutFormatTarget.hpp>
//#include <xercesc/framework/LocalFileFormatTarget.hpp>
//#include <xercesc/parsers/XercesDOMParser.hpp>
//#include <xercesc/util/XMLUni.hpp>
//
//XERCES_CPP_NAMESPACE_USE

namespace load{
namespace detail
{
	kueken::rasterizer::name loadPolygon
	(
		kueken::renderer & Renderer, 
		TiXmlElement* Rasterizer
	)
	{
		kueken::rasterizer::creator<kueken::rasterizer::POLYGON> Creator;

	    TiXmlElement* Viewport = Rasterizer->FirstChildElement("viewport");
		if(Viewport)
		{
			glm::ivec4 Value(0);
			TiXmlAttribute* Attribute = Viewport->FirstAttribute();
			do
			{
				if(!strcmp("x", Attribute->Name()))
					Value.x = atoi(Attribute->Value());
				else if(!strcmp("y", Attribute->Name()))
					Value.y = atoi(Attribute->Value());
				else if(!strcmp("w", Attribute->Name()))
					Value.z = atoi(Attribute->Value());
				else if(!strcmp("h", Attribute->Name()))
					Value.w = atoi(Attribute->Value());
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());

			Creator.setViewport(Value);
		}

	    TiXmlElement* Multisample = Rasterizer->FirstChildElement("multisample");
		if(Multisample)
		{
			TiXmlAttribute* Attribute = Multisample->FirstAttribute();
			do
			{
				if(!strcmp("enable", Attribute->Name()))
					Creator.setMultisample(std::string(Attribute->Value()) == std::string("true") ? true : false);
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());
		}

	    TiXmlElement* Scissor = Rasterizer->FirstChildElement("scissor");
		if(Scissor)
		{
			bool Enable = false;
			glm::ivec4 Rect(0);
			TiXmlAttribute* Attribute = Scissor->FirstAttribute();
			do
			{
				if(!strcmp("enable", Attribute->Name()))
					Enable = std::string(Attribute->Value()) == std::string("true") ? true : false;
				else if(!strcmp("x", Attribute->Name()))
					Rect.x = atoi(Attribute->Value());
				else if(!strcmp("y", Attribute->Name()))
					Rect.y = atoi(Attribute->Value());
				else if(!strcmp("w", Attribute->Name()))
					Rect.z = atoi(Attribute->Value());
				else if(!strcmp("h", Attribute->Name()))
					Rect.w = atoi(Attribute->Value());
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());

			Creator.setScissor(Enable, Rect);
		}

	    TiXmlElement* Frontface = Rasterizer->FirstChildElement("frontface");
		if(Frontface)
		{
			kueken::rasterizer::front Face = kueken::rasterizer::CCW;

			TiXmlAttribute* Attribute = Frontface->FirstAttribute();
			do
			{
				if(!strcmp("face", Attribute->Name()))
					Face = std::string(Attribute->Value()) == std::string("ccw") ? kueken::rasterizer::CCW : kueken::rasterizer::CW;
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());

			Creator.setFrontFace(Face);
		}

	    TiXmlElement* Cullface = Rasterizer->FirstChildElement("cullface");
		if(Cullface)
		{
			kueken::rasterizer::cull Face = kueken::rasterizer::CULL_NONE;

			TiXmlAttribute* Attribute = Cullface->FirstAttribute();
			do
			{
				if(!strcmp("face", Attribute->Name()))
				{
					if(std::string(Attribute->Value()) == std::string("none"))
						Face = kueken::rasterizer::CULL_NONE;
					else if(std::string(Attribute->Value()) == std::string("front"))
						Face = kueken::rasterizer::FRONT;
					else if(std::string(Attribute->Value()) == std::string("back"))
						Face = kueken::rasterizer::BACK;
					else if(std::string(Attribute->Value()) == std::string("both"))
						Face = kueken::rasterizer::FRONT_AND_BACK;
				}
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());

			Creator.setCullface(Face);
		}

	    TiXmlElement* Offset = Rasterizer->FirstChildElement("offset");
		if(Offset)
		{
			float Factor = 0.0f;
			float Units = 0.0f;

			TiXmlAttribute* Attribute = Offset->FirstAttribute();
			do
			{
				if(!strcmp("factor", Attribute->Name()))
					Factor = float(atof(Attribute->Value()));
				else if(!strcmp("units", Attribute->Name()))
					Units = float(atof(Attribute->Value()));
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());

			Creator.setOffset(Factor, Units);
		}

		return Renderer.create(Creator);
	}

	kueken::rasterizer::name loadLine
	(
		kueken::renderer & Renderer, 
		TiXmlElement* Rasterizer
	)
	{
		kueken::rasterizer::creator<kueken::rasterizer::LINE> Creator;

		assert(0);

		return kueken::rasterizer::name::Null();
	}

	kueken::rasterizer::name loadPoint
	(
		kueken::renderer & Renderer, 
		TiXmlElement* Rasterizer
	)
	{
		kueken::rasterizer::creator<kueken::rasterizer::LINE> Creator;
		assert(0);

		return kueken::rasterizer::name::Null();
	}

	kueken::rasterizer::name loadDiscard
	(
		kueken::renderer & Renderer, 
		TiXmlElement* Rasterizer
	)
	{
		kueken::rasterizer::creator<kueken::rasterizer::LINE> Creator;

		assert(0);

		return kueken::rasterizer::name::Null();
	}

}//namespace detail

	kueken::rasterizer::name rasterizer
	(
		kueken::renderer & Renderer,
		void * Pointer
	)
	{
		TiXmlElement* Rasterizer = reinterpret_cast<TiXmlElement*>(Pointer);

		if(Rasterizer)
		{
			TiXmlAttribute* Attribute = Rasterizer->FirstAttribute();
			do
			{
				if(!strcmp("type", Attribute->Name()))
				{
					if(!strcmp("polygon", Attribute->Value()))
						return detail::loadPolygon(Renderer, Rasterizer);
					else if(!strcmp("line", Attribute->Value()))
						return detail::loadLine(Renderer, Rasterizer);
					else if(!strcmp("point", Attribute->Value()))
						return detail::loadPoint(Renderer, Rasterizer);
					else if(!strcmp("discard", Attribute->Value()))
						return detail::loadDiscard(Renderer, Rasterizer);
					else
						assert(0);
				}
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());
		}

		return kueken::rasterizer::name::Null();
	}

	kueken::rasterizer::name rasterizer
	(
		kueken::renderer & Renderer,
		std::string const & Filename
	)
	{
		TiXmlDocument Document(Filename.c_str());
		Document.LoadFile();
		TiXmlElement* Root = Document.RootElement();
		if(std::string(Root->Value()) != std::string("rasterizer"))
			return kueken::rasterizer::name::Null();

		return rasterizer(Renderer, (void*)Root);
	}
/*
	kueken::rasterizer::name load2
	(
		kueken::renderer & Renderer, 
		void * Pointer
	)
	{
		kueken::rasterizer::creator<kueken::rasterizer::POLYGON> Creator;
		DOMElement* Rasterizer = reinterpret_cast<DOMElement*>(Pointer);

	    DOMNode* Child = Rasterizer->getFirstChild();
		if(Child)
		{
			do
			{
				if(Child->getNodeType() != DOMNode::ELEMENT_NODE)
					continue;

				DOMElement* Element = reinterpret_cast<DOMElement*>(Child);
				if(XMLString::equals(Element->getTagName(), XMLString::transcode("viewport")))
				{
					glm::ivec4 Rect(0);
					{
						char * Data = XMLString::transcode(Element->getAttribute(XMLString::transcode("x")));
						Rect.x = atoi(Data);
						XMLString::release(&Data);
					}
					{
						char * Data = XMLString::transcode(Element->getAttribute(XMLString::transcode("y")));
						Rect.y = atoi(Data);
						XMLString::release(&Data);
					}
					{
						char * Data = XMLString::transcode(Element->getAttribute(XMLString::transcode("w")));
						Rect.z = atoi(Data);
						XMLString::release(&Data);
					}
					{
						char * Data = XMLString::transcode(Element->getAttribute(XMLString::transcode("h")));
						Rect.w = atoi(Data);
						XMLString::release(&Data);
					}
					Creator.setViewport(Rect);
				}
				else if(XMLString::equals(Element->getTagName(), XMLString::transcode("multisample")))
				{
					bool Enable = false;

					{
						char * Data = XMLString::transcode(Element->getAttribute(XMLString::transcode("enable")));
						Enable = std::string(Data) == std::string("true");
						XMLString::release(&Data);
					}

					Creator.setMultisample(Enable);
				}
			}
			while(Child = Child->getNextSibling());
		}

	 //   TiXmlElement* Multisample = Rasterizer->FirstChildElement("multisample");
		//if(Multisample)
		//{
		//	TiXmlAttribute* Attribute = Multisample->FirstAttribute();
		//	do
		//	{
		//		if(!strcmp("enable", Attribute->Name()))
		//			Creator.setMultisample(std::string(Attribute->Value()) == std::string("true") ? true : false);
		//		else
		//			assert(0);
		//	}    
		//	while (Attribute = Attribute->Next());
		//}

	 //   TiXmlElement* Scissor = Rasterizer->FirstChildElement("scissor");
		//if(Scissor)
		//{
		//	bool Enable = false;
		//	glm::ivec4 Rect(0);
		//	TiXmlAttribute* Attribute = Scissor->FirstAttribute();
		//	do
		//	{
		//		if(!strcmp("enable", Attribute->Name()))
		//			Enable = std::string(Attribute->Value()) == std::string("true") ? true : false;
		//		else if(!strcmp("x", Attribute->Name()))
		//			Rect.x = atoi(Attribute->Value());
		//		else if(!strcmp("y", Attribute->Name()))
		//			Rect.y = atoi(Attribute->Value());
		//		else if(!strcmp("w", Attribute->Name()))
		//			Rect.z = atoi(Attribute->Value());
		//		else if(!strcmp("h", Attribute->Name()))
		//			Rect.w = atoi(Attribute->Value());
		//		else
		//			assert(0);
		//	}    
		//	while (Attribute = Attribute->Next());

		//	Creator.setScissor(Enable, Rect);
		//}

	 //   TiXmlElement* Frontface = Rasterizer->FirstChildElement("frontface");
		//if(Frontface)
		//{
		//	kueken::rasterizer::front Face = kueken::rasterizer::CCW;

		//	TiXmlAttribute* Attribute = Frontface->FirstAttribute();
		//	do
		//	{
		//		if(!strcmp("face", Attribute->Name()))
		//			Face = std::string(Attribute->Value()) == std::string("ccw") ? kueken::rasterizer::CCW : kueken::rasterizer::CW;
		//		else
		//			assert(0);
		//	}    
		//	while (Attribute = Attribute->Next());

		//	Creator.setFrontFace(Face);
		//}

	 //   TiXmlElement* Cullface = Rasterizer->FirstChildElement("cullface");
		//if(Cullface)
		//{
		//	kueken::rasterizer::cull Face = kueken::rasterizer::CULL_NONE;

		//	TiXmlAttribute* Attribute = Cullface->FirstAttribute();
		//	do
		//	{
		//		if(!strcmp("face", Attribute->Name()))
		//		{
		//			if(std::string(Attribute->Value()) == std::string("none"))
		//				Face = kueken::rasterizer::CULL_NONE;
		//			else if(std::string(Attribute->Value()) == std::string("front"))
		//				Face = kueken::rasterizer::FRONT;
		//			else if(std::string(Attribute->Value()) == std::string("back"))
		//				Face = kueken::rasterizer::BACK;
		//			else if(std::string(Attribute->Value()) == std::string("both"))
		//				Face = kueken::rasterizer::FRONT_AND_BACK;
		//		}
		//		else
		//			assert(0);
		//	}    
		//	while (Attribute = Attribute->Next());

		//	Creator.setCullface(Face);
		//}

	 //   TiXmlElement* Offset = Rasterizer->FirstChildElement("offset");
		//if(Offset)
		//{
		//	float Factor = 0.0f;
		//	float Units = 0.0f;

		//	TiXmlAttribute* Attribute = Offset->FirstAttribute();
		//	do
		//	{
		//		if(!strcmp("factor", Attribute->Name()))
		//			Factor = float(atof(Attribute->Value()));
		//		else if(!strcmp("units", Attribute->Name()))
		//			Units = float(atof(Attribute->Value()));
		//		else
		//			assert(0);
		//	}    
		//	while (Attribute = Attribute->Next());

		//	Creator.setOffset(Factor, Units);
		//}

		return Renderer.create(Creator);
	}

	kueken::rasterizer::name load2
	(
		kueken::renderer & Renderer, 
		std::string const & Filename
	)
	{
		XercesDOMParser* Parser = new XercesDOMParser;
		Parser->parse(Filename.c_str());

		DOMDocument* Document = Parser->getDocument();
		DOMElement* Root = Document->getDocumentElement();

		if(!XMLString::equals(Root->getTagName(), XMLString::transcode("rasterizer")))
			return kueken::rasterizer::name::Null();

		return load2(Renderer, (void*)Root);
	}
*/

}//namespace load
