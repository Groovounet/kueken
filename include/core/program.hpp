#ifndef KUEKEN_PROGRAM_INCLUDED
#define KUEKEN_PROGRAM_INCLUDED

#include "detail/detail.hpp"
#include "buffer.hpp"

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
	typedef glm::uint32 block;
	typedef glm::uint32 subroutine;

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
		void addSubroutine(
			target const & Target,
			semantic const & Semantic, 
			std::string const & Name);
		void addSubroutineLocation(
			target const & Target,
			semantic const & Semantic, 
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
		std::array<std::vector<detail::indirection>, TARGET_MAX> SubroutineVariables;
		std::array<semantic, TARGET_MAX> SubroutineSemanticsMax;
		std::array<std::vector<detail::indirection>, TARGET_MAX> SubroutineLocationVariables;
		std::array<semantic, TARGET_MAX> SubroutineLocationSemanticsMax;
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

		template <typename genType>
		void setUniform(
			semantic const & Semantic, 
			genType const & Value);
		
		// Sampler
		void setSampler(
			semantic const & Semantic, 
			sampler const & Value);

		// Sampler array
		void setSampler(
			semantic const & Semantic, 
			count const & Count, 
			sampler const * Value);

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
			semantic const & Semantic, 
			subroutine const & Value);

		// Subroutine array
		void setSubroutine(
			target const & Target,
			semantic const & Semantic, 
			count const & Count, 
			subroutine const * Value);

	private:
		//detail::data Data;
		std::vector<GLuint> UniformIndirection;
		std::vector<GLuint> BlockIndirection;
		std::array<std::vector<GLuint>, TARGET_MAX> SubroutineIndirection;
		std::array<std::vector<GLuint>, TARGET_MAX> SubroutineLocationIndirection;
		std::array<std::vector<GLuint>, TARGET_MAX> SubroutineLocation;
		GLuint Name;
		std::string Log;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace program
}//namespace kueken

#include "program.inl"

#endif//KUEKEN_PROGRAM_INCLUDED
