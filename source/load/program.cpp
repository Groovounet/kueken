#include <load/program.hpp>
#include <fstream>

namespace load{
namespace detail
{
	kueken::program::shader shader_cast(char const * String)
	{
		if(!strcmp("vertex", String))
			return kueken::program::VERTEX;
		else if(!strcmp("geometry", String))
			return kueken::program::GEOMETRY;
		else if(!strcmp("fragment", String))
			return kueken::program::FRAGMENT;
		else 
		{
			return kueken::program::VERTEX;
			assert(0);
		}
	}

	std::string loadFile(const char* Filename)
	{
		std::ifstream stream(Filename, std::ios::in);

		if(!stream.is_open())
			return std::string();

		std::string Line = "";
		std::string Text = "";

		while(getline(stream, Line))
			Text += "\n" + Line;

		stream.close();

		return Text;
	}
}//namespace detail

	kueken::program::name program
	(
		kueken::renderer & Renderer,
		void * Pointer
	)
	{
		kueken::program::creator Creator;

		TiXmlElement* Program = reinterpret_cast<TiXmlElement*>(Pointer);

	    TiXmlElement* Child = Program->FirstChildElement();
		do
		{
			if(std::string(Child->Value()) == std::string("shader"))
			{
				kueken::program::shader Target;
				std::string Path;

				TiXmlAttribute* Attribute = Child->FirstAttribute();
				do
				{
					if(!strcmp("target", Attribute->Name()))
						Target = detail::shader_cast(Attribute->Value());
					else if(!strcmp("path", Attribute->Name()))
						Path = std::string(Attribute->Value());
					else
						assert(0);
				}    
				while (Attribute = Attribute->Next());

				Creator.attachShader(Target, detail::loadFile(Path.c_str()));
			}
			else
			{
				assert(0);
			}
		}
		while (Child = Child->NextSiblingElement());

		return Renderer.create(Creator);
	}

	kueken::program::name program
	(
		kueken::renderer & Renderer,
		std::string const & Filename
	)
	{
		TiXmlDocument Document(Filename.c_str());
		Document.LoadFile();
		TiXmlElement* Root = Document.RootElement();
		if(std::string(Root->Value()) != std::string("program"))
			return kueken::program::name::Null();

		return program(Renderer, (void*)Root);
	}

}//namespace load

