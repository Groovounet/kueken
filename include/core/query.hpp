#ifndef KUEKEN_QUERY_INCLUDED
#define KUEKEN_QUERY_INCLUDED

#include "detail/detail.hpp"

namespace kueken
{
	class renderer;
}//namespace kueken

namespace kueken{
namespace query
{
	enum target
	{
		PRIMITIVES,		//GL_PRIMITIVES_GENERATED
		SAMPLES,		//GL_SAMPLES_PASSED
		TIME,			//GL_TIME_ELAPSED_EXT
		//FEEDBACK,		//GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN
		TARGET_MAX
	};

	enum mode
	{
		QUERY_WAIT,					//GL_QUERY_WAIT
		QUERY_NO_WAIT,				//GL_QUERY_NO_WAIT
		QUERY_BY_REGION_WAIT,		//GL_QUERY_BY_REGION_WAIT
		QUERY_BY_REGION_NO_WAIT,	//GL_QUERY_BY_REGION_NO_WAIT
		MODE_MAX
	};

	enum use
	{
		RECORD,
		CONDITION
	};

namespace detail
{
	struct data
	{
		data();
		GLenum Target;
		GLenum ConditionMode;
		bool Wait;
	};
}//namespace detail

	class creator
	{
		friend class object;

	public:
		creator(renderer & Renderer);
		void setTarget(target Target);
		void setCondition(mode Mode);
		void setWait(bool Wait);
		
	private:
		detail::data Data;
	};

	class object
	{
		friend class renderer;

	public:
		object(creator const & Creator);
		~object();

		void begin(use const & Use);
		void end(use const & Use);
		glm::uint64 get();

	private:
		void run();

		detail::data Data;
		GLenum Name;
		glm::uint64 Result;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace query
}//namespace kueken

#include "query.inl"

#endif//KUEKEN_QUERY_INCLUDED
