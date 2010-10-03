#ifndef KUEKEN_RASTERIZER_INCLUDED
#define KUEKEN_RASTERIZER_INCLUDED

#include "detail/detail.hpp"

namespace kueken
{
	class renderer;

namespace rasterizer{

	enum type
	{
		POLYGON,
		LINE,
		POINT,
		DISCARD
	};

namespace detail{

	struct data
	{
		data();

		glm::uint32 Id;

		type Type;

		glm::ivec4 Viewport;
		GLenum FrontFace;
		GLenum CullFace;
		bool CullFaceEnabled;
		bool Multisampling;
		glm::ivec4 Scissor;
		bool ScissorEnabled;
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
	class creator<POLYGON> : public kueken::detail::creator
	{
		friend class object;

	public:
		void setId(glm::uint32 Id);
		void setViewport(glm::ivec4 const & Viewport);
		void setMultisample(bool Multisample);
		void setScissor(bool Enabled, glm::ivec4 const & Rect);

		void setFrontFace(front Front);
		void setCullface(cull Cull);
		void setOffset(float Factor, float Units);

		virtual bool validate();

	private:
		detail::data Data;
	};

	template <>
	class creator<LINE> : public kueken::detail::creator
	{
		friend class object;

	public:
		void setId(glm::uint32 Id);
		void setViewport(glm::ivec4 const & Viewport);
		void setMultisample(bool Multisample);
		void setScissor(bool Enabled, glm::ivec4 const & Rect);

		void setSize(float Size);
		void setSmooth(bool Enable);

		virtual bool validate();

	private:
		detail::data Data;
	};

	template <>
	class creator<POINT> : public kueken::detail::creator
	{
		friend class object;

	public:
		void setId(glm::uint32 Id);
		void setViewport(glm::ivec4 const & Viewport);
		void setMultisample(bool Multisample);
		void setScissor(bool Enabled, glm::ivec4 const & Rect);

		void setSize(float Size, float Min, float Max);
		void setFadeSize(float Size);
		void setAttenuation(glm::vec3 const & Attenuation);
		void setSmooth(bool Enable);

		virtual bool validate();

	private:
		detail::data Data;
	};

	template <>
	class creator<DISCARD> : public kueken::detail::creator
	{
		friend class object;

	public:

	private:
		detail::data Data;
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
