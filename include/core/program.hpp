#ifndef KUEKEN_PROGRAM_INCLUDED
#define KUEKEN_PROGRAM_INCLUDED

#include "detail/detail.hpp"
#include "buffer.hpp"
#include "sampler.hpp"

//TODO
// - program uniform type matching with user setting
// - program sampler type matching with texture target

namespace kueken
{
	class renderer;
}//namespace kueken

namespace kueken{
namespace program
{
	enum target
	{
		UNIFIED,
		VERTEX,
		CONTROL,
		EVALUATION,
		PRIMITIVE,
        FRAGMENT,
		TARGET_MAX
	};

	enum version
	{
		DEFAULT,
		GLES_200,
		CORE_330,
		CORE_400,
		CORE_410
	};

	enum extension
	{
		DOUBLE			= (1 << 0),
		VIEWPORT		= (1 << 1),
		PRECISION		= (1 << 2),
		STENCIL_EXPORT	= (1 << 3)
	};

	enum option
	{
		QUIET 			= (1 << 0),
		DEBUGING 		= (1 << 1),
		OPTIMIZE		= (1 << 2)
	};

	enum input
	{
		DATA,
		FILE
	};

	enum clear
	{
		DEFINITIONS	= (1 << 0),
		SOURCES		= (1 << 1),
		ALL			= DEFINITIONS | SOURCES
	};

	typedef std::size_t slot;
	typedef std::size_t semantic;
	typedef std::size_t count;
	typedef int sampler;
	typedef buffer::target block;
	typedef glm::uint32 subroutine;
	typedef glm::uint32 function;

	enum format
	{
		F16VEC1,
		F16VEC2,
		F16VEC3,
		F16VEC4,
		F32VEC1,
		F32VEC2,
		F32VEC3,
		F32VEC4,
		F64VEC1,
		F64VEC2,
		F64VEC3,
		F64VEC4,
		I8VEC1,
		I8VEC2,
		I8VEC3,
		I8VEC4,
		I16VEC1,
		I16VEC2,
		I16VEC3,
		I16VEC4,
		I32VEC1,
		I32VEC2,
		I32VEC3,
		I32VEC4,
		U8VEC1,
		U8VEC2,
		U8VEC3,
		U8VEC4,
		U16VEC1,
		U16VEC2,
		U16VEC3,
		U16VEC4,
		U32VEC1,
		U32VEC2,
		U32VEC3,
		U32VEC4,
		BVEC1,
		BVEC2,
		BVEC3,
		BVEC4,
		F32MAT2X2,
		F32MAT2X3,
		F32MAT2X4,
		F32MAT3X2,
		F32MAT3X3,
		F32MAT3X4,
		F32MAT4X2,
		F32MAT4X3,
		F32MAT4X4,
		F64MAT2X2,
		F64MAT2X3,
		F64MAT2X4,
		F64MAT3X2,
		F64MAT3X3,
		F64MAT3X4,
		F64MAT4X2,
		F64MAT4X3,
		F64MAT4X4,
		FORMAT_MAX
	};

	enum samplerType
	{
		SAMPLER1D,
		SAMPLER1D_ARRAY,
		SAMPLER1D_SHADOW,
		SAMPLER1D_ARRAY_SHADOW,
		SAMPLER2D,
		SAMPLER2D_ARRAY,
		SAMPLER2D_SHADOW,
		SAMPLER2D_ARRAY_SHADOW,
		SAMPLER2D_RECT,
		SAMPLER2D_RECT_SHADOW,
		SAMPLER2D_MS,
		SAMPLER2D_MS_ARRAY,
		SAMPLER3D,
		SAMPLER_CUBE,
		SAMPLER_CUBE_ARRAY,
		SAMPLER_CUBE_SHADOW,
		SAMPLER_CUBE_ARRAY_SHADOW,
		SAMPLER_BUFFER,

		ISAMPLER1D,
		ISAMPLER1D_ARRAY,
		ISAMPLER2D,
		ISAMPLER2D_ARRAY,
		ISAMPLER2D_RECT,
		ISAMPLER2D_MS,
		ISAMPLER2D_MS_ARRAY,
		ISAMPLER3D,
		ISAMPLER_CUBE,
		ISAMPLER_CUBE_ARRAY,
		ISAMPLER_BUFFER,

		USAMPLER1D,
		USAMPLER1D_ARRAY,
		USAMPLER2D,
		USAMPLER2D_ARRAY,
		USAMPLER2D_RECT,
		USAMPLER2D_MS,
		USAMPLER2D_MS_ARRAY,
		USAMPLER3D,
		USAMPLER_CUBE,
		USAMPLER_CUBE_ARRAY,
		USAMPLER_BUFFER,

		SAMPLER_MAX
	};

namespace detail
{
	GLenum program_target_cast(kueken::program::target const & Target);

	struct indirection
	{
		inline indirection
		(
			semantic const & Semantic,
			std::string const & Name
		) :
			Semantic(Semantic),
			Name(Name)
		{}
		semantic Semantic;
		std::string Name;
	};

}//namespace detail

	class creator// : public kueken::detail::creator
	{
		friend class object;

	public:
		creator(renderer & Renderer);
		void setVersion(
			version const & Version);
		void setExtensions(
			glm::uint const & ExtensionFlags);
		void setOptions(
			glm::uint const & ExtensionFlags);
		void addDefinition(
			std::string const & Name);
		void addDefinition(
			std::string const & Name, 
			std::string const & Value);
		void addSemantic(
			semantic const & Location,
			std::string const & Value);
		void addVariable(
			semantic const & Semantic, 
			std::string const & Name);
		void addBlock(
			semantic const & Semantic, 
			std::string const & Name);
		void addFunction(
			target const & Target,
			function const & Function, 
			std::string const & Name);
		void addSubroutine(
			target const & Target,
			subroutine const & Subroutine, 
			std::string const & Name);
		void addSource(
			target const & Target, 
			input const & Input, 
			std::string const & Source);
		void setFeedbackVariable(
			std::vector<std::string> const & Names, 
			buffer::mode const & Mode);
		void clear(flag const & Flag);
		bool build();

	private:
		void update();

		std::string Version;
		std::string Extensions;
		std::string Options;
		std::string Definitions;
		std::string Semantics;
		std::string Sources[TARGET_MAX];
		std::string SourcesBuilt[TARGET_MAX];

		std::vector<std::string> FeedbackVariables;
		std::vector<detail::indirection> UniformVariables;
		semantic UniformSemanticsMax;
		std::vector<detail::indirection> BlockVariables;
		semantic BlockSemanticsMax;
		std::array<std::vector<detail::indirection>, TARGET_MAX> FunctionVariables;
		std::array<semantic, TARGET_MAX> FunctionSemanticsMax;
		std::array<std::vector<detail::indirection>, TARGET_MAX> RoutineVariables;
		std::array<semantic, TARGET_MAX> RoutineSemanticsMax;
		renderer & Renderer;
		GLenum FeedbackBufferMode;
		bool Quiet;
		bool Built;
	};

	enum type
	{
		NONE,
		SAMPLER,
		UNIFORM,
		ATTRIB
	};

	class object
	{
	public:
		object(creator const & Creator);
		~object();

		void bind();

		// Uniform
		template <typename genType>
		void setUniform(
			semantic const & Semantic, 
			genType const & Value);
		
		// Uniform array
		template <typename genType>
		void setUniform(
			semantic const & Semantic, 
			count const & Count, 
			genType const * Value);
		
		// Sampler
		void setSampler(
			semantic const & Semantic, 
			sampler::slot const & Value);

		// Sampler array
		void setSampler(
			semantic const & Semantic, 
			count const & Count, 
			sampler::slot const * Value);

		// Block
		void setBlock(
			semantic const & Semantic, 
			block const & Value);

		// Block array
		void setBlock(
			semantic const & Semantic, 
			count const & Count, 
			block const * Value);

		// Subroutine
		void setSubroutine(
			target const & Target,
			subroutine const & Subroutine, 
			function const & Function);

		// Subroutine array
		void setSubroutine(
			target const & Target,
			count const & Count, 
			function const * Value);

	private:
		//detail::data Data;
		std::vector<GLuint> UniformIndirection;
		std::vector<GLuint> BlockIndirection;
		std::array<std::vector<GLuint>, TARGET_MAX> FunctionIndirection;
		std::array<std::vector<subroutine>, TARGET_MAX> RoutineIndirection;
		std::array<std::vector<GLuint>, TARGET_MAX> Subroutines;
		GLuint Name;
		std::string Log;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace program
}//namespace kueken

#include "program.inl"

#endif//KUEKEN_PROGRAM_INCLUDED
