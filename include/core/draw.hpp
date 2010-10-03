#ifndef KUEKEN_DRAW_INCLUDED
#define KUEKEN_DRAW_INCLUDED

#include "detail/detail.hpp"
#include "buffer.hpp"

namespace kueken
{
	class renderer;

namespace draw{

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
		ELEMENT
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
	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

	template <type TYPE>
	class creator
	{};

	template <>
	class creator<ARRAY> : public kueken::detail::creator
	{
		friend class object;

	public:
		creator();
		void setPrimitive(primitive const & Primitive);
		void setFirst(glm::uint32 const & First);
		void setCount(glm::uint32 const & Count);
		void setInstances(glm::uint const & Instances);

		bool validate();

	private:
		detail::data Data;
	};

	template <>
	class creator<ELEMENT> : public kueken::detail::creator
	{
		friend class object;

	public:
		creator();
		void setPrimitive(primitive const & Primitive);
		void setFirst(glm::uint const & First);
		void setCount(glm::uint const & Count);
		void setElementFormat(format const & ElementFormat);
		void setInstances(glm::uint const & Instances);
		void setBaseVertex(glm::uint const & BaseVertex);

		bool validate();

	private:
		detail::data Data;
	};

	class object
	{
		template <type TYPE>
		friend class creator;

	public:
		object(creator<ARRAY> const & Creator);
		object(creator<ELEMENT> const & Creator);
		~object();

		void exec();

	private:
		detail::data Data;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace draw
}//namespace kueken

#include "clear.inl"

#endif//KUEKEN_DRAW_INCLUDED
