#include <fx/texture.hpp>
#include <fx/effect.hpp>

namespace fx{
namespace effect{
namespace detail
{
	kueken::blend::function func_cast(char const * String)
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

	kueken::blend::equation equa_cast(char const * String)
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

	kueken::texture::name object::loadTexture
	(
		void * Pointer
	)
	{
		kueken::texture::creator<kueken::texture::image> Creator;
		glm::uint Program = 0;
		std::string Variable;
		glm::uint Sampler = 0;
		glm::uint Image = 0;
		glm::uint Buffer = 0;
		glm::uint Id = 0;

		TiXmlElement* Texture = reinterpret_cast<TiXmlElement*>(Pointer);

		TiXmlAttribute* Attribute = Texture->FirstAttribute();
		do
		{
			if(!strcmp("id", Attribute->Name()))
				Id = atoi(Attribute->Value());
			else if(!strcmp("program", Attribute->Name()))
				Program = atoi(Attribute->Value());
			else if(!strcmp("variable", Attribute->Name()))
				Variable = std::string(Attribute->Value());
			else if(!strcmp("sampler", Attribute->Name()))
				Sampler = atoi(Attribute->Value());
			else if(!strcmp("image", Attribute->Name()))
				Image = atoi(Attribute->Value());
			else if(!strcmp("buffer", Attribute->Name()))
				Buffer = atoi(Attribute->Value());
			else
				assert(0);
		}
		while (Attribute = Attribute->Next());

		Creator.setImage(findImage(Image));
		Creator.setSampler(findSampler(Sampler));
		kueken::program::name ProgramName = findProgram(Program);
		kueken::program::object* ProgramObject = Renderer->map(ProgramName);
		kueken::program::variable VariableObject = ProgramObject->get(Variable, kueken::program::SAMPLER);
		Renderer->unmap(ProgramName);
		Creator.setVariable(VariableObject);

		kueken::texture::name Name = Renderer->create(Creator);
		Textures.insert(std::make_pair(Id, Name));
		return Name;
	}

	kueken::texture::name object::loadTexture
	(
		std::string const & Filename
	)
	{
		TiXmlDocument Document(Filename.c_str());
		Document.LoadFile();
		TiXmlElement* Root = Document.RootElement();
		if(std::string(Root->Value()) != std::string("texture"))
			return kueken::texture::name::Null();

		return loadTexture((void*)Root);
	}
}//namespace effect

namespace texture
{
	command::command() :
		Renderer(0),
		Texture(kueken::texture::name::Null())
	{}

	command::command
	(
		kueken::renderer* Renderer,
		kueken::texture::name Texture,
		kueken::texture::slot Slot
	) :
		Renderer(Renderer),
		Texture(Texture),
		Slot(Slot)
	{}

	void command::exec()
	{
		Renderer->bind(Texture, Slot);
	}

}//namespace texture
}//namespace fx
