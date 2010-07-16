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

}//namespace detail

	void creator::attachShader(target const & Target, std::string const & Source)
	{
		Sources[Target] += Source;
	}

	void creator::setFeedbackVariable
	(
		std::vector<std::string> const & Names, 
		buffer::mode const & Mode
	)
	{
		FeedbackVariables = Names;
		BufferMode = program_buffer_mode_cast(Mode);
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
			std::string Source = Creator.Sources[VERTEX];
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
			std::string Source = Creator.Sources[FRAGMENT];
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

				glTransformFeedbackVaryingsEXT(
					Name, 
					GLsizei(Creator.FeedbackVariables.size()),
					VariablesData, 
					Creator.BufferMode);

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

