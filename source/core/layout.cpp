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
		static std::size_t const Cast[] =
		{
			1, //F16VEC1,
			2, //F16VEC2,
			3, //F16VEC3,
			4, //F16VEC4,
			1, //F32VEC1,
			2, //F32VEC2,
			3, //F32VEC3,
			4, //F32VEC4,
			1, //I8VEC1,
			2, //I8VEC2,
			3, //I8VEC3,
			4, //I8VEC4
			1, //I16VEC1,
			2, //I16VEC2,
			3, //I16VEC3,
			4, //I16VEC4
			1, //I32VEC1,
			2, //I32VEC2,
			3, //I32VEC3,
			4, //I32VEC4
			1, //U8VEC1,
			2, //U8VEC2,
			3, //U8VEC3,
			4, //U8VEC4
			1, //U16VEC1,
			2, //U16VEC2,
			3, //U16VEC3,
			4, //U16VEC4
			1, //U32VEC1,
			2, //U32VEC2,
			3, //U32VEC3,
			4  //U32VEC4
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
		static GLenum const Cast[] =
		{
			GL_HALF_FLOAT,		//F16VEC1,
			GL_HALF_FLOAT,		//F16VEC2,
			GL_HALF_FLOAT,		//F16VEC3,
			GL_HALF_FLOAT,		//F16VEC4,
			GL_FLOAT,			//F32VEC1,
			GL_FLOAT,			//F32VEC2,
			GL_FLOAT,			//F32VEC3,
			GL_FLOAT,			//F32VEC4,
			GL_BYTE,			//I8VEC1,
			GL_BYTE,			//I8VEC2,
			GL_BYTE,			//I8VEC3,
			GL_BYTE,			//I8VEC4
			GL_SHORT,			//I16VEC1,
			GL_SHORT,			//I16VEC2,
			GL_SHORT,			//I16VEC3,
			GL_SHORT,			//I16VEC4
			GL_INT,				//I32VEC1,
			GL_INT,				//I32VEC2,
			GL_INT,				//I32VEC3,
			GL_INT,				//I32VEC4
			GL_UNSIGNED_BYTE,	//U8VEC1,
			GL_UNSIGNED_BYTE,	//U8VEC2,
			GL_UNSIGNED_BYTE,	//U8VEC3,
			GL_UNSIGNED_BYTE,	//U8VEC4
			GL_UNSIGNED_SHORT,	//U16VEC1,
			GL_UNSIGNED_SHORT,	//U16VEC2,
			GL_UNSIGNED_SHORT,	//U16VEC3,
			GL_UNSIGNED_SHORT,	//U16VEC4
			GL_UNSIGNED_INT,	//U32VEC1,
			GL_UNSIGNED_INT,	//U32VEC2,
			GL_UNSIGNED_INT,	//U32VEC3,
			GL_UNSIGNED_INT		//U32VEC4
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
				Buffer,
				kueken::buffer::ARRAY);

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
