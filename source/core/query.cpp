#include <core/query.hpp>
#include <core/manager.hpp>

namespace
{
	GLenum query_target_cast(kueken::query::target Target)
	{
		static GLenum const Cast[kueken::query::TARGET_MAX] =
		{
			GL_PRIMITIVES_GENERATED,					//PRIMITIVES_GENERATED,
			//GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN,	//PRIMITIVES_WRITTEN,
			GL_SAMPLES_PASSED,							//SAMPLES_PASSED
			GL_TIME_ELAPSED_EXT							//TIME
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::query::TARGET_MAX,
			"Cast array size mismatch");

		return Cast[Target];
	}

	GLenum query_mode_cast(kueken::query::mode Mode)
	{
		static GLenum const Cast[kueken::query::MODE_MAX] =
		{
			GL_QUERY_WAIT,					//QUERY_WAIT
			GL_QUERY_NO_WAIT,				//QUERY_NO_WAIT
			GL_QUERY_BY_REGION_WAIT,		//QUERY_BY_REGION_WAIT
			GL_QUERY_BY_REGION_NO_WAIT,		//QUERY_BY_REGION_NO_WAIT
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::query::MODE_MAX,
			"Cast array size mismatch");

		return Cast[Mode];
	}
}

namespace kueken{
namespace query{

namespace detail
{
	data::data() :
		Target(GL_NONE),
		ConditionMode(GL_QUERY_WAIT),
		Wait(false)
	{}
}//namespace detail

	//////////////////////////////////
	// creator

	void creator::setTarget(target Target)
	{
		Data.Target = query_target_cast(Target);
	}

	void creator::setCondition(mode Mode)
	{
		Data.ConditionMode = query_mode_cast(Mode);
	}

	void creator::setWait(bool Wait)
	{
		Data.Wait = Wait;
	}

	//////////////////////////////////
	// object

	object::object(creator const & Creator) :
		Data(Creator.Data),
		Result(0)
	{
		glGenQueries(1, &Name);
	}

	object::~object()
	{
		glDeleteQueries(1, &Name);
	}

	void object::begin(use const & Use)
	{
		switch(Use)
		{
		case RECORD:
			glBeginQuery(Data.Target, Name);
			break;
		case CONDITION:
			glBeginConditionalRenderNV(Name, Data.ConditionMode);
			break;
		}
	}

	void object::end(use const & Use)
	{
		switch(Use)
		{
		case RECORD:
			glEndQuery(Data.Target);
			break;
		case CONDITION:
			glEndConditionalRenderNV();
			break;
		}
	}

	glm::uint64 object::get()
	{
		GLint Available = 0;
		while(!Available && Data.Wait)
		{
			glGetQueryObjectiv(
				Name, 
				GL_QUERY_RESULT_AVAILABLE, 
				&Available);
		}

		GLuint64EXT Result;
		glGetQueryObjectui64vEXT(
			Name, 
			GL_QUERY_RESULT, 
			&Result);
		return glm::uint64(Result);
	}

}//namespace query
}//namespace kueken
