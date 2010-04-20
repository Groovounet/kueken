#include <load/image.hpp>
#include <gli/gli.hpp>

namespace load{
namespace detail
{
	kueken::image::target target_cast(char const * String)
	{
		if(!strcmp("image1d", String))
			return kueken::image::IMAGE1D;
		else if(!strcmp("image2d", String))
			return kueken::image::IMAGE2D;
		else if(!strcmp("image3d", String))
			return kueken::image::IMAGE3D;
		else if(!strcmp("array1d", String))
			return kueken::image::IMAGE3D;
		else if(!strcmp("array2d", String))
			return kueken::image::IMAGE3D;
		else if(!strcmp("rect", String))
			return kueken::image::IMAGE3D;
		else if(!strcmp("cube-pos-x", String))
			return kueken::image::IMAGE3D;
		else if(!strcmp("cube-neg-x", String))
			return kueken::image::IMAGE3D;
		else if(!strcmp("cube-pos-y", String))
			return kueken::image::IMAGE3D;
		else if(!strcmp("cube-neg-y", String))
			return kueken::image::IMAGE3D;
		else if(!strcmp("cube-pos-z", String))
			return kueken::image::IMAGE3D;
		else if(!strcmp("cube-neg-z", String))
			return kueken::image::IMAGE3D;
		else 
		{
			return kueken::image::IMAGE2D;
			assert(0);
		}
	}
}//namespace detail

	kueken::image::name image
	(
		kueken::renderer & Renderer,
		void * Pointer
	)
	{
		kueken::image::creator Creator;
		gli::image ImageFile;
		glm::uint Id = 0;

		TiXmlElement* Image = reinterpret_cast<TiXmlElement*>(Pointer);

	    TiXmlElement* Child = Image->FirstChildElement();
		do
		{
			if(std::string(Child->Value()) == std::string("file"))
			{
				std::string Path;
				kueken::image::target Target = kueken::image::IMAGE2D;

				TiXmlAttribute* Attribute = Child->FirstAttribute();
				do
				{
					if(!strcmp("target", Attribute->Name()))
						Target = detail::target_cast(Attribute->Value());
					else if(!strcmp("path", Attribute->Name()))
						Path = std::string(Attribute->Value());
					else
						assert(0);
				}    
				while (Attribute = Attribute->Next());

				ImageFile = gli::import_as(Path);

				//Creator.setFormat(kueken::image::format(ImageFile.format()));
				Creator.setFormat(kueken::image::RGB8);//format(ImageFile.format()));
				Creator.setTarget(Target);
				for(std::size_t Level = 0; Level < ImageFile.levels(); ++Level)
				{
					Creator.setMipmap(
						Level, 
						ImageFile[Level].dimensions(), 
						ImageFile[Level].data());
				}
			}
			else if(std::string(Child->Value()) == std::string("generate-mipmaps"))
			{
				bool GenerateMipmaps = false;

				TiXmlAttribute* Attribute = Child->FirstAttribute();
				do
				{
					if(!strcmp("value", Attribute->Name()))
						GenerateMipmaps = std::string(Attribute->Value()) == std::string("true") ? true : false;
					else
						assert(0);
				}    
				while (Attribute = Attribute->Next());

				Creator.setGenerateMipmaps(GenerateMipmaps);
			}
			else
			{
				assert(0);
			}
		}
		while (Child = Child->NextSiblingElement());

		return Renderer.create(Creator);
	}

	kueken::image::name image
	(
		kueken::renderer & Renderer,
		std::string const & Filename
	)
	{
		TiXmlDocument Document(Filename.c_str());
		Document.LoadFile();
		TiXmlElement* Root = Document.RootElement();
		if(std::string(Root->Value()) != std::string("image"))
			return kueken::image::name::Null();

		return image(Renderer, (void*)Root);
	}

}//namespace load
