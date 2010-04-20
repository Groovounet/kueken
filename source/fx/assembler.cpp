#include <core/assembler.hpp>
#include <core/renderer.hpp>

#define KUEKEN_BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace
{
	GLenum assembler_primitive_cast(kueken::assembler::primitive Primitive)
	{
		static GLenum const CastAssemblerPrimitive[kueken::assembler::PRIMITIVE_MAX] =
		{
			GL_TRIANGLES				// TRIANGLES
		};

		return CastAssemblerPrimitive[Primitive];
	}

	std::size_t assembler_componant_cast(kueken::assembler::array Array)
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

	GLenum assembler_type_cast(kueken::assembler::array Array)
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

	GLenum assembler_element_cast(kueken::assembler::element Element)
	{
		static GLenum const Cast[kueken::assembler::ELEMENT_MAX] =
		{
			GL_NONE,			//NONE
			GL_UNSIGNED_SHORT,	//U16,
			GL_UNSIGNED_INT,	//U32
		};

		return Cast[Element];
	}

}//namespace

namespace kueken{
namespace assembler{

namespace detail
{
	data::data() :
		Primitive(GL_TRIANGLES),
		ElementFormat(GL_NONE),
		Renderer(0)
	{}

}//namespace detail

	void creator::setRenderer(renderer * Renderer)
	{
		Data.Renderer = Renderer;
	}

	void creator::setElements
	(
		element ElementFormat, 
		buffer::name Buffer
	)
	{
		Data.ElementFormat = assembler_element_cast(ElementFormat);
		Data.ElementBuffer = Buffer;
	}

	void creator::setPrimitive(primitive Primitive)
	{
		Data.Primitive = assembler_primitive_cast(Primitive);
	}

	void creator::addAttribute
	(
		buffer::name Buffer,
		program::variable Variable,
		array Array, 
		//bool Normalize,
		std::size_t Stride,
		std::size_t Offset
	)
	{
		detail::attribute Attribute;
		Attribute.Buffer = Buffer;
		Attribute.Location = Variable.Location;
		Attribute.Size = assembler_componant_cast(Array);
		Attribute.Type = assembler_type_cast(Array);
		//Attribute.Normalize = Normalize ? GL_TRUE : GL_FALSE;
		Attribute.Stride = Stride;
		Attribute.Pointer = KUEKEN_BUFFER_OFFSET(Offset);
		Data.Attributes.push_back(Attribute);
	}

	object::object
	(
		creator const & Creator
	) :
		Primitive(Creator.Data.Primitive),
		Renderer(Creator.Data.Renderer),
		ElementBuffer(Creator.Data.ElementBuffer),
		ElementFormat(Creator.Data.ElementFormat)
	{
		glGenVertexArrays(1, &Name);

		glBindVertexArray(Name);
		for(std::size_t i = 0; i < Creator.Data.Attributes.size(); ++i)
		{
			Creator.Data.Renderer->bind(
				Creator.Data.Attributes[i].Buffer,
				kueken::buffer::ARRAY);

			glVertexAttribPointer(
				Creator.Data.Attributes[i].Location, 
				Creator.Data.Attributes[i].Size,
				Creator.Data.Attributes[i].Type,
				GL_FALSE,//Creator.Data.Attributes[i].Normalize, 
				Creator.Data.Attributes[i].Stride,
				Creator.Data.Attributes[i].Pointer);

			glEnableVertexAttribArray(
				Creator.Data.Attributes[i].Location);

			if(ElementFormat != GL_NONE)
				Renderer->bind(ElementBuffer, kueken::buffer::ELEMENT);
		}
		glBindVertexArray(0);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	GLenum object::getPrimitive() const
	{
		return Primitive;
	}

	GLenum object::getElementFormat() const
	{
		return ElementFormat;
	}

	object::~object()
	{
		glDeleteVertexArrays(1, &Name);
	}

	void object::bind()
	{
		glBindVertexArray(Name);
	}

}//namespace assembler
}//namespace kueken
