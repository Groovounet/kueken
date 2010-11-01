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
		memset(&SubroutineSemanticsMax[0], 0, sizeof(semantic) * SubroutineSemanticsMax.size());
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
	
	void creator::addSubroutine
	(
		target const & Target,
		semantic const & Semantic, 
		std::string const & Name
	)
	{
		this->update();
		this->SubroutineVariables[Target].push_back(
			detail::indirection(Semantic, Name));
		this->SubroutineSemanticsMax[Target] = glm::max(this->SubroutineSemanticsMax[Target], Semantic);
	}

	void creator::addSubroutineLocation
	(
		target const & Target,
		semantic const & Semantic, 
		std::string const & Name
	)
	{
		this->update();
		this->SubroutineLocationVariables[Target].push_back(
			detail::indirection(Semantic, Name));
		this->SubroutineLocationSemanticsMax[Target] = glm::max(this->SubroutineLocationSemanticsMax[Target], Semantic);
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
				GLuint Location = glGetUniformLocation(this->Name, Creator.UniformVariables[i].Name.c_str());
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
				if(Creator.SubroutineSemanticsMax[TargetIndex] == 0)
					continue;

				this->SubroutineIndirection[TargetIndex].resize(Creator.SubroutineSemanticsMax[TargetIndex] + 1);
				for(std::size_t i = 0; i < Creator.SubroutineVariables[TargetIndex].size(); ++i)
				{
					GLuint Location = glGetSubroutineUniformLocation(this->Name, TargetIndex, Creator.SubroutineVariables[TargetIndex][i].Name.c_str());
					assert(Location != GLuint(-1));
					this->SubroutineIndirection[TargetIndex][Creator.SubroutineVariables[TargetIndex][i].Semantic] = Location;
				}

				this->SubroutineLocation[TargetIndex].resize(Creator.SubroutineSemanticsMax[TargetIndex] + 1, 0);
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
			if(this->SubroutineLocationIndirection[Target].empty())
				continue;
			glUniformSubroutinesuiv(
				detail::program_target_cast(kueken::program::target(Target)), 
				this->SubroutineLocationIndirection[Target].size(), 
				&this->SubroutineLocationIndirection[Target][0]);
		}
	}

	template <>
	void object::setUniform
	(
		semantic const & Semantic, 
		float const & Value
	)
	{
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
		glProgramUniform1iEXT(
			this->Name, 
			this->UniformIndirection[Semantic], 
			Value);
	}

	void object::setSampler
	(
		semantic const & Semantic, 
		count const & Count,
		sampler::slot const * Value
	)
	{
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
		semantic const & Semantic, 
		subroutine const & Value
	)
	{
		assert(Semantic <= SubroutineIndirection[Target].size());
		assert(Semantic <= SubroutineLocationIndirection[Target].size());

		GLuint Location = SubroutineLocationIndirection[Target][Semantic];
		SubroutineIndirection[Target][Location] = Value;

		glUniformSubroutinesuiv(
			detail::program_target_cast(Target), 
			SubroutineIndirection[Target].size(), 
			&SubroutineIndirection[Target][0]);
	}

	void object::setSubroutine
	(
		target const & Target,
		semantic const & Semantic, 
		count const & Count,
		subroutine const * Value
	)
	{
		assert(Semantic + Count <= SubroutineIndirection[Target].size());

		for(count i = 0; i < Count; ++i)
			SubroutineIndirection[Target][Semantic + i] = Value[i];

		glUniformSubroutinesuiv(
			detail::program_target_cast(Target), 
			SubroutineIndirection[Target].size(), 
			&SubroutineIndirection[Target][0]);
	}

}//namespace program
}//namespace kueken

