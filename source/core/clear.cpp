#include <core/clear.hpp>

namespace
{

}//namespace

namespace kueken{
namespace clear{
namespace detail{

	data::data() :
		Depth(1.0f),
		Stencil(0),
		Drawbuffers(0)
	{
		memset(&this->Color[0][0], 0, sizeof(this->Color));
	}

}//namespace detail

	creator::creator
	(
		renderer & Renderer
	)
	{}

	void creator::setColor
	(
		colorbuffer const & Colorbuffer,
		glm::vec4 const & Color
	)
	{
		this->Data.Drawbuffers |= (1 << Colorbuffer);
		this->Data.Color[Colorbuffer] = Color;
	}

	void creator::setDepth(float Depth)
	{
		this->Data.Drawbuffers |= DEPTHBUFFER_BIT;
		this->Data.Depth = Depth;
	}

	void creator::setStencil(int Stencil)
	{
		this->Data.Drawbuffers |= STENCILBUFFER_BIT;
		this->Data.Stencil = Stencil;
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
		for(std::size_t i = 0; i < COLORBUFFER_MAX; ++i)
			if(this->Data.Drawbuffers & (1 << i))
				glClearBufferfv(GL_COLOR, i, &this->Data.Color[i][0]);
		if((this->Data.Drawbuffers & DEPTHBUFFER_BIT) && (this->Data.Drawbuffers & STENCILBUFFER_BIT))
			glClearBufferfi(GL_DEPTH_STENCIL, 0, this->Data.Depth, this->Data.Stencil);
		else if(this->Data.Drawbuffers & DEPTHBUFFER_BIT)
			glClearBufferfv(0, GL_DEPTH, &this->Data.Depth);
		else if(this->Data.Drawbuffers & STENCILBUFFER_BIT)
			glClearBufferiv(0, GL_STENCIL, &this->Data.Stencil);
	}

}//namespace clear
}//namespace kueken
