#include <core/clear.hpp>

namespace
{

}//namespace

namespace kueken{
namespace clear{
namespace detail{

	data::data() :
		BitField(0),
		Red(0.0f),
		Green(0.0f),
		Blue(0.0f),
		Alpha(0.0f),
		Depth(1.0f),
		Stencil(0)
	{}

}//namespace detail

	creator::creator
	(
		renderer & Renderer
	)
	{}

	void creator::setColor(glm::vec4 const & Color)
	{
		Data.BitField |= GL_COLOR_BUFFER_BIT;
		Data.Red = Color.r;
		Data.Green = Color.g;
		Data.Blue = Color.b;
		Data.Alpha = Color.a;
	}

	void creator::setDepth(float Depth)
	{
		Data.BitField |= GL_DEPTH_BUFFER_BIT;
		Data.Depth = Depth;
	}

	void creator::setStencil(int Stencil)
	{
		Data.BitField |= GL_STENCIL_BUFFER_BIT;
		Data.Stencil = Stencil;
	}

	bool creator::validate()
	{
		return true;
	}

	object::object
	(
		creator const & Creator
	) :
		Data(Creator.Data)
	{}

	void object::exec()
	{
		if(Data.BitField & GL_COLOR_BUFFER_BIT)
			glClearColor(Data.Red, Data.Green, Data.Blue, Data.Alpha);
		if(Data.BitField & GL_DEPTH_BUFFER_BIT)
			glClearDepth(Data.Depth);
		if(Data.BitField & GL_STENCIL_BUFFER_BIT)
			glClearStencil(Data.Stencil);

		glClear(Data.BitField);
	}

}//namespace clear
}//namespace kueken
