#ifndef KUEKEN_RASTERIZER_INCLUDED
#define KUEKEN_RASTERIZER_INCLUDED

#include "detail/detail.hpp"

namespace kueken
{
	class renderer;
}//namespace kueken

namespace kueken{
namespace rasterizer
{
	enum target
	{
		RASTERIZER
	};

	enum type
	{
		POLYGON,
		LINE,
		POINT,
		DISCARD
	};

	enum provoking
	{
		FIRST_VERTEX,
		LAST_VERTEX,
		PROMOKING_MAX
	};

	enum viewport
	{
		VIEWPORT0,
		VIEWPORT1,
		VIEWPORT2,
		VIEWPORT3,
		VIEWPORT4,
		VIEWPORT5,
		VIEWPORT6,
		VIEWPORT7,
/*
		VIEWPORT8,
		VIEWPORT9,
		VIEWPORT10,
		VIEWPORT11,
		VIEWPORT12,
		VIEWPORT13,
		VIEWPORT14,
		VIEWPORT15,
*/
		VIEWPORT_MAX
	};

namespace detail
{
	struct data
	{
		data();

		type Type;

		std::array<glm::vec4, VIEWPORT_MAX> Viewports;
		std::array<glm::dvec2, VIEWPORT_MAX> DepthRanges;
		std::array<glm::ivec4, VIEWPORT_MAX> Scissors;
		std::array<bool, VIEWPORT_MAX> ScissorsEnabled;

		GLenum FrontFace;
		GLenum CullFace;
		bool CullFaceEnabled;
		bool Multisampling;
		float OffsetFactor;
		float OffsetUnits;

		float PointSize;
		float PointMin;
		float PointMax;
		float PointFade;
		glm::vec3 PointAttenuation;
		bool PointSmooth;

		float LineSize;
		bool LineSmooth;
		bool Discard;
		GLenum ProvokeMode;
	};

	class creator : public kueken::detail::creator
	{
	public:
		void setViewport(viewport const & Viewport, glm::vec4 const & Rect);
		void setScissor(viewport const & Viewport, bool Enabled, glm::ivec4 const & Rect);
		void setDepthRange(viewport const & Viewport, float Near, float Far);
		void setMultisample(bool Multisample);

	protected:
		data Data;
	};

}//namespace detail

	enum front
	{
		CCW,
		CW,
		FRONT_MAX
	};

	enum cull
	{
		CULL_NONE,
		FRONT,
		BACK,
		FRONT_AND_BACK,
		CULL_MAX
	};

	template <type Type>
	class creator
	{};

	template <>
	class creator<POLYGON> : public kueken::rasterizer::detail::creator
	{
		friend class object;

	public:
		creator(renderer & Renderer);

		void setFrontFace(front Front);
		void setCullface(cull Cull);
		void setProvokingVertex(provoking const & Mode);
		void setOffset(float Factor, float Units);

		virtual bool validate();
	};

	template <>
	class creator<LINE> : public kueken::rasterizer::detail::creator
	{
		friend class object;

	public:
		creator(renderer & Renderer);

		void setSize(float Size);
		void setSmooth(bool Enable);
		void setProvokingVertex(provoking const & Mode);

		virtual bool validate();
	};

	template <>
	class creator<POINT> : public kueken::rasterizer::detail::creator
	{
		friend class object;

	public:
		creator(renderer & Renderer);

		void setSize(float Size, float Min, float Max);
		void setFadeSize(float Size);
		void setAttenuation(glm::vec3 const & Attenuation);
		void setSmooth(bool Enable);

		virtual bool validate();
	};

	template <>
	class creator<DISCARD> : public kueken::rasterizer::detail::creator
	{
		friend class object;

	public:
		creator(renderer & Renderer);
	};

	class object
	{
	public:
		template <type TYPE>
		object(creator<TYPE> const & Creator);
		~object();

		void bind();

	private:
		void run(type Type);

		detail::data Data;
		GLuint Name;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace rasterizer
}//namespace kueken

#include "rasterizer.inl"

#endif//KUEKEN_RASTERIZER_INCLUDED
