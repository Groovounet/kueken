#include <core/program.hpp>
#include <core/manager.hpp>

namespace
{
	GLenum program_buffer_mode_cast(kueken::buffer::mode Mode)
	{
		static GLenum const Cast[kueken::buffer::MODE_MAX] = 
		{
			GL_INTERLEAVED_ATTRIBS,		//INTERLEAVED,
			GL_SEPARATE_ATTRIBS			//SEPARATED,
		};

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
	GLenum program_target_cast(kueken::program::target Target)
	{
		static GLenum const Cast[kueken::program::TARGET_MAX] = 
		{
			GL_VERTEX_SHADER,			//VERTEX,
			GL_TESS_CONTROL_SHADER,		//CONTROL,
			GL_TESS_EVALUATION_SHADER,	//EVALUATION,
			GL_GEOMETRY_SHADER,			//PRIMITIVE,
			GL_FRAGMENT_SHADER,			//FRAGMENT
			GL_NONE,					//LIBRARY
		};

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

	creator::creator() :
		FeedbackBufferMode(0),
		Quiet(false),
		Built(false)
	{}	

	void creator::setVersion(version const & Version)
	{
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
			this->Sources[Target] += Source;
			break;
		case FILE:
			this->Sources[Target] += detail::loadFile(Source);
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

			SourcesBuilt[i] += Extensions;
			SourcesBuilt[i] += std::string("\n");

			SourcesBuilt[i] += Extensions;
			SourcesBuilt[i] += std::string("\n");

			SourcesBuilt[i] += Options;
			SourcesBuilt[i] += std::string("\n");

			SourcesBuilt[i] += Definitions;
			SourcesBuilt[i] += std::string("\n");

			SourcesBuilt[i] += Sources[i];
		}

		for(std::size_t i = 0; i < TARGET_MAX; ++i)
		{
			if(SourcesBuilt[i].find(std::string("main")) == std::string::npos)
				return this->Built = false;
		}

		return this->Built = true;
	}

	variable::variable(GLuint Program, GLuint Location, type Type) :
//		Data(0),
		Program(Program),
		Location(Location),
		Type(Type)
	{}

	variable::variable() :
//		Data(0),
		Program(0),
		Location(0),
		Type(NONE)
	{}

	void variable::set(float * Value, std::size_t Size)
	{
		glProgramUniform1fvEXT(
			Program, Location, 
			Size, Value);
	}

	void variable::set(glm::vec2 const * const Value, std::size_t Size)
	{
		glProgramUniform2fvEXT(
			Program, Location, 
			Size, glm::value_ptr(*Value));
	}

	void variable::set(buffer::name const & Buffer)
	{
		glUniformBufferEXT(
			Program, Location, 
			kueken::manager::instance().Buffer.getObject(Buffer).GetName());
	}

	void variable::set(glm::vec4 const & Value)
	{
		glProgramUniform4fEXT(
			Program, Location, 
			Value.r, Value.g, Value.b, Value.a);
	}

	void variable::set(glm::mat4 const & Value)
	{
		glm::mat4 Copy = Value;
		glProgramUniformMatrix4fvEXT(
			Program, Location, 
			1, GL_FALSE, &Copy[0][0]);
	}

	void variable::set(int Value)
	{
		glProgramUniform1iEXT(
			Program, Location, 
			Value);
	}

	object::object
	(
		creator const & Creator
	) :
		Name(0)
	{
		bool Success = true;
		
		// Compile a shader
		GLuint VertexShaderName = 0;
		if(Success)
		{
			std::string Source = Creator.SourcesBuilt[VERTEX];
			char const * SourcePtr = Source.c_str();
			VertexShaderName = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(VertexShaderName, 1, &SourcePtr, NULL);
			glCompileShader(VertexShaderName);

			Success = checkShader(VertexShaderName, SourcePtr);
		}

		// Compile a shader
		GLuint FragmentShaderName = 0;
		if(Success)
		{
			std::string Source = Creator.SourcesBuilt[FRAGMENT];
			char const * SourcePtr = Source.c_str();
			FragmentShaderName = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(FragmentShaderName, 1, &SourcePtr, NULL);
			glCompileShader(FragmentShaderName);

			Success = checkShader(FragmentShaderName, SourcePtr);
		}

		// Link a program
		if(Success)
		{
			Name = glCreateProgram();
			glAttachShader(Name, VertexShaderName);
			glDeleteShader(VertexShaderName);
			glAttachShader(Name, FragmentShaderName);
			glDeleteShader(FragmentShaderName);

			if(!Creator.FeedbackVariables.empty())
			{
				std::size_t VariablesSize = Creator.FeedbackVariables.size();
				char const ** VariablesData = new char const* [VariablesSize];

				for(std::size_t i = 0; i < Creator.FeedbackVariables.size(); ++i)
					VariablesData[i] = (char*)Creator.FeedbackVariables[i].c_str();

				glTransformFeedbackVaryings(
					Name, 
					GLsizei(Creator.FeedbackVariables.size()),
					VariablesData, 
					Creator.FeedbackBufferMode);

				delete[] VariablesData;
			}

			glLinkProgram(Name);

			Success = checkProgram(Name);
		}
	}

	object::~object()
	{
		if(Name)
			glDeleteProgram(Name);
	}

	void object::bind()
	{
		glUseProgram(Name);
	}

	void object::setSampler
	(
		std::string const & VarName, 
		int const & Value
	)
	{
		GLuint Location = glGetUniformLocation(this->Name, VarName.c_str());
		glProgramUniform1iEXT(
			this->Name, Location, 
			Value);
	}

	void object::setUniform
	(
		std::string const & VarName, 
		int const & Value
	)
	{
		GLuint Location = glGetUniformLocation(this->Name, VarName.c_str());
		glProgramUniform1iEXT(
			this->Name, Location, 
			Value);
	}

	void object::setUniform
	(
		std::string const & VarName, 
		glm::vec4 const & Value
	)
	{
		GLuint Location = glGetUniformLocation(this->Name, VarName.c_str());
		glProgramUniform4fvEXT(
			this->Name, Location, 
			1, glm::value_ptr(Value));
	}

	void object::setUniform
	(
		std::string const & VarName, 
		glm::mat4 const & Value
	)
	{
		GLuint Location = glGetUniformLocation(this->Name, VarName.c_str());
		glProgramUniformMatrix4fvEXT(
			this->Name, Location, 
			1, GL_FALSE, glm::value_ptr(Value));
	}

	variable object::get
	(
		std::string const & String, 
		type Type
	)
	{
		GLuint Location = 0;
		if(Type == UNIFORM || Type == SAMPLER)
			Location = glGetUniformLocation(Name, String.c_str());
		else
			Location = glGetAttribLocation(Name, String.c_str());
		assert(Location != GLuint(-1));

		variable Variable(Name, Location, Type);//, glGetUniformBufferSizeEXT());
		return Variable;
	}

}//namespace program
}//namespace kueken

