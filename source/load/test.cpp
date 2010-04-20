#include <load/test.hpp>

namespace load{
namespace detail
{
	static kueken::test::function func_cast(char const * String)
	{
		if(!strcmp("never", String))
			return kueken::test::NEVER;
		else if(!strcmp("less", String))
			return kueken::test::LESS;
		else if(!strcmp("equal", String))
			return kueken::test::EQUAL;
		else if(!strcmp("lequal", String))
			return kueken::test::LEQUAL;
		else if(!strcmp("greater", String))
			return kueken::test::GREATER;
		else if(!strcmp("notequal", String))
			return kueken::test::NOTEQUAL;
		else if(!strcmp("gequal", String))
			return kueken::test::GEQUAL;
		else if(!strcmp("always", String))
			return kueken::test::ALWAYS;
		else 
		{
			return kueken::test::ALWAYS;
			assert(0);
		}
	}

	static kueken::test::action action_cast(char const * String)
	{
		if(!strcmp("keep", String))
			return kueken::test::KEEP;
		else if(!strcmp("zero", String))
			return kueken::test::ZERO;
		else if(!strcmp("replace", String))
			return kueken::test::REPLACE;
		else if(!strcmp("incr", String))
			return kueken::test::INCR;
		else if(!strcmp("incr_wrap", String))
			return kueken::test::INCR_WRAP;
		else if(!strcmp("decr", String))
			return kueken::test::DECR;
		else if(!strcmp("decr_wrap", String))
			return kueken::test::DECR_WRAP;
		else 
		{
			return kueken::test::KEEP;
			assert(0);
		}
	}

	static kueken::test::face face_cast(char const * String)
	{
		if(!strcmp("front", String))
			return kueken::test::FRONT;
		else if(!strcmp("back", String))
			return kueken::test::BACK;
		else 
		{
			return kueken::test::FRONT;	
			assert(0);
		}
	}

}//namespace detail

	kueken::test::name test
	(
		kueken::renderer & Renderer,
		void * Pointer
	)
	{
		kueken::test::creator Creator;

		TiXmlElement* Test = reinterpret_cast<TiXmlElement*>(Pointer);

	    TiXmlElement* Depth = Test->FirstChildElement("depth");
		if(Depth)
		{
			bool Enable = false;

			TiXmlAttribute* Attribute = Depth->FirstAttribute();
			do
			{
				if(!strcmp("enable", Attribute->Name()))
					Enable = std::string(Attribute->Value()) == std::string("true") ? true : false;
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());

			Creator.setDepthEnable(Enable);
		}

	    TiXmlElement* DepthFunction = Test->FirstChildElement("depth-function");
		if(DepthFunction)
		{
			kueken::test::function Func = kueken::test::LESS;

			TiXmlAttribute* Attribute = DepthFunction->FirstAttribute();
			do
			{
				if(!strcmp("value", Attribute->Name()))
					Func = detail::func_cast(Attribute->Value());
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());

			Creator.setDepthFunc(Func);
		}

	    TiXmlElement* DepthRange = Test->FirstChildElement("depth-range");
		if(DepthRange)
		{
			float Near = 0.0f;
			float Far = 1.0f;

			TiXmlAttribute* Attribute = DepthRange->FirstAttribute();
			do
			{
				if(!strcmp("near", Attribute->Name()))
					Near = float(atof(Attribute->Value()));
				else if(!strcmp("far", Attribute->Name()))
					Far = float(atof(Attribute->Value()));
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());

			Creator.setDepthRange(Near, Far);
		}

	    TiXmlElement* Stencil = Test->FirstChildElement("stencil");
		if(Stencil)
		{
			bool Enable = false;

			TiXmlAttribute* Attribute = Stencil->FirstAttribute();
			do
			{
				if(!strcmp("enable", Attribute->Name()))
					Enable = std::string(Attribute->Value()) == std::string("true") ? true : false;
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());

			Creator.setStencilEnable(Enable);
		}

		TiXmlElement* StencilFunction = Test->FirstChildElement("stencil-function");
		if(StencilFunction)
		//do
		{
			kueken::test::face Face = kueken::test::FRONT;
			kueken::test::function Func[kueken::test::FACE_MAX] = {kueken::test::ALWAYS, kueken::test::ALWAYS};
			int Ref[kueken::test::FACE_MAX] = {0, 0};
			glm::uint Mask[kueken::test::FACE_MAX] = {0xFF, 0xFF};

			TiXmlAttribute* Attribute = StencilFunction->FirstAttribute();
			do
			{
				if(!strcmp("face", Attribute->Name()))
					Face = detail::face_cast(Attribute->Value());
				else if(!strcmp("function", Attribute->Name()))
					Func[Face] = detail::func_cast(Attribute->Value());
				else if(!strcmp("ref", Attribute->Name()))
					Ref[Face] = atoi(Attribute->Value());
				else if(!strcmp("mask", Attribute->Name()))
					Mask[Face] = atoi(Attribute->Value());
				else
					assert(0);
			}    
			while(Attribute = Attribute->Next());

			Creator.setStencilFunc(kueken::test::FRONT, Func[kueken::test::FRONT], Ref[kueken::test::FRONT], Mask[kueken::test::FRONT]);
			Creator.setStencilFunc(kueken::test::BACK, Func[kueken::test::BACK], Ref[kueken::test::BACK], Mask[kueken::test::BACK]);
		}
		//while(StencilFunction = StencilFunction->NextSiblingElement());

		TiXmlElement* StencilOperation = Test->FirstChildElement("stencil-operation");
		if(StencilOperation)
		//do
		{
			kueken::test::face Face = kueken::test::FRONT;
			kueken::test::action sfail[kueken::test::FACE_MAX] = {kueken::test::KEEP, kueken::test::KEEP};
			kueken::test::action dpfail[kueken::test::FACE_MAX] = {kueken::test::KEEP, kueken::test::KEEP};
			kueken::test::action dppass[kueken::test::FACE_MAX] = {kueken::test::KEEP, kueken::test::KEEP};

			TiXmlAttribute* Attribute = StencilOperation->FirstAttribute();
			do
			{
				if(!strcmp("face", Attribute->Name()))
					Face = detail::face_cast(Attribute->Value());
				else if(!strcmp("sfail", Attribute->Name()))
					sfail[Face] = detail::action_cast(Attribute->Value());
				else if(!strcmp("dpfail", Attribute->Name()))
					dpfail[Face] = detail::action_cast(Attribute->Value());
				else if(!strcmp("dppass", Attribute->Name()))
					dppass[Face] = detail::action_cast(Attribute->Value());
				else
					assert(0);
			}    
			while(Attribute = Attribute->Next());

			Creator.setStencilOp(kueken::test::FRONT, sfail[kueken::test::FRONT], dpfail[kueken::test::FRONT], dppass[kueken::test::FRONT]);
			Creator.setStencilOp(kueken::test::BACK, sfail[kueken::test::BACK], dpfail[kueken::test::BACK], dppass[kueken::test::BACK]);
		}
		//while(StencilOperation = StencilOperation->NextSiblingElement());

		return Renderer.create(Creator);
	}

	kueken::test::name test
	(
		kueken::renderer & Renderer,
		std::string const & Filename
	)
	{
		TiXmlDocument Document(Filename.c_str());
		Document.LoadFile();
		TiXmlElement* Root = Document.RootElement();
		if(std::string(Root->Value()) != std::string("test"))
			return kueken::test::name::Null();

		return test(Renderer, (void*)Root);
	}

}//namespace load
