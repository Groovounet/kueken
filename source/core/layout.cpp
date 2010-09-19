#include <core/layout.hpp>
#include <core/renderer.hpp>

#define KUEKEN_BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace
{
	std::size_t layout_componant_cast
	(
		kueken::layout::array Array
	)
	{
		static std::size_t const CastAssemblerComponant[kueken::assembler::ARRAY_MAX] =
		{
			1, //F16VEC1,
			2, //F16VEC2,
			3, //F16VEC3,
			4, //F16VEC4,
			1, //F32VEC1,
			2, //F32VEC2,
			3, //F32VEC3,
			4, //F32VEC4,
			1, //U8VEC1,
			2, //U8VEC2,
			3, //U8VEC3,
			4  //U8VEC4
		};

		return CastAssemblerComponant[Array];
	}

	GLenum layout_type_cast
	(
		kueken::layout::array Array
	)
	{
		static GLenum const Cast[kueken::assembler::ARRAY_MAX] =
		{
			GL_HALF_FLOAT,		//F16VEC1,
			GL_HALF_FLOAT,		//F16VEC2,
			GL_HALF_FLOAT,		//F16VEC3,
			GL_HALF_FLOAT,		//F16VEC4,
			GL_FLOAT,			//F32VEC1,
			GL_FLOAT,			//F32VEC2,
			GL_FLOAT,			//F32VEC3,
			GL_FLOAT,			//F32VEC4,
			GL_UNSIGNED_BYTE,	//U8VEC1,
			GL_UNSIGNED_BYTE,	//U8VEC2,
			GL_UNSIGNED_BYTE,	//U8VEC3,
			GL_UNSIGNED_BYTE	//U8VEC4
		};

		return Cast[Array];
	}

}//namespace

namespace kueken{
namespace layout
{
	void creator::setRenderer(renderer * Renderer)
	{
		this->Renderer = Renderer;
	}

	void creator::setVertexArray
	(
		slot const & Slot,
		semantic const & Semantic,
		array const & Format, 
		std::size_t const & Stride,
		std::size_t const & Offset,
		std::size_t const & Divisor
	)
	{
		detail::vertexArray VertexArray;
		VertexArray.Slot = Slot;
		VertexArray.Semantic = Semantic;
		VertexArray.Size = layout_componant_cast(Format);
		VertexArray.Type = layout_type_cast(Format);
		VertexArray.Stride = Stride;
		VertexArray.Offset = KUEKEN_BUFFER_OFFSET(Offset);
		VertexArray.Divisor = GLuint(Divisor);
		VertexArrays.insert(std::make_pair(Semantic, VertexArray));
	}

	object::object
	(
		creator const & Creator
	) :
		Renderer(Creator.Renderer)
	{
		this->VertexArrays.resize(Creator.VertexArrays.size());
		std::size_t i = 0;
		for
		(
			std::map<semantic, detail::vertexArray>::const_iterator it = Creator.VertexArrays.begin();
			it != Creator.VertexArrays.end();
			++it, ++i
		)
		{
			this->VertexArrays[i] = it->second;
		}
	}

	object::~object()
	{}

	void object::bind()
	{
		for(std::vector<detail::vertexArray>::size_type i = 0; i < this->VertexArrays.size(); ++i)
		{
			//buffer::name Buffer = this->Renderer->getBinding(
			//	this->VertexArrays[i].Slot);

			//this->Renderer->bind(
			//	0,
			//	kueken::buffer::ARRAY,
			//	Buffer);

			glVertexAttribPointer(
				this->VertexArrays[i].Semantic, 
				this->VertexArrays[i].Size,
				this->VertexArrays[i].Type,
				GL_FALSE, 
				this->VertexArrays[i].Stride,
				this->VertexArrays[i].Offset);
			glVertexAttribDivisor(
				this->VertexArrays[i].Semantic, 
				this->VertexArrays[i].Divisor);
			glEnableVertexAttribArray(
				this->VertexArrays[i].Semantic);
		}
	}

}//namespace layout
}//namespace kueken
