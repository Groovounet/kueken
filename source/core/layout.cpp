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
		static std::size_t const Cast[kueken::layout::ARRAY_MAX] =
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

		static_assert(
			sizeof(Cast) / sizeof(std::size_t) == kueken::layout::ARRAY_MAX,
			"Cast array size mismatch");

		return Cast[Array];
	}

	GLenum layout_type_cast
	(
		kueken::layout::array Array
	)
	{
		static GLenum const Cast[kueken::layout::ARRAY_MAX] =
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

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::layout::ARRAY_MAX,
			"Cast array size mismatch");

		return Cast[Array];
	}

}//namespace

namespace kueken{
namespace layout
{
	creator::creator(renderer & Renderer) :
		Renderer(Renderer)	
	{}

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
		
		if(this->SlotData.size() <= Slot)
			this->SlotData.resize(Slot + slot(1));
		this->SlotData[Slot].push_back(VertexArray);
	}

	object::object
	(
		creator const & Creator
	) :
		Renderer(Creator.Renderer)
	{
		this->SlotData = Creator.SlotData;
	}

	object::~object()
	{}

	void object::bind()
	{
		for(detail::slotData::size_type SlotIndex = 0; SlotIndex < this->SlotData.size(); ++SlotIndex)
		{
			detail::semanticData const & SemanticsData = this->SlotData[SlotIndex];
			if(SemanticsData.empty())
				continue;

			buffer::name const & Buffer = this->Renderer.getBinding(buffer::slot(SlotIndex));
			this->Renderer.bind(
				0,
				kueken::buffer::ARRAY,
				Buffer);

			for
			(
				detail::semanticData::size_type SemanticIndex = 0; 
				SemanticIndex < SemanticsData.size(); 
				++SemanticIndex
			)
			{
				detail::vertexArray const & VertexArray = SemanticsData[SemanticIndex];

				glVertexAttribPointer(
					VertexArray.Semantic, 
					VertexArray.Size,
					VertexArray.Type,
					GL_FALSE, 
					VertexArray.Stride,
					VertexArray.Offset);
				
				glVertexAttribDivisor(
					VertexArray.Semantic, 
					VertexArray.Divisor);

				glEnableVertexAttribArray(
					VertexArray.Semantic);
			}
		}
	}

}//namespace layout
}//namespace kueken
