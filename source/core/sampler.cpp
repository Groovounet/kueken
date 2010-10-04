#include <core/sampler.hpp>
#include <core/renderer.hpp>

namespace
{
	//GLenum sampler_target_cast(kueken::sampler::target Target)
	//{
	//	static GLenum const CastSamplerTarget[kueken::sampler::TARGET_MAX] =
	//	{
	//		GL_TEXTURE_1D,						// IMAGE_1D
	//		GL_TEXTURE_2D,						// IMAGE_2D
	//		GL_TEXTURE_3D,						// IMAGE_3D
	//		GL_TEXTURE_1D_ARRAY_EXT,			// ARRAY_1D
	//		GL_TEXTURE_2D_ARRAY_EXT,			// ARRAY_2D
	//		GL_TEXTURE_RECTANGLE_ARB,			// RECT
	//		GL_TEXTURE_CUBE_MAP_POSITIVE_X,		// CUBE_POS_X
	//		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,		// CUBE_NEG_X
	//		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,		// CUBE_POS_Y
	//		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,		// CUBE_NEG_Y
	//		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,		// CUBE_POS_Z
	//		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z		// CUBE_NEG_Z
	//	};

	//	return CastSamplerTarget[Target];
	//}

	GLenum sampler_min_cast(kueken::sampler::filter Filter)
	{
		static GLenum const Cast[kueken::sampler::FILTER_MAX] = 
		{
			GL_NEAREST,						// NEAREST
			GL_LINEAR,						// BILINEAR
			GL_LINEAR_MIPMAP_LINEAR,		// TRILINEAR
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::sampler::FILTER_MAX,
			"Cast array size mismatch");

		return Cast[Filter];
	}

	GLenum sampler_mag_cast(kueken::sampler::filter Filter)
	{
		static GLenum const Cast[kueken::sampler::FILTER_MAX] = 
		{
			GL_NEAREST,						// NEAREST
			GL_LINEAR,						// BILINEAR
			GL_LINEAR,						// TRILINEAR
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::sampler::FILTER_MAX,
			"Cast array size mismatch");

		return Cast[Filter];
	}

	GLenum sampler_wrap_cast(kueken::sampler::wrap Wrap)
	{
		static GLenum const Cast[kueken::sampler::WRAP_MAX] = 
		{
			GL_CLAMP_TO_BORDER,				// CLAMP_TO_BORDER
			GL_CLAMP_TO_EDGE,				// CLAMP_TO_EDGE
			GL_MIRRORED_REPEAT,				// MIRRORED_REPEAT
			GL_REPEAT,						// REPEAT
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::sampler::WRAP_MAX,
			"Cast array size mismatch");

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

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::sampler::SWIZZLE_MAX,
			"Cast array size mismatch");

		return Cast[Swizzle];
	}

	GLenum sampler_compare_cast(kueken::sampler::compare Compare)
	{
		static GLenum const Cast[kueken::sampler::COMPARE_MAX] = 
		{
			GL_NONE,	//NONE,
			GL_LEQUAL,	//LEQUAL,
			GL_GEQUAL,	//GEQUAL,
			GL_LESS,	//LESS,
			GL_GREATER,	//GREATER,
			GL_EQUAL,	//EQUAL
			GL_NOTEQUAL,//NOTEQUAL
			GL_ALWAYS,	//ALWAYS
			GL_NEVER	//NEVER
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::sampler::COMPARE_MAX,
			"Cast array size mismatch");

		return Cast[Compare];
	}

}//namespace

namespace kueken{
namespace sampler
{
	//////////////////////////////////
	// Creator

	creator::creator(renderer & Renderer) :
		Renderer(Renderer)
	{}

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

	void creator::setLod(float Min, float Max, float Bias)
	{
		Data.LodMin = Min;
		Data.LodMax = Max;
		Data.LodBias = Bias;
	}

	void creator::setBorderColor(glm::vec4 const & Color)
	{
		Data.BorderColor = Color;
	}

	void creator::setCompare(compare const & Compare)
	{
		Data.CompareMode = sampler_compare_cast(Compare) == kueken::sampler::NONE ? GL_NONE : GL_COMPARE_REF_TO_TEXTURE;
		Data.CompareFunc = sampler_compare_cast(Compare);
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
	)
	{
		glGenSamplers(1, &Name);

		glSamplerParameteri(Name, GL_TEXTURE_MIN_FILTER, Creator.Data.Min);
		glSamplerParameteri(Name, GL_TEXTURE_MAG_FILTER, Creator.Data.Mag);
		glSamplerParameterf(Name, GL_TEXTURE_MAX_ANISOTROPY_EXT, Creator.Data.Anisotropy);
		glSamplerParameteri(Name, GL_TEXTURE_WRAP_S, Creator.Data.WrapS);
		glSamplerParameteri(Name, GL_TEXTURE_WRAP_T, Creator.Data.WrapT);
		glSamplerParameteri(Name, GL_TEXTURE_WRAP_R, Creator.Data.WrapQ);
		glSamplerParameterf(Name, GL_TEXTURE_MIN_LOD, Creator.Data.LodMin);
		glSamplerParameterf(Name, GL_TEXTURE_MAX_LOD, Creator.Data.LodMax);
		glSamplerParameterf(Name, GL_TEXTURE_LOD_BIAS, Creator.Data.LodBias);
		glSamplerParameterfv(Name, GL_TEXTURE_BORDER_COLOR, &Creator.Data.BorderColor[0]);
		glSamplerParameteri(Name, GL_TEXTURE_COMPARE_MODE, Creator.Data.CompareMode);
		if(Creator.Data.CompareFunc != GL_NONE)
			glSamplerParameteri(Name, GL_TEXTURE_COMPARE_FUNC, Creator.Data.CompareFunc);
	}

	object::~object()
	{
		glDeleteSamplers(1, &Name);
	}

	void object::bind(std::size_t Unit)
	{
		glBindSampler(Unit, Name);
	}

}//namespace sampler
}//namespace kueken
