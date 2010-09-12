#ifndef KUEKEN_PROGRAM_INCLUDED
#define KUEKEN_PROGRAM_INCLUDED

#include "detail/detail.hpp"
#include "buffer.hpp"

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

namespace detail
{
	GLenum program_target_cast(kueken::program::target Target);

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
		creator();
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
		void addSource(
			target const & Target, 
			input const & Input, 
			std::string const & Source);
		void addVariable(
			semantic const & Semantic, 
			std::string const & Name);
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
		std::string Sources[TARGET_MAX];
		std::string SourcesBuilt[TARGET_MAX];

		std::vector<std::string> FeedbackVariables;
		std::vector<detail::indirection> UniformVariables;
		semantic SemanticsMax;
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

	class variable
	{
	public: 
		variable(GLuint Program, GLuint Location, type Type);//, std::size_t Size);
		variable();
		//void set(std::size_t Offset, std::size_t Size, void const * Pointer);
		void set(buffer::name const & Value);
		void set(glm::vec4 const & Value);
		void set(glm::mat4 const & Value);
		void set(float * Value, std::size_t Size);
		void set(glm::vec2 const * const Value, std::size_t Size);
		void set(int Value);

	//private:
		GLuint Program;
		GLuint Location;
		type Type;
		//std::vector<glm::byte> Data;
	};

	class object
	{
	public:
		object(creator const & Creator);
		~object();

		void bind();

		void setSampler(std::string const & Name, int const & Value);
		void setUniform(std::string const & Name, int const & Value);
		void setUniform(std::string const & Name, glm::vec4 const & Value);
		void setUniform(std::string const & Name, glm::mat4 const & Value);

		variable get(std::string const & Name, type Type);
		
		template <typename genType>
		void setUniform(
			semantic const & Semantic, 
			genType const & Value);
		void setSampler(
			semantic const & Semantic, 
			sampler const & Value);
		void setSampler(
			semantic const & Semantic, 
			count const & Count, 
			sampler const * Value);

	private:
		//detail::data Data;
		std::vector<GLuint> Indirection;
		GLuint Name;
		std::string Log;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace program
}//namespace kueken

#include "program.inl"

#endif//KUEKEN_PROGRAM_INCLUDED
