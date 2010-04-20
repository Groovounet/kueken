#include <core/sampler.hpp>

namespace
{
	GLenum sampler_target_cast(kueken::sampler::target Target)
	{
		static GLenum const CastSamplerTarget[kueken::sampler::TARGET_MAX] =
		{
			GL_TEXTURE_1D,						// IMAGE_1D
			GL_TEXTURE_2D,						// IMAGE_2D
			GL_TEXTURE_3D,						// IMAGE_3D
			GL_TEXTURE_1D_ARRAY_EXT,			// ARRAY_1D
			GL_TEXTURE_2D_ARRAY_EXT,			// ARRAY_2D
			GL_TEXTURE_RECTANGLE_ARB,			// RECT
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,		// CUBE_POS_X
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,		// CUBE_NEG_X
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,		// CUBE_POS_Y
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,		// CUBE_NEG_Y
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z,		// CUBE_POS_Z
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z		// CUBE_NEG_Z
		};

		return CastSamplerTarget[Target];
	}

	GLenum sampler_min_cast(kueken::sampler::filter Filter)
	{
		static GLenum const CastImageMin[kueken::sampler::FILTER_MAX] = 
		{
			GL_NEAREST,						// NEAREST
			GL_LINEAR,						// BILINEAR
			GL_LINEAR_MIPMAP_LINEAR,		// TRILINEAR
		};

		return CastImageMin[Filter];
	}

	GLenum sampler_mag_cast(kueken::sampler::filter Filter)
	{
		static GLenum const CastImageMag[kueken::sampler::FILTER_MAX] = 
		{
			GL_NEAREST,						// NEAREST
			GL_LINEAR,						// BILINEAR
			GL_LINEAR,						// TRILINEAR
		};

		return CastImageMag[Filter];
	}

	GLenum sampler_wrap_cast(kueken::sampler::wrap Wrap)
	{
		static GLenum const Cast[kueken::sampler::WRAP_MAX] = 
		{
			GL_CLAMP,						// CLAMP
			GL_CLAMP_TO_BORDER,				// CLAMP_TO_BORDER
			GL_CLAMP_TO_EDGE,				// CLAMP_TO_EDGE
			GL_MIRRORED_REPEAT,				// MIRRORED_REPEAT
			GL_REPEAT,						// REPEAT
		};

		return Cast[Wrap];
	}

	GLenum sampler_swizzle_cast(kueken::sampler::swizzle Swizzle)
	{
		static GLenum const Cast[kueken::sampler::SWIZZLE_MAX] = 
		{
			GL_ZERO,	//ZERO,
			GL_ONE,		//ONE,
			GL_RED,		//RED,
			GL_GREEN,	//GREEN,
			GL_BLUE,	//BLUE,
			GL_ALPHA,	//ALPHA
		};

		return Cast[Swizzle];
	}

}//namespace

namespace kueken{
namespace sampler
{
	//////////////////////////////////
	// Creator

	void creator::setFilter(filter Filter)
	{
		Data.Min = sampler_min_cast(Filter);
		Data.Mag = sampler_mag_cast(Filter);
	}

	void creator::setWrap(wrap WrapS, wrap WrapT, wrap WrapQ)
	{
		Data.WrapS = sampler_wrap_cast(WrapS);
		Data.WrapT = sampler_wrap_cast(WrapT);
		Data.WrapQ = sampler_wrap_cast(WrapQ);
	}

	void creator::setAnisotropy(float Anisotropy)
	{
		assert(Anisotropy >= 1.0f);

		Data.Anisotropy = Anisotropy;
	}

	void creator::setTarget(target Target)
	{
		Data.Target = Target;
	}

	void creator::setSwizzle(swizzle R, swizzle G, swizzle B, swizzle A)
	{
		Data.Swizzle[0] = sampler_swizzle_cast(R);
		Data.Swizzle[1] = sampler_swizzle_cast(G);
		Data.Swizzle[2] = sampler_swizzle_cast(B);
		Data.Swizzle[3] = sampler_swizzle_cast(A);
	}

	void creator::setLod(float Min, float Max, float Bias)
	{
		Data.LodMin = Min;
		Data.LodMax = Max;
		Data.LodBias = Bias;
	}

	bool creator::validate()
	{
		return true;
	}

	//////////////////////////////////
	// Object

	object::object
	(
		creator const & Creator
	) :
		Data(Creator.Data)
	{
#if KUEKEN_STATE_OBJECTS
		Name = glGenLists(1);

		glNewList(Name, GL_COMPILE);
			run();
		glEndList();
#endif//KUEKEN_STATE_OBJECTS	
	}

	object::~object()
	{
#		if KUEKEN_STATE_OBJECTS
			glDeleteLists(Name, 1); 
#		endif//KUEKEN_STATE_OBJECTS
	}

	void object::bind()
	{
#		if KUEKEN_STATE_OBJECTS
			glCallList(Name);
#		else
			run();
#		endif//KUEKEN_STATE_OBJECTS
	}

	void object::bind(std::size_t Unit)
	{
		run(GL_TEXTURE0 + GLenum(Unit));
	}

	void object::run()
	{
		assert(glGetError() == GL_NO_ERROR);

		glTexParameteri(
			Data.Target,
			GL_TEXTURE_MIN_FILTER,
			Data.Min);

		assert(glGetError() == GL_NO_ERROR);

		glTexParameteri(
			Data.Target,
			GL_TEXTURE_MAG_FILTER,
			Data.Mag);

		assert(glGetError() == GL_NO_ERROR);

		glTexParameteri(
			Data.Target,
			GL_TEXTURE_WRAP_S,
			Data.WrapS);

		assert(glGetError() == GL_NO_ERROR);

		glTexParameteri(
			Data.Target,
			GL_TEXTURE_WRAP_T,
			Data.WrapT);

		assert(glGetError() == GL_NO_ERROR);

		glTexParameteri(
			Data.Target,
			GL_TEXTURE_WRAP_R,
			Data.WrapQ);

		assert(glGetError() == GL_NO_ERROR);

		glTexParameterf(
			Data.Target,
			GL_TEXTURE_MAX_ANISOTROPY_EXT,
			Data.Anisotropy);

		assert(glGetError() == GL_NO_ERROR);

        glTexParameteriv(
			Data.Target,
			GL_TEXTURE_SWIZZLE_RGBA_EXT,
			Data.Swizzle);

		assert(glGetError() == GL_NO_ERROR);

        glTexParameterf(
			Data.Target,
			GL_TEXTURE_MIN_LOD,
			Data.LodMin);

		assert(glGetError() == GL_NO_ERROR);

        glTexParameterf(
			Data.Target,
			GL_TEXTURE_MAX_LOD,
			Data.LodMax);

		assert(glGetError() == GL_NO_ERROR);

        glTexParameterf(
			Data.Target,
			GL_TEXTURE_LOD_BIAS,
			Data.LodBias);

		assert(glGetError() == GL_NO_ERROR);
	}

	void object::run(GLenum Unit)
	{
		glMultiTexParameteriEXT(
			Unit, 
			Data.Target, 
			GL_TEXTURE_MIN_FILTER, 
			Data.Min);

		glMultiTexParameteriEXT(
			Unit, 
			Data.Target, 
			GL_TEXTURE_MAG_FILTER, 
			Data.Mag);

		glMultiTexParameteriEXT(
			Unit, 
			Data.Target, 
			GL_TEXTURE_WRAP_S, 
			Data.WrapS);

		glMultiTexParameteriEXT(
			Unit, 
			Data.Target, 
			GL_TEXTURE_WRAP_T, 
			Data.WrapT);

		glMultiTexParameteriEXT(
			Unit, 
			Data.Target, 
			GL_TEXTURE_WRAP_R, 
			Data.WrapQ);

		glMultiTexParameterfEXT(
			Unit, 
			Data.Target, 
			GL_TEXTURE_MAX_ANISOTROPY_EXT, 
			Data.Anisotropy);

        glMultiTexParameterivEXT(
			Unit, 
			Data.Target, 
			GL_TEXTURE_SWIZZLE_RGBA_EXT, 
			Data.Swizzle);

        glMultiTexParameterfEXT(
			Unit, 
			Data.Target, 
			GL_TEXTURE_MIN_LOD, 
			Data.LodMin);

        glMultiTexParameterfEXT(
			Unit, 
			Data.Target, 
			GL_TEXTURE_MAX_LOD, 
			Data.LodMax);

        glMultiTexParameterfEXT(
			Unit, 
			Data.Target, 
			GL_TEXTURE_LOD_BIAS, 
			Data.LodBias);
	}

}//namespace sampler
}//namespace kueken
