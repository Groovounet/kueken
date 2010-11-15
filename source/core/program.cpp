#include <core/program.hpp>
#include <core/renderer.hpp>

namespace
{
	GLenum program_buffer_mode_cast(kueken::buffer::mode Mode)
	{
		static GLenum const Cast[] = 
		{
			GL_INTERLEAVED_ATTRIBS,		//INTERLEAVED,
			GL_SEPARATE_ATTRIBS			//SEPARATED,
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::buffer::MODE_MAX,
			"Cast array size mismatch");

		return Cast[Mode];
	}

	bool checkProgram(GLuint ProgramName)
	{
		if(!ProgramName)
			return false;

		GLint Result = GL_FALSE;
		glGetProgramiv(ProgramName, GL_LINK_STATUS, &Result);

		fprintf(stdout, "Linking program\n");
		int InfoLogLength;
		glGetProgramiv(ProgramName, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> Buffer(InfoLogLength);
		glGetProgramInfoLog(ProgramName, InfoLogLength, NULL, &Buffer[0]);
		fprintf(stdout, "%s\n", &Buffer[0]);

		return Result == GL_TRUE;
	}

	bool checkShader(GLuint ShaderName, const char* Source)
	{
		if(!ShaderName)
			return false;

		GLint Result = GL_FALSE;
		glGetShaderiv(ShaderName, GL_COMPILE_STATUS, &Result);

		fprintf(stdout, "Compiling shader\n%s...\n", Source);
		int InfoLogLength;
		glGetShaderiv(ShaderName, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> Buffer(InfoLogLength);
		glGetShaderInfoLog(ShaderName, InfoLogLength, NULL, &Buffer[0]);
		fprintf(stdout, "%s\n", &Buffer[0]);

		return Result == GL_TRUE;
	}

}//namespace

namespace kueken{
namespace program{
namespace detail
{
	GLenum program_target_cast(kueken::program::target const & Target)
	{
		static GLenum const Cast[] = 
		{
			GL_NONE,					//UNIFIED
			GL_VERTEX_SHADER,			//VERTEX,
			GL_TESS_CONTROL_SHADER,		//CONTROL,
			GL_TESS_EVALUATION_SHADER,	//EVALUATION,
			GL_GEOMETRY_SHADER,			//PRIMITIVE,
			GL_FRAGMENT_SHADER			//FRAGMENT
		};

		static_assert(
			sizeof(Cast) / sizeof(GLenum) == kueken::program::TARGET_MAX,
			"Cast array size mismatch");

		return Cast[Target];
	}

	std::string loadFile(std::string const & Filename)
	{
		std::ifstream Stream(Filename.c_str(), std::ios::in);

		if(!Stream.is_open())
			return "";

		std::string Line = "";
		std::string Text = "";

		while(getline(Stream, Line))
			Text += "\n" + Line;

		Stream.close();

		return Text;
	}

}//namespace detail

	creator::creator(renderer & Renderer) :
		Renderer(Renderer),
		UniformSemanticsMax(0),
		BlockSemanticsMax(0),
		FeedbackBufferMode(0),
		Quiet(false),
		Built(false)
	{
		memset(&FunctionSemanticsMax[0], 0, sizeof(semantic) * FunctionSemanticsMax.size());
		memset(&RoutineSemanticsMax[0], 0, sizeof(semantic) * RoutineSemanticsMax.size());
	}	

	void creator::setVersion(version const & Version)
	{
		assert(this->Renderer.getVersion() >= Version);

		this->update();

		switch(Version)
		{
		default:
		case GLES_200: 
			break;
		case CORE_330: 
			this->Version = std::string("#version 330 core");
			break;
		case CORE_400: 
			this->Version = std::string("#version 400 core");
			break;
		case CORE_410: 
			this->Version = std::string("#version 410 core");
			break;
		}
	}

	void creator::setExtensions(glm::uint const & ExtensionFlags)
	{
		this->update();

		if(ExtensionFlags & DOUBLE)
		{
			Options += std::string("#extension GL_ARB_gpu_shader_fp64 : required\n");
			Options += std::string("#extension GL_ARB_vertex_attrib_64bit : required\n");
		}
		if(ExtensionFlags & VIEWPORT)
		{
			Options += std::string("#extension GL_ARB_viewport_array : required\n");
		}
		if(ExtensionFlags & PRECISION)
		{
			Options += std::string("#extension GL_ARB_shader_precision : required\n");
		}
		if(ExtensionFlags & STENCIL_EXPORT)
		{
			Options += std::string("#extension GL_ARB_shader_stencil_export : required\n");
		}
	}

	void creator::setOptions(glm::uint const & OptionFlags)
	{
		this->update();

		this->Quiet = OptionFlags & QUIET;

		if(OptionFlags & DEBUGING)
			Options += std::string("#pragma debug(on)\n");
		else 
			Options += std::string("#pragma debug(off)\n");

		if(OptionFlags & OPTIMIZE)
			Options += std::string("#pragma optimize(on)\n");
		else 
			Options += std::string("#pragma optimize(off)\n");
	}

	void creator::addDefinition(std::string const & Name)
	{
		this->update();
		this->Definitions += std::string("#define ") + Name + std::string("\n");
	}

	void creator::addDefinition(std::string const & Name, std::string const & Value)
	{
		this->update();
		this->Definitions += std::string("#define ") + Name + std::string(" ") + Value + std::string("\n");
	}

	void creator::addSemantic
	(
		semantic const & Location,
		std::string const & Value
	)
	{
		this->update();
		this->Semantics += (boost::format("#define %s %d\n") % Value.c_str() % Location).str();
	}

	void creator::addVariable
	(
		semantic const & Semantic, 
		std::string const & Name
	)
	{
		this->update();
		this->UniformVariables.push_back(
			detail::indirection(Semantic, Name));
		this->UniformSemanticsMax = glm::max(this->UniformSemanticsMax, Semantic);
	}

	void creator::addBlock
	(
		semantic const & Semantic, 
		std::string const & Name
	)
	{
		this->update();
		this->BlockVariables.push_back(
			detail::indirection(Semantic, Name));
		this->BlockSemanticsMax = glm::max(this->BlockSemanticsMax, Semantic);
	}
	
	void creator::addFunction
	(
		target const & Target,
		function const & Function, 
		std::string const & Name
	)
	{
		this->update();
		this->FunctionVariables[Target].push_back(
			detail::indirection(Function, Name));
		this->FunctionSemanticsMax[Target] = glm::max(this->FunctionSemanticsMax[Target], Function);
	}

	void creator::addSubroutine
	(
		target const & Target,
		subroutine const & Subroutine, 
		std::string const & Name
	)
	{
		this->update();
		this->RoutineVariables[Target].push_back(
			detail::indirection(Subroutine, Name));
		this->RoutineSemanticsMax[Target] = glm::max(this->RoutineSemanticsMax[Target], Subroutine);
	}

	void creator::addSource
	(
		target const & Target, 
		input const & Input,
		std::string const & Source
	)
	{
		this->update();

		switch(Input)
		{
			case DATA:
			{
				this->Sources[Target] += Source;
			}
			break;
			case FILE:
			{
				std::string Data = detail::loadFile(Source);
				assert(!Data.empty());
				this->Sources[Target] += Data;
			}
			break;
		}
	}

	void creator::setFeedbackVariable
	(
		std::vector<std::string> const & Names, 
		buffer::mode const & Mode
	)
	{
		this->update();

		FeedbackVariables = Names;
		FeedbackBufferMode = program_buffer_mode_cast(Mode);
	}

	void creator::clear(flag const & Flag)
	{
		if(Flag & DEFINITIONS)
		{
			this->Definitions.clear();
		}
		if(Flag & SOURCES)
		{
			for(std::size_t i = 0; i < TARGET_MAX; ++i)
				this->Sources[i].clear();	
		}
	}

	void creator::update()
	{
		this->Built = false;
	}

	bool creator::build()
	{
		if(this->Built)
			return true;

		bool HaveSource = false;
		for(std::size_t i = 0; i < TARGET_MAX; ++i)
		{
			if(Sources[i].empty())
				continue;
			HaveSource = true;
			break;
		}
		if(!HaveSource)
			return this->Built = false;

		for(std::size_t i = 0; i < TARGET_MAX; ++i)
		{
			if(Sources[i].empty())
				continue;

			// If a file have a #version in it, it's not a file to edit.
			if(Sources[i].find("#version") != std::string::npos)
			{
				SourcesBuilt[i] = Sources[i];
				continue;
			}

			SourcesBuilt[i].clear();
			SourcesBuilt[i] += Version;
			SourcesBuilt[i] += std::string("\n");

			if(!Extensions.empty())
			{
				SourcesBuilt[i] += Extensions;
				SourcesBuilt[i] += std::string("\n");
			}

			if(!Options.empty())
			{
				SourcesBuilt[i] += Options;
				SourcesBuilt[i] += std::string("\n");
			}

			if(!Definitions.empty())
			{
				SourcesBuilt[i] += Definitions;
				SourcesBuilt[i] += std::string("\n");
			}

			if(!Semantics.empty())
			{
				SourcesBuilt[i] += Semantics;
				SourcesBuilt[i] += std::string("\n");
			}

			SourcesBuilt[i] += Sources[i];
		}

		for(std::size_t i = 0; i < TARGET_MAX; ++i)
		{
			if(SourcesBuilt[i].find(std::string("main")) == std::string::npos)
				return this->Built = false;
		}

		return this->Built = true;
	}

	object::object
	(
		creator const & Creator
	) :
		Name(0)
	{
		bool Success = true;
		
		// Compile a shader
		if(Success)
		{
			std::array<GLuint, TARGET_MAX> Shader = {0, 0, 0, 0, 0, 0};
			std::array<std::string, TARGET_MAX> Source;

			this->Name = glCreateProgram();
			for(std::size_t Stage = 0; Stage < Shader.size(); ++Stage)
			{
				if(Creator.SourcesBuilt[Stage].empty())
					continue;
				Source[Stage] = Creator.SourcesBuilt[Stage];
				char const * SourcePtr = Source[Stage].c_str();
				Shader[Stage] = glCreateShader(detail::program_target_cast(kueken::program::target(Stage)));
				glShaderSource(Shader[Stage], 1, &SourcePtr, NULL);
				glCompileShader(Shader[Stage]);
			}

			for(std::size_t Stage = 0; Stage < Shader.size(); ++Stage)
			{
				if(Shader[Stage] == 0)
					continue;
				Success = Success && checkShader(Shader[Stage], Source[Stage].c_str());
				assert(Success);
			}

			for(std::size_t Stage = 0; Stage < Shader.size(); ++Stage)
			{
				if(Shader[Stage] == 0)
					continue;
				glAttachShader(this->Name, Shader[Stage]);
				glDeleteShader(Shader[Stage]);
			}

			if(!Creator.FeedbackVariables.empty())
			{
				std::size_t VariablesSize = Creator.FeedbackVariables.size();
				char const ** VariablesData = new char const* [VariablesSize];

				for(std::size_t i = 0; i < Creator.FeedbackVariables.size(); ++i)
					VariablesData[i] = (char*)Creator.FeedbackVariables[i].c_str();

				glTransformFeedbackVaryings(
					this->Name, 
					GLsizei(Creator.FeedbackVariables.size()),
					VariablesData, 
					Creator.FeedbackBufferMode);

				delete[] VariablesData;
			}

			glLinkProgram(this->Name);

			Success = checkProgram(this->Name);
			assert(Success);
		}

		// Load variables
		if(Success)
		{
			this->UniformIndirection.resize(Creator.UniformSemanticsMax + 1);
			for(std::size_t i = 0; i < Creator.UniformVariables.size(); ++i)
			{
				std::string const & VariableName = Creator.UniformVariables[i].Name;
				GLuint Location = glGetUniformLocation(this->Name, VariableName.c_str());
				assert(Location != GLuint(-1));
				this->UniformIndirection[Creator.UniformVariables[i].Semantic] = Location;
			}

			this->BlockIndirection.resize(Creator.BlockSemanticsMax + 1);
			for(std::size_t i = 0; i < Creator.BlockVariables.size(); ++i)
			{
				GLuint Location = glGetUniformBlockIndex(this->Name, Creator.BlockVariables[i].Name.c_str());
				assert(Location != GLuint(-1));
				this->BlockIndirection[Creator.BlockVariables[i].Semantic] = Location;
			}

			for(std::size_t TargetIndex = 0; TargetIndex < TARGET_MAX; ++TargetIndex)
			{
				if(Creator.FunctionSemanticsMax[TargetIndex] == 0)
					continue;

				this->FunctionIndirection[TargetIndex].resize(Creator.FunctionSemanticsMax[TargetIndex] + 1);

				std::vector<detail::indirection> const & FunctionIndirections = Creator.FunctionVariables[TargetIndex];
				for(std::vector<detail::indirection>::size_type i = 0; i < FunctionIndirections.size(); ++i)
				{
					semantic Semantic = FunctionIndirections[i].Semantic;
					GLuint Index = glGetSubroutineIndex(
						this->Name, 
						detail::program_target_cast(program::target(TargetIndex)), 
						FunctionIndirections[i].Name.c_str());
					assert(Index != GLuint(-1));
					this->FunctionIndirection[TargetIndex][Semantic] = Index;
				}

				this->RoutineIndirection[TargetIndex].resize(Creator.RoutineSemanticsMax[TargetIndex] + 1, 0);

				std::vector<detail::indirection> const & RoutineIndirections = Creator.RoutineVariables[TargetIndex];
				for(std::vector<detail::indirection>::size_type i = 0; i < RoutineIndirections.size(); ++i)
				{
					semantic Semantic = RoutineIndirections[i].Semantic;
					GLuint Location = glGetSubroutineUniformLocation(
						this->Name, 
						detail::program_target_cast(program::target(TargetIndex)), 
						RoutineIndirections[i].Name.c_str());
					assert(Location != GLuint(-1));
					this->RoutineIndirection[TargetIndex][Semantic] = Location;
				}

				this->Subroutines[TargetIndex].resize(this->RoutineIndirection[TargetIndex].size());
			}
		}

		assert(Success);
	}

	object::~object()
	{
		if(Name)
			glDeleteProgram(Name);
	}

	void object::bind()
	{
		glUseProgram(Name);
		for(std::size_t Target = 0; Target < TARGET_MAX; ++Target)
		{
			if(this->Subroutines[Target].empty())
				continue;
			glUniformSubroutinesuiv(
				detail::program_target_cast(kueken::program::target(Target)), 
				this->Subroutines[Target].size(), 
				&this->Subroutines[Target][0]);
		}
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		float const & Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform1fvEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			1, 
			&Value);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		count const & Count,
		float const * Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform1fvEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			Count, 
			Value);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		int const & Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform1iEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			Value);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		count const & Count,
		int const * Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform1ivEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			GLsizei(Count), 
			Value);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		glm::uint const & Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform1uiEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			Value);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		count const & Count,
		glm::uint const * Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform1uivEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			GLsizei(Count), 
			Value);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		glm::vec2 const & Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform2fvEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			1, 
			glm::value_ptr(Value));
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		count const & Count,
		glm::vec2 const * Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform2fvEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			Count, 
			&Value[0][0]);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		glm::ivec2 const & Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform2ivEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			1, 
			glm::value_ptr(Value));
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		count const & Count,
		glm::ivec2 const * Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform2ivEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			Count, 
			&Value[0][0]);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		glm::uvec2 const & Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform2uivEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			1, 
			glm::value_ptr(Value));
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		count const & Count,
		glm::uvec2 const * Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform2uivEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			Count, 
			&Value[0][0]);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		glm::vec3 const & Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform3fvEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			1, 
			glm::value_ptr(Value));
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		count const & Count,
		glm::vec3 const * Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform3fvEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			Count, 
			&Value[0][0]);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		glm::ivec3 const & Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform3ivEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			1, 
			glm::value_ptr(Value));
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		count const & Count,
		glm::ivec3 const * Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform3ivEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			Count, 
			&Value[0][0]);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		glm::uvec3 const & Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform3uivEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			1, 
			glm::value_ptr(Value));
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		count const & Count,
		glm::uvec3 const * Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform3uivEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			Count, 
			&Value[0][0]);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		glm::vec4 const & Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform4fvEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			1, 
			glm::value_ptr(Value));
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		count const & Count,
		glm::vec4 const * Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform4fvEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			Count, 
			&Value[0][0]);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		glm::ivec4 const & Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform4ivEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			1, 
			glm::value_ptr(Value));
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		count const & Count,
		glm::ivec4 const * Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform4ivEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			Count, 
			&Value[0][0]);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		glm::uvec4 const & Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform4uivEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			1, 
			glm::value_ptr(Value));
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		count const & Count,
		glm::uvec4 const * Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform4uivEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			Count, 
			&Value[0][0]);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		glm::mat2 const & Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniformMatrix2fvEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			1, GL_FALSE, 
			glm::value_ptr(Value));
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		count const & Count, 
		glm::mat2 const * Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniformMatrix2fvEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			Count, GL_FALSE, 
			&Value[0][0][0]);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		glm::mat3 const & Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniformMatrix3fv(
			this->Name, 
			this->UniformIndirection[Semantic], 
			1, GL_FALSE, 
			glm::value_ptr(Value));
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		count const & Count, 
		glm::mat3 const * Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniformMatrix3fvEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			Count, GL_FALSE, 
			&Value[0][0][0]);
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		glm::mat4 const & Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniformMatrix4fvEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			1, 
			GL_FALSE, 
			glm::value_ptr(Value));
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		count const & Count, 
		glm::mat4 const * Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniformMatrix4fvEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			Count, GL_FALSE, 
			&Value[0][0][0]);
	}

	void object::setSampler
	(
		semantic const & Semantic, 
		sampler::slot const & Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		GLuint Location = this->UniformIndirection[Semantic];
		glProgramUniform1iEXT(this->Name, Location, Value);
	}

	void object::setSampler
	(
		semantic const & Semantic, 
		count const & Count,
		sampler::slot const * Value
	)
	{
		assert(Semantic <= this->UniformIndirection.size());

		glProgramUniform1ivEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			GLsizei(Count), 
			(GLint*)Value);
	}

	void object::setBlock
	(
		semantic const & Semantic, 
		block const & Value
	)
	{
		assert(Value >= buffer::UNIFORM && Value <= buffer::UNIFORM_MAX);

		glUniformBlockBinding(
			this->Name, 
			this->BlockIndirection[Semantic], 
			Value - buffer::UNIFORM);
	}

	void object::setBlock
	(
		semantic const & Semantic, 
		count const & Count,
		block const * Value
	)
	{
		assert(Value[0] >= buffer::UNIFORM && Value[0] + Count <= buffer::UNIFORM_MAX);

		for(count Index = 0; Index < Count; ++Index)
		{
			glUniformBlockBinding(
				this->Name, 
				this->BlockIndirection[Semantic] + Index, 
				GLuint(Value - buffer::UNIFORM + Index));
		}
	}

	void object::setSubroutine
	(
		target const & Target,
		subroutine const & Subroutine,
		function const & Function
	)
	{
		assert(Function <= this->FunctionIndirection[Target].size());
		assert(Subroutine <= this->RoutineIndirection[Target].size());

		GLuint Index = this->FunctionIndirection[Target][Function];
		GLuint Location = this->RoutineIndirection[Target][Subroutine];
		this->Subroutines[Target][Location] = Index;

		//glUniformSubroutinesuiv(
		//	detail::program_target_cast(Target), 
		//	this->Subroutines[Target].size(), 
		//	&this->Subroutines[Target][0]);
	}

	void object::setSubroutine
	(
		target const & Target,
		count const & Count,
		function const * Functions
	)
	{
		assert(Count <= this->RoutineIndirection[Target].size());

		for(count i = 0; i < Count; ++i)
		{
			GLuint Index = this->FunctionIndirection[Target][*(Functions + i)];
			GLuint Location = this->RoutineIndirection[Target][i];
			this->Subroutines[Target][i] = Index;
		}

		//glUniformSubroutinesuiv(
		//	detail::program_target_cast(Target), 
		//	this->Subroutines[Target].size(), 
		//	&this->Subroutines[Target][0]);
	}

}//namespace program
}//namespace kueken

