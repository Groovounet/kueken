#include <core/rasterizer.hpp>

namespace
{
	GLenum rasterizer_front_cast(kueken::rasterizer::front Front)
	{
		static GLenum const Cast[kueken::rasterizer::FRONT_MAX] = 
		{
			GL_CCW,		// CCW
			GL_CW		// CW
		};

		return Cast[Front];
	}

	GLenum rasterizer_cull_cast(kueken::rasterizer::cull Cull)
	{
		static GLenum const Cast[kueken::rasterizer::CULL_MAX] =
		{
			GL_NONE,			// CULL_NONE
			GL_FRONT,			// FRONT
			GL_BACK,			// BACK
			GL_FRONT_AND_BACK	// FRONT_AND_BACK
		};

		return Cast[Cull];
	}

}//namespace

namespace kueken{
namespace rasterizer{
namespace detail{

	data::data() :
		Id(0),
		Viewport(0),
		FrontFace(GL_CCW),
		CullFace(GL_BACK),
		CullFaceEnabled(false),
		Multisampling(false),
		ScissorEnabled(false),
		Scissor(0),
		OffsetFactor(0),
		OffsetUnits(0),

		PointSize(1),
		PointMin(0), 
		PointMax(1),
		PointFade(1),
		PointAttenuation(1, 0, 0),
		PointSmooth(false),

		LineSize(1),
		LineSmooth(false),

		Discard(false)
	{}

}//namespace detail

	///////////////////////////////
	// creator<POLYGON>

	void creator<POLYGON>::setId(glm::uint32 Id)
	{
		Data.Id = Id;
	}

	void creator<POLYGON>::setViewport(glm::ivec4 const & Viewport)
	{
		Data.Viewport = Viewport;
	}

	void creator<POLYGON>::setFrontFace(front Front)
	{
		Data.CullFace = rasterizer_front_cast(Front);
	}

	void creator<POLYGON>::setCullface(cull Cull)
	{
		Data.CullFaceEnabled = Cull != CULL_NONE;
		Data.CullFace = rasterizer_cull_cast(Cull);
	}

	void creator<POLYGON>::setMultisample(bool Multisample)
	{
		Data.Multisampling = Multisample;
	}

	void creator<POLYGON>::setScissor(bool Enabled, glm::ivec4 const & Rect)
	{
		Data.ScissorEnabled = Enabled;
		Data.Scissor = Rect;
	}

	void creator<POLYGON>::setOffset(float Factor, float Units)
	{
		Data.OffsetFactor = Factor;
		Data.OffsetUnits = Units;
	}

	bool creator<POLYGON>::validate()
	{
		return true;
	}

	///////////////////////////////
	// creator<LINE>

	void creator<LINE>::setId(glm::uint32 Id)
	{
		Data.Id = Id;
	}

	void creator<LINE>::setViewport(glm::ivec4 const & Viewport)
	{
		Data.Viewport = Viewport;
	}

	void creator<LINE>::setMultisample(bool Multisample)
	{
		Data.Multisampling = Multisample;
	}

	void creator<LINE>::setScissor(bool Enabled, glm::ivec4 const & Rect)
	{
		Data.ScissorEnabled = Enabled;
		Data.Scissor = Rect;
	}

	void creator<LINE>::setSize(float Size)
	{
		Data.LineSize = Size;
	}

	void creator<LINE>::setSmooth(bool Enable)
	{
		Data.LineSmooth = Enable;
	}

	bool creator<LINE>::validate()
	{
		return true;
	}

	///////////////////////////////
	// creator<POINT>

	void creator<POINT>::setId(glm::uint32 Id)
	{
		Data.Id = Id;
	}

	void creator<POINT>::setViewport(glm::ivec4 const & Viewport)
	{
		Data.Viewport = Viewport;
	}

	void creator<POINT>::setMultisample(bool Multisample)
	{
		Data.Multisampling = Multisample;
	}

	void creator<POINT>::setScissor(bool Enabled, glm::ivec4 const & Rect)
	{
		Data.ScissorEnabled = Enabled;
		Data.Scissor = Rect;
	}

	bool creator<POINT>::validate()
	{
		return true;
	}

	void creator<POINT>::setSize(float Size, float Min, float Max)
	{
		Data.PointSize = Size;
		Data.PointMin = Min;
		Data.PointMax = Max;
	}

	void creator<POINT>::setFadeSize(float Size)
	{
		Data.PointFade = Size;
	}

	void creator<POINT>::setAttenuation(glm::vec3 const & Attenuation)
	{
		Data.PointAttenuation = Attenuation;
	}

	void creator<POINT>::setSmooth(bool Enable)
	{
		Data.PointSmooth = Enable;
	}

	///////////////////////////////
	// creator<DISCARD>

	///////////////////////////////
	// object

	object::object
	(
		creator<POLYGON> const & Creator
	) :
		Data(Creator.Data),
		Name(0)
	{
		Data.Type = POLYGON;

#if KUEKEN_STATE_OBJECTS
		Name = glGenLists(1);

		glNewList(Name, GL_COMPILE);
			run(POLYGON);
		glEndList();
#endif//KUEKEN_STATE_OBJECTS
	}

	object::object
	(
		creator<LINE> const & Creator
	) :
		Data(Creator.Data),
		Name(0)
	{
		Data.Type = LINE;

#if KUEKEN_STATE_OBJECTS
		Name = glGenLists(1);

		glNewList(Name, GL_COMPILE);
			run(LINE);
		glEndList();
#endif//KUEKEN_STATE_OBJECTS	
	}

	object::object
	(
		creator<POINT> const & Creator
	) :
		Data(Creator.Data),
		Name(0)
	{
		Data.Type = POINT;

#if KUEKEN_STATE_OBJECTS
		Name = glGenLists(1);

		glNewList(Name, GL_COMPILE);
			run(POINT);
		glEndList();
#endif//KUEKEN_STATE_OBJECTS
	}

	object::object
	(
		creator<DISCARD> const & Creator
	) :
		Data(Creator.Data),
		Name(0)
	{
		Data.Type = DISCARD;
		Data.Discard = true;

#if KUEKEN_STATE_OBJECTS
		Name = glGenLists(1);

		//glNewList(Name, GL_COMPILE);
			//run(DISCARD);
		//glEndList();
#endif//KUEKEN_STATE_OBJECTS
	}

	object::~object()
	{
#if KUEKEN_STATE_OBJECTS
		glDeleteLists(Name, 1); 
#endif//KUEKEN_STATE_OBJECTS
	}

	void object::bind()
	{
#if KUEKEN_STATE_OBJECTS
		glCallList(Name);
#else
		run(Data.Type);
#endif//KUEKEN_STATE_OBJECTS
	}

	void object::run(type Type)
	{
		assert(glGetError() == GL_NO_ERROR);

		glViewport(Data.Viewport.x, Data.Viewport.y, Data.Viewport.z, Data.Viewport.w);

		assert(glGetError() == GL_NO_ERROR);

		if(Data.Multisampling)
			glEnable(GL_MULTISAMPLE);
		else
			glDisable(GL_MULTISAMPLE);

		assert(glGetError() == GL_NO_ERROR);

		if(Data.CullFaceEnabled)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(Data.CullFace);
		}
		else
			glDisable(GL_CULL_FACE);

		glFrontFace(Data.FrontFace);

		assert(glGetError() == GL_NO_ERROR);

		if(Data.ScissorEnabled)
			glEnable(GL_SCISSOR_TEST);
		else
			glDisable(GL_SCISSOR_TEST);
		glScissor(
			Data.Scissor.x, 
			Data.Scissor.y, 
			Data.Scissor.z, 
			Data.Scissor.w);

		assert(glGetError() == GL_NO_ERROR);

		glPolygonOffset(
			Data.OffsetFactor, 
			Data.OffsetUnits);

		assert(glGetError() == GL_NO_ERROR);

		glPointSize(Data.PointSize);

		assert(glGetError() == GL_NO_ERROR);
		//glPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE, Data.PointFade);
		assert(glGetError() == GL_NO_ERROR);

		//if(Data.PointSmooth)
		//	glEnable(GL_POINT_SMOOTH);
		//else
		//	glDisable(GL_POINT_SMOOTH);

		assert(glGetError() == GL_NO_ERROR);

		glLineWidth(Data.LineSize);
		if(Data.LineSmooth)
			glEnable(GL_LINE_SMOOTH);
		else
			glDisable(GL_LINE_SMOOTH);

		assert(glGetError() == GL_NO_ERROR);

		if(Data.Discard)
			glEnable(GL_RASTERIZER_DISCARD);
		else
			glDisable(GL_RASTERIZER_DISCARD);

		assert(glGetError() == GL_NO_ERROR);
	}

}//namespace rasterizer
}//namespace kueken
