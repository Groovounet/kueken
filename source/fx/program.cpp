#include <fx/program.hpp>
#include <fx/effect.hpp>
#include <fstream>

namespace fx{
namespace effect{
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

	kueken::program::name object::loadProgram
	(
		void * Pointer
	)
	{
		kueken::program::creator Creator;

		TiXmlElement* Program = reinterpret_cast<TiXmlElement*>(Pointer);
		glm::uint Id = 0;
		{
			TiXmlAttribute* Attribute = Program->FirstAttribute();
			do
			{
				if(!strcmp("id", Attribute->Name()))
					Id = atoi(Attribute->Value());
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());
		}

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

		kueken::program::name Name = Renderer->create(Creator);
		Programs.insert(std::make_pair(Id, Name));
		return Name;
	}

	kueken::program::name object::loadProgram
	(
		std::string const & Filename
	)
	{
		TiXmlDocument Document(Filename.c_str());
		Document.LoadFile();
		TiXmlElement* Root = Document.RootElement();
		if(std::string(Root->Value()) != std::string("program"))
			return kueken::program::name::Null();

		return loadProgram((void*)Root);
	}
}//namespace effect

namespace program
{
	command::command() :
		Renderer(0),
		Program(kueken::program::name::Null())
	{}

	command::command
	(
		kueken::renderer* Renderer,
		kueken::program::name Program
	) :
		Renderer(Renderer),
		Program(Program)
	{}
	
	void command::exec()
	{
		Renderer->bind(Program);
	}	

}//namespace program
}//namespace fx

