#ifndef KUEKEN_ASSEMBLER_INCLUDED
#define KUEKEN_ASSEMBLER_INCLUDED

#include "detail/detail.hpp"
#include "buffer.hpp"
#include "program.hpp"

namespace kueken
{
	class renderer;

namespace assembler{

	enum primitive
	{
		TRIANGLES,
		PRIMITIVE_MAX
	};

	enum array
	{
		F16VEC1,
		F16VEC2,
		F16VEC3,
		F16VEC4,
		F32VEC1,
		F32VEC2,
		F32VEC3,
		F32VEC4,
		U8VEC1,
		U8VEC2,
		U8VEC3,
		U8VEC4,
		ARRAY_MAX
	};

	enum element
	{
		NONE,
		U16,
		U32,
		ELEMENT_MAX
	};


namespace detail{

	struct attribute
	{
		buffer::name Buffer;
		GLuint Location;
		GLint Size;
		GLenum Type;
		//GLboolean Normalize;
		GLsizei Stride;
		GLvoid const * Pointer;
	};

	struct data
	{
		data();

		GLenum Primitive;
		buffer::name ElementBuffer;
		GLenum ElementFormat;
		renderer * Renderer;
		std::vector<attribute> Attributes;
	};

}//namespace detail

	class creator// : public kueken::detail::creator
	{
		friend class object;

	public:
		void setRenderer(renderer * Renderer);
		void setElements(element Format, buffer::name Buffer);
		void setPrimitive(primitive Primitive);
		void addAttribute(
			buffer::name Buffer,
			GLuint Location,
			array Format, 
			//bool Normalize,
			std::size_t Stride,
			std::size_t Offset);

		virtual bool validate(){assert(0); return false;}

	private:
		detail::data Data;
	};

	class object
	{
	public:
		object(creator const & Creator);
		~object();

		void bind();

		GLenum getPrimitive() const;
		GLenum getElementFormat() const;

	private:
		GLuint Name;
		GLenum Primitive;
		GLenum ElementFormat;
		buffer::name ElementBuffer;
		renderer * Renderer;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace assembler
}//namespace kueken

#include "assembler.inl"

#endif//KUEKEN_ASSEMBLER_INCLUDED
