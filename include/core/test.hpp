#ifndef KUEKEN_TEST_INCLUDED
#define KUEKEN_TEST_INCLUDED

#include "detail/detail.hpp"

namespace kueken{
namespace test{

	enum function
	{
		NEVER,						// GL_NEVER
		LESS,						// GL_LESS
		EQUAL,						// GL_EQUAL
		LEQUAL,						// GL_LEQUAL
		GREATER,					// GL_GREATER
		NOTEQUAL,					// GL_NOTEQUAL
		GEQUAL,						// GL_GEQUAL
		ALWAYS,						// GL_ALWAYS
		FUNCTION_MAX
	};

	enum face
	{
		FRONT,
		BACK,
		FACE_MAX
	};

	enum action
	{
		KEEP,		// GL_KEEP, 
		ZERO,		// GL_ZERO, 
		REPLACE,	// GL_REPLACE, 
		INCR,		// GL_INCR, 
		INCR_WRAP,	// GL_INCR_WRAP, 
		DECR,		// GL_DECR, 
		DECR_WRAP,	// GL_DECR_WRAP, 
		INVERT,		// GL_INVERT
		ACTION_MAX
	};

namespace detail
{
	struct data
	{
		data();

		bool DepthTest;
		GLenum DepthFunc;
		float DepthNear;
		float DepthFar;

		bool StencilTest;
		GLenum StencilFunc[FACE_MAX];
		GLint StencilRef[FACE_MAX];
		GLuint StencilMask[FACE_MAX];
		GLenum StencilSFail[FACE_MAX];
		GLenum StencilDFail[FACE_MAX];
		GLenum StencilDPass[FACE_MAX];
	};

}//namespace detail

	class creator : public kueken::detail::creator
	{
		friend class object;

	public:
		void setDepthEnable(bool Enabled);
		void setDepthFunc(function Func);
		void setDepthRange(float Near, float Far);

		void setStencilEnable(bool Enabled);
		void setStencilFunc(face Face, function Func, int Ref, glm::uint Mask);
		void setStencilOp(face Face, action sfail, action dpfail, action dppass);

		virtual bool validate(){assert(0); return true;}

	private:
		detail::data Data;
	};

	class object
	{
	public:
		object(creator const & Creator);
		~object();

		void bind();

	private:
		void run();

		detail::data Data;
		GLuint Name;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace test
}//namespace kueken

#include "test.inl"

#endif//KUEKEN_TEST_INCLUDED
