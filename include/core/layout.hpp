#ifndef KUEKEN_LAYOUT_INCLUDED
#define KUEKEN_LAYOUT_INCLUDED

#include "detail/detail.hpp"

namespace kueken
{
	class renderer;
}//namespace kueken

namespace kueken{
namespace layout
{
	enum target
	{
		VERTEX
	};

	//enum semantic
	//{
	//	POSITION		= 0,
	//	COLOR			= 3,
	//	TEXCOORD		= 4
	//};

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
		I8VEC1,
		I8VEC2,
		I8VEC3,
		I8VEC4,
		I16VEC1,
		I16VEC2,
		I16VEC3,
		I16VEC4,
		I32VEC1,
		I32VEC2,
		I32VEC3,
		I32VEC4,
		U8VEC1,
		U8VEC2,
		U8VEC3,
		U8VEC4,
		U16VEC1,
		U16VEC2,
		U16VEC3,
		U16VEC4,
		U32VEC1,
		U32VEC2,
		U32VEC3,
		U32VEC4,
		ARRAY_MAX
	};

	typedef std::size_t slot;
	typedef std::size_t semantic;

namespace detail{

	struct vertexArray
	{
		slot Slot;
		semantic Semantic;
		GLint Size;
		GLenum Type;
		GLsizei Stride;
		GLvoid const * Offset;
		GLuint Divisor;
	};

	typedef std::vector<vertexArray> semanticData;
	typedef std::vector<semanticData> slotData;

}//namespace detail

	class creator// : public kueken::detail::creator
	{
		friend class object;

	public:
		creator(renderer & Renderer);
		void setVertexArray(
			slot const & Slot,
			semantic const & Semantic,
			array const & Format, 
			std::size_t const & Stride,
			std::size_t const & Offset,
			std::size_t const & Divisor);

		virtual bool validate(){assert(0); return false;}

	private:
		renderer & Renderer;
		detail::slotData SlotData;
	};

	class object
	{
	public:
		object(creator const & Creator);
		~object();

		void bind();

	private:
		renderer & Renderer;
		detail::slotData SlotData;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace layout
}//namespace kueken

#include "layout.inl"

#endif//KUEKEN_LAYOUT_INCLUDED
