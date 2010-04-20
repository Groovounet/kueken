#include <core/program.hpp>
#include <core/manager.hpp>

namespace
{
	GLenum program_shader_cast(kueken::program::shader Shader)
	{
		static GLenum const Cast[kueken::program::SHADER_MAX] = 
		{
			GL_VERTEX_SHADER,			//VERTEX,
			GL_GEOMETRY_SHADER_ARB,		//GEOMETRY,
			GL_FRAGMENT_SHADER,			//FRAGMENT
			GL_NONE,					//LIBRARY
		};

		return Cast[Shader];
	}

	GLenum program_buffer_mode_cast(kueken::buffer::mode Mode)
	{
		static GLenum const Cast[kueken::buffer::MODE_MAX] = 
		{
			GL_INTERLEAVED_ATTRIBS_EXT,		//INTERLEAVED,
			GL_SEPARATE_ATTRIBS_EXT			//SEPARATED,
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
namespace detail{

}//namespace detail

	void creator::attachShader(shader Shader, std::string const & Source)
	{
		Sources[Shader] += Source;
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
		Type(Type),
		Program(Program),
		Location(Location)
	{}

	variable::variable() :
//		Data(0),
		Program(0),
		Location(0)
	{}

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

	//void variable::set
	//(
	//	std::size_t Offset, 
	//	std::size_t Size, 
	//	void const * Pointer
	//)
	//{
	//	Data.resize(Size);
	//}

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
			std::string Source = Creator.Sources[LIBRARY] + Creator.Sources[VERTEX];
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
			std::string Source = Creator.Sources[LIBRARY] + Creator.Sources[FRAGMENT];
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

	variable object::get
	(
		std::string const & String, 
		type Type
	)
	{
		GLuint Location = 0;
		if(Type == UNIFORM)
			Location = glGetUniformLocation(Name, String.c_str());
		else
			Location = glGetAttribLocation(Name, String.c_str());
		assert(Location != GLuint(-1));

		variable Variable(Name, Location, Type);//, glGetUniformBufferSizeEXT());
		return Variable;
	}

}//namespace program
}//namespace kueken

