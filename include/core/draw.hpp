#ifndef KUEKEN_DRAW_INCLUDED
#define KUEKEN_DRAW_INCLUDED

#include "detail/detail.hpp"
#include "buffer.hpp"

namespace kueken
{
	class renderer;
}//namespace kueken

namespace kueken{
namespace draw
{
	enum mode
	{
		QUERY_WAIT,					//GL_QUERY_WAIT
		QUERY_NO_WAIT,				//GL_QUERY_NO_WAIT
		QUERY_BY_REGION_WAIT,		//GL_QUERY_BY_REGION_WAIT
		QUERY_BY_REGION_NO_WAIT,	//GL_QUERY_BY_REGION_NO_WAIT
		MODE_MAX
	};

	enum target
	{
		PRIMITIVES,		//GL_PRIMITIVES_GENERATED
		FEEDBACK,		//GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN
		SAMPLES,		//GL_SAMPLES_PASSED
		TIME,			//GL_TIME_ELAPSED_EXT
		TARGET_MAX
	};

	enum primitive
	{
		TRIANGLES
	};

	enum format
	{
		UINT8,
		UINT16,
		UINT32
	};

	enum type
	{
		ARRAY,
		ELEMENT,
		ARRAY_INDIRECT,
		ELEMENT_INDIRECT
	};

	struct DrawArraysIndirectCommand
	{
		DrawArraysIndirectCommand() :
			Count(0),
			PrimCount(1),
			First(0),
			ReservedMustBeZero(0)
		{}

		glm::uint32 Count;
		glm::uint32 PrimCount;
		glm::int32 First;
		glm::uint32 ReservedMustBeZero;
	};

	struct DrawElementsIndirectCommand
	{
		glm::uint32 Count;
		glm::uint32 PrimCount;
		glm::uint32 FirstIndex;
		glm::int32 BaseVertex;
		glm::uint32 ReservedMustBeZero;
	};

namespace detail
{
	struct data
	{
		data(type const & Type);
		type Type;
		GLenum Mode;
		GLint First;
		GLsizei Count;
		GLenum ElementType;
		const void* Indices;
		GLsizei PrimCount; 
		GLint BaseVertex;
	};
}//namespace detail

	class object;
	class objectArray;
	class objectArrayIndirect;
	class objectElement;
	class objectElementIndirect;
	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

	template <type TYPE>
	class creator
	{};

	template <>
	class creator<ARRAY> : public kueken::detail::creator
	{
		friend class objectArray;

	public:
		creator(renderer & Renderer);
		void setPrimitive(primitive const & Primitive);
		void setFirst(glm::uint32 const & First);
		void setCount(glm::uint32 const & Count);
		void setInstances(glm::uint const & Instances);

		bool validate();

	protected:
		detail::data Data;
	};

	template <>
	class creator<ARRAY_INDIRECT> : public kueken::detail::creator
	{
		friend class objectArrayIndirect;

	public:
		creator(renderer & Renderer);
		void setPrimitive(primitive const & Primitive);

		bool validate();

	protected:
		detail::data Data;
	};

	template <>
	class creator<ELEMENT> : public kueken::detail::creator
	{
		friend class objectElement;

	public:
		creator(renderer & Renderer);
		void setPrimitive(primitive const & Primitive);
		void setFirst(glm::uint const & First);
		void setCount(glm::uint const & Count);
		void setElementFormat(format const & ElementFormat);
		void setInstances(glm::uint const & Instances);
		void setBaseVertex(glm::uint const & BaseVertex);

		bool validate();

	protected:
		detail::data Data;
	};

	template <>
	class creator<ELEMENT_INDIRECT> : public kueken::detail::creator
	{
		friend class objectElementIndirect;

	public:
		creator(renderer & Renderer);
		void setPrimitive(primitive const & Primitive);
		void setElementFormat(format const & ElementFormat);

		bool validate();

	protected:
		detail::data Data;
	};

	class object
	{
	public:
		object(detail::data const & Data);
		virtual ~object(){}

		virtual void exec() = 0;

	protected:
		detail::data Data;
	};

	class objectArray : public object
	{
		friend class creator<ARRAY>;

	public:
		objectArray(creator<ARRAY> const & Creator);
		virtual ~objectArray();

		void exec();
	};

	class objectElement : public object
	{
		friend class creator<ELEMENT>;

	public:
		objectElement(creator<ELEMENT> const & Creator);
		virtual ~objectElement();

		void exec();
	};

	class objectArrayIndirect : public object
	{
		friend class creator<ARRAY_INDIRECT>;

	public:
		objectArrayIndirect(creator<ARRAY_INDIRECT> const & Creator);
		virtual ~objectArrayIndirect();

		void exec();
	};

	class objectElementIndirect : public object
	{
		friend class creator<ELEMENT_INDIRECT>;

	public:
		objectElementIndirect(creator<ELEMENT_INDIRECT> const & Creator);
		virtual ~objectElementIndirect();

		void exec();
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace draw
}//namespace kueken

#include "clear.inl"

#endif//KUEKEN_DRAW_INCLUDED
