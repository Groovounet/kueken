#include <core/rasterizer.hpp>

namespace
{
	GLenum rasterizer_front_cast(kueken::rasterizer::front Front)
	{
		static GLenum const Cast[] = 
		{
			GL_CCW,		// CCW
			GL_CW		// CW
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::rasterizer::FRONT_MAX,
			"Cast array size mismatch");

		return Cast[Front];
	}

	GLenum rasterizer_cull_cast(kueken::rasterizer::cull Cull)
	{
		static GLenum const Cast[] =
		{
			GL_NONE,			// CULL_NONE
			GL_FRONT,			// FRONT
			GL_BACK,			// BACK
			GL_FRONT_AND_BACK	// FRONT_AND_BACK
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::rasterizer::CULL_MAX,
			"Cast array size mismatch");

		return Cast[Cull];
	}

	GLenum rasterizer_provoking_vertex_cast(kueken::rasterizer::provoking Mode)
	{
		static GLenum const Cast[] = 
		{
			GL_FIRST_VERTEX_CONVENTION,		// FIRST_VERTEX
			GL_LAST_VERTEX_CONVENTION 		// LAST_VERTEX
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::rasterizer::PROMOKING_MAX,
			"Cast array size mismatch");

		return Cast[Mode];
	}

}//namespace

namespace kueken{
namespace rasterizer{
namespace detail{

	data::data() :
		FrontFace(GL_CCW),
		CullFace(GL_BACK),
		CullFaceEnabled(false),
		Multisampling(false),
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

		Discard(false),
		ProvokeMode(GL_LAST_VERTEX_CONVENTION)
	{
		for(std::size_t i = 0; i < VIEWPORT_MAX; ++i)
			this->ScissorsEnabled[i] = false;
	}

	///////////////////////////////
	// detail::creator

	void creator::setViewport
	(
		viewport const & Viewport,
		glm::vec4 const & Sizes
	)
	{
		assert(std::size_t(Viewport) < this->Data.Viewports.size());
		this->Data.Viewports[Viewport] = Sizes;
	}

	void creator::setScissor
	(
		viewport const & Viewport, 
		bool Enabled, 
		glm::ivec4 const & Rect
	)
	{
		assert(std::size_t(Viewport) < this->Data.ScissorsEnabled.size());
		this->Data.ScissorsEnabled[Viewport] = Enabled;

		assert(std::size_t(Viewport) < this->Data.Scissors.size());
		this->Data.Scissors[Viewport] = Rect;
	}

	void creator::setDepthRange
	(
		viewport const & Viewport, 
		float Near, 
		float Far
	)
	{
		assert(std::size_t(Viewport) < this->Data.DepthRanges.size());
		this->Data.DepthRanges[Viewport] = glm::vec2(Near, Far);
	}

	void creator::setMultisample(bool Multisample)
	{
		this->Data.Multisampling = Multisample;
	}

}//namespace detail

	///////////////////////////////
	// creator<POLYGON>

	creator<POLYGON>::creator
	(
		renderer & Renderer
	)
	{}

	void creator<POLYGON>::setFrontFace(front Front)
	{
		Data.CullFace = rasterizer_front_cast(Front);
	}

	void creator<POLYGON>::setCullface(cull Cull)
	{
		Data.CullFaceEnabled = Cull != CULL_NONE;
		Data.CullFace = rasterizer_cull_cast(Cull);
	}

	void creator<POLYGON>::setProvokingVertex(provoking const & Mode)
	{
		Data.ProvokeMode = Mode;
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

	creator<LINE>::creator
	(
		renderer & Renderer
	)
	{}

	void creator<LINE>::setSize(float Size)
	{
		Data.LineSize = Size;
	}

	void creator<LINE>::setSmooth(bool Enable)
	{
		Data.LineSmooth = Enable;
	}

	void creator<LINE>::setProvokingVertex(provoking const & Mode)
	{
		Data.ProvokeMode = Mode;
	}

	bool creator<LINE>::validate()
	{
		return true;
	}

	///////////////////////////////
	// creator<POINT>

	creator<POINT>::creator
	(
		renderer & Renderer
	)
	{}

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

	template <>
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

	template <>
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

	template <>
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

	template <>
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

		glViewportArrayv(0, VIEWPORT_MAX, &Data.Viewports[0][0]);

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

		for(std::size_t i = 0; i < VIEWPORT_MAX; ++i)
		{
			if(Data.ScissorsEnabled[i])
				glEnableIndexedEXT(GL_SCISSOR_TEST, i);
			else
				glDisableIndexedEXT(GL_SCISSOR_TEST, i);
		}

		assert(glGetError() == GL_NO_ERROR);

		glScissorArrayv(
			0,
			VIEWPORT_MAX,
			&Data.Scissors[0][0]);

		assert(glGetError() == GL_NO_ERROR);

		glDepthRangeArrayv(
			0,
			VIEWPORT_MAX,
			&Data.DepthRanges[0][0]);

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

		glProvokingVertex(Data.ProvokeMode);

		assert(glGetError() == GL_NO_ERROR);
	}

}//namespace rasterizer
}//namespace kueken
