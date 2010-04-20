#ifndef KUEKEN_SAMPLER_INCLUDED
#define KUEKEN_SAMPLER_INCLUDED

#include "detail/detail.hpp"

namespace kueken{
namespace sampler{
namespace detail{

	struct data
	{
		data() :
			Target(GL_TEXTURE_2D),
			Min(GL_NEAREST),
			Mag(GL_NEAREST),
			WrapS(GL_CLAMP),
			WrapT(GL_CLAMP),
			WrapQ(GL_CLAMP),
			Anisotropy(1.0f),
			Unit(0),
			LodMin(-1000.f),
			LodMax(1000.f),
			LodBias(0.0f)
		{
			Swizzle[0] = GL_RED;
			Swizzle[1] = GL_GREEN;
			Swizzle[2] = GL_BLUE;
			Swizzle[3] = GL_ALPHA;
		}

		GLenum Target;
		GLenum Min;
		GLenum Mag;
		GLenum WrapS;
		GLenum WrapT;
		GLenum WrapQ;
		GLfloat Anisotropy;
		GLint Swizzle[4];
		GLenum Unit;
		GLfloat LodMin;
		GLfloat LodMax;
		GLfloat LodBias;
	};

}//namespace detail

	enum filter
	{
		NEAREST,
		BILINEAR,
		TRILINEAR,
		FILTER_MAX
	};

	enum wrap
	{
		CLAMP,
		CLAMP_TO_BORDER,
		CLAMP_TO_EDGE,
		MIRRORED_REPEAT,
		REPEAT,
		WRAP_MAX
	};

	enum target
	{
		IMAGE1D,
		IMAGE2D,
		IMAGE3D,
		ARRAY1D,
		ARRAY2D,
		RECT,
		CUBE_POS_X,
		CUBE_NEG_X,
		CUBE_POS_Y,
		CUBE_NEG_Y,
		CUBE_POS_Z,
		CUBE_NEG_Z,
		TARGET_MAX
	};

	enum swizzle
	{
		ZERO,
		ONE,
		RED,
		GREEN,
		BLUE,
		ALPHA,
		SWIZZLE_MAX
	};

	class creator// : public kueken::detail::creator
	{
		friend class object;

	public:
		void setFilter(filter Filter);
		void setWrap(wrap WrapS, wrap WrapT, wrap WrapQ);
		void setAnisotropy(float Anisotropy);
		void setTarget(target Target);
		void setSwizzle(swizzle R, swizzle G, swizzle B, swizzle A);
		void setLod(float Min, float Max, float Bias);

		virtual bool validate();

	private:
		detail::data Data;
	};

	class object
	{
	public:
		object(creator const & Creator);
		~object();

		void bind();
		void bind(std::size_t Unit);

		GLenum getUnit() const;

	private:
		void run(GLenum Unit);
		void run();

		detail::data Data;
		GLuint Name;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace sampler
}//namespace kueken

#include "sampler.inl"

#endif//KUEKEN_SAMPLER_INCLUDED
