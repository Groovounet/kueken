#include <fx/image.hpp>
#include <fx/effect.hpp>
#include <gli/gli.hpp>

namespace fx{
namespace effect{
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

	kueken::image::name object::loadImage
	(
		void * Pointer
	)
	{
		kueken::image::creator Creator;
		gli::image ImageFile;
		glm::uint Id = 0;

		TiXmlElement* Image = reinterpret_cast<TiXmlElement*>(Pointer);
		{
			TiXmlAttribute* Attribute = Image->FirstAttribute();
			do
			{
				if(!strcmp("id", Attribute->Name()))
					Id = atoi(Attribute->Value());
				else
					assert(0);
			}    
			while (Attribute = Attribute->Next());
		}

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
					glm::uvec3 Size(
						ImageFile[Level].dimensions().x, 
						ImageFile[Level].dimensions().y, 
						ImageFile[Level].dimensions().z);

					Creator.setMipmap(
						Level, 
						Size, 
						ImageFile[Level].data());
				}
			}
			else
			{
				assert(0);
			}
		}
		while (Child = Child->NextSiblingElement());

		kueken::image::name Name = Renderer->create(Creator);
		Images.insert(std::make_pair(Id, Name));
		return Name;
	}

	kueken::image::name object::loadImage
	(
		std::string const & Filename
	)
	{
		TiXmlDocument Document(Filename.c_str());
		Document.LoadFile();
		TiXmlElement* Root = Document.RootElement();
		if(std::string(Root->Value()) != std::string("image"))
			return kueken::image::name::Null();

		return loadImage((void*)Root);
	}

	//else if(std::string(Child->Value()) == std::string("swizzle"))
	//{
	//	kueken::sampler::swizzle SwizzleR = kueken::sampler::RED;
	//	kueken::sampler::swizzle SwizzleG = kueken::sampler::GREEN;
	//	kueken::sampler::swizzle SwizzleB = kueken::sampler::BLUE;
	//	kueken::sampler::swizzle SwizzleA = kueken::sampler::ALPHA;

	//	TiXmlAttribute* Attribute = Child->FirstAttribute();
	//	do
	//	{
	//		if(!strcmp("r", Attribute->Name()))
	//			SwizzleR = detail::swizzle_cast(Attribute->Value());
	//		else if(!strcmp("g", Attribute->Name()))
	//			SwizzleG = detail::swizzle_cast(Attribute->Value());
	//		else if(!strcmp("b", Attribute->Name()))
	//			SwizzleB = detail::swizzle_cast(Attribute->Value());
	//		else if(!strcmp("a", Attribute->Name()))
	//			SwizzleA = detail::swizzle_cast(Attribute->Value());
	//		else
	//			assert(0);
	//	}    
	//	while (Attribute = Attribute->Next());

	//	Creator.setSwizzle(SwizzleR, SwizzleG, SwizzleB, SwizzleA);
	//}

}//namespace effect
}//namespace fx
