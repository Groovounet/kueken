#include <core/draw.hpp>
#include <core/manager.hpp>

#define KUEKEN_BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace
{
	GLenum draw_target_cast(kueken::draw::target Target)
	{
		static GLenum const Cast[kueken::draw::TARGET_MAX] =
		{
			GL_PRIMITIVES_GENERATED,					//PRIMITIVES_GENERATED,
			GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN,	//PRIMITIVES_WRITTEN,
			GL_SAMPLES_PASSED,							//SAMPLES_PASSED
			GL_TIME_ELAPSED_EXT							//TIME
		};

		return Cast[Target];
	}

	GLenum draw_mode_cast(kueken::draw::mode Mode)
	{
		static GLenum const Cast[kueken::draw::MODE_MAX] =
		{
			GL_QUERY_WAIT,				//QUERY_WAIT,
			GL_QUERY_NO_WAIT,			//QUERY_NO_WAIT,
			GL_QUERY_BY_REGION_WAIT,	//QUERY_BY_REGION_WAIT,
			GL_QUERY_BY_REGION_NO_WAIT	//QUERY_BY_REGION_NO_WAIT
		};

		return Cast[Mode];
	}
}//namespace

namespace kueken{
namespace draw{
namespace detail{

	//feedback::feedback() :
	//	Buffer(0),
	//	Offset(0),
	//	Size(0)
	//{}

	//query::query() :
	//	Name(0),
	//	Target(0),
	//	Enabled(false)
	//{}

	//condition::condition() :
	//	Query(0),
	//	Mode(0),
	//	Enabled(false)
	//{}

	data::data(type const & Type) :
		Type(Type),
		Mode(GL_NONE),
		Count(0),
		ElementType(GL_UNSIGNED_INT),
		Indices(0),
		PrimCount(1),
		BaseVertex(0)
	{}
}//namespace detail

	///////////////////////
	// creator<ARRAY>

	creator<ARRAY>::creator() :
		Data(ARRAY)
	{}

	void creator<ARRAY>::setPrimitive(primitive const & Primitive)
	{
		GLenum Cast[] = 
		{
			GL_TRIANGLES
		};

		assert(Primitive < sizeof(Cast) / sizeof(GLenum));

		this->Data.Mode = Cast[Primitive];
	}

	void creator<ARRAY>::setFirst(glm::uint32 const & First)
	{
		this->Data.First = First;
	}

	void creator<ARRAY>::setCount(glm::uint32 const & Count)
	{
		this->Data.Count = Count;
	}

	void creator<ARRAY>::setInstances(glm::uint const & Instances)
	{
		this->Data.PrimCount = Instances;
	}

	bool creator<ARRAY>::validate()
	{
		assert(0); 
		return false;
	}

	///////////////////////
	// creator<ELEMENT>

	creator<ELEMENT>::creator() :
		Data(ELEMENT)
	{}

	void creator<ELEMENT>::setPrimitive(primitive const & Primitive)
	{
		GLenum Cast[] = 
		{
			GL_TRIANGLES
		};

		assert(Primitive < sizeof(Cast) / sizeof(GLenum));

		this->Data.Mode = Cast[Primitive];
	}

	void creator<ELEMENT>::setElementFormat(format const & ElementFormat)
	{
		GLenum Cast[] = 
		{
			GL_UNSIGNED_BYTE,
			GL_UNSIGNED_SHORT,
			GL_UNSIGNED_INT
		};

		assert(ElementFormat < sizeof(Cast) / sizeof(GLenum));

		this->Data.ElementType = Cast[ElementFormat];
	}

	void creator<ELEMENT>::setFirst(glm::uint const & First)
	{
		this->Data.Indices = KUEKEN_BUFFER_OFFSET(First);
	}

	void creator<ELEMENT>::setCount(glm::uint const & Count)
	{
		this->Data.Count = Count;
	}

	void creator<ELEMENT>::setInstances(glm::uint const & Instances)
	{
		this->Data.PrimCount = Instances;
	}

	void creator<ELEMENT>::setBaseVertex(glm::uint const & BaseVertex)
	{
		this->Data.BaseVertex = BaseVertex;
	}

	bool creator<ELEMENT>::validate()
	{
		assert(0); 
		return false;
	}

	//void creator::setCondition(draw::name Draw, target Target, mode Mode)
	//{
	//	kueken::manager& Manager = kueken::manager::instance();

	//	draw::object & Object = Manager.Draw.getObject(Draw);

	//	Data.Condition.Query = Object.Data.Queries[Target].Name;
	//	Data.Condition.Mode = draw_mode_cast(Mode);
	//}

	//void creator::setQuery(target Target, bool Enabled)
	//{
	//	Data.Queries[Target].Enabled = Enabled;
	//}

	//void creator::addFeedback(buffer::name Buffer, glm::uint Offset, glm::uint Size)
	//{
	//	kueken::manager& Manager = kueken::manager::instance();

	//	detail::feedback Feedback;
	//	Feedback.Buffer = Manager.Buffer.getObject(Buffer).GetName();
	//	Feedback.Offset = Offset;
	//	Feedback.Size = Size;
	//	Data.Feedbacks.push_back(Feedback);
	//}

	object::object
	(
		creator<ARRAY> const & Creator
	) :
		Data(Creator.Data)
	{
		//for(std::size_t i = 0; i < TARGET_MAX; ++i)
		//{
		//	if(!Data.Queries[i].Enabled)
		//		continue;

		//	Data.Queries[i].Target = draw_target_cast(target(i));
		//	glGenQueries(1, &Data.Queries[i].Name);
		//}
	}

	object::object
	(
		creator<ELEMENT> const & Creator
	) :
		Data(Creator.Data)
	{
		//for(std::size_t i = 0; i < TARGET_MAX; ++i)
		//{
		//	if(!Data.Queries[i].Enabled)
		//		continue;

		//	Data.Queries[i].Target = draw_target_cast(target(i));
		//	glGenQueries(1, &Data.Queries[i].Name);
		//}
	}

	object::~object()
	{
		//for(std::size_t i = 0; i < TARGET_MAX; ++i)
		//	if(Data.Queries[i].Enabled)
		//		glDeleteQueries(1, &Data.Queries[i].Name);
	}

	void object::exec()
	{
		//if(Data.Condition.Enabled)
		//{
		//	glBeginConditionalRenderNV(
		//		Data.Condition.Query,
		//		Data.Condition.Mode);
		//}

		//for(std::size_t i = 0; i < TARGET_MAX; ++i)
		//{
		//	if(Data.Queries[i].Enabled)
		//		glBeginQuery(
		//			Data.Queries[i].Target, 
		//			Data.Queries[i].Name);
		//}

		//if(!Data.Feedbacks.empty())
		//{
		//	for(std::size_t i = 0; i < Data.Feedbacks.size(); ++i)
		//	{
		//		glBindBufferRange(
		//			GL_TRANSFORM_FEEDBACK_BUFFER,
		//			GLuint(i),
		//			Data.Feedbacks[i].Buffer,
		//			Data.Feedbacks[i].Offset,
		//			Data.Feedbacks[i].Size);
		//	}

		//	glBeginTransformFeedbackEXT(Primitive);
		//}

		if(this->Data.Type != GL_NONE)
		{
			glDrawElementsInstancedBaseVertex(
				this->Data.Mode, 
				this->Data.Count, 
				this->Data.ElementType,
				this->Data.Indices, 
				this->Data.PrimCount,
				this->Data.BaseVertex);
		}
		else
		{
			glDrawArraysInstanced(
				this->Data.Mode, 
				this->Data.First, 
				this->Data.Count, 
				this->Data.PrimCount);
		}

		//if(!Data.Feedbacks.empty())
		//	glEndTransformFeedbackEXT();

		//for(std::size_t i = 0; i < TARGET_MAX; ++i)
		//{
		//	if(Data.Queries[i].Enabled)
		//		glEndQuery(Data.Queries[i].Target);
		//}

		//if(Data.Condition.Enabled)
		//	glEndConditionalRenderNV();
	}

	//glm::uint64 object::query(target Target, bool Wait) const
	//{
	//	assert(Data.Queries[Target].Enabled);

	//	GLint Available = 0;
	//	while(!Available && Wait)
	//	{
	//		glGetQueryObjectiv(
	//			Data.Queries[Target].Name, 
	//			GL_QUERY_RESULT_AVAILABLE, 
	//			&Available);
	//	}

	//	GLuint64EXT Result;
	//	glGetQueryObjectui64vEXT(
	//		Data.Queries[Target].Name, 
	//		GL_QUERY_RESULT, 
	//		&Result);
	//	return glm::uint64(Result);
	//}

}//namespace draw
}//namespace kueken
