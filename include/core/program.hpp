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

	typedef std::size_t slot;

namespace detail
{
	GLenum program_target_cast(kueken::program::target Target);
}//namespace detail

	class creator// : public kueken::detail::creator
	{
		friend class object;

	public:
		void attachShader(target const & Target, std::string const & Source);
		void setStage(target const & Target);
		void setFeedbackVariable(std::vector<std::string> const & Names, buffer::mode const & Mode);

		virtual bool validate(){return true;}

	private:
		std::string Sources[TARGET_MAX];
		std::vector<std::string> FeedbackVariables;
		GLenum BufferMode;
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
		variable get(std::string const & Name, type Type);

	private:
		//detail::data Data;
		GLuint Name;
		std::string Log;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace program
}//namespace kueken

#include "program.inl"

#endif//KUEKEN_PROGRAM_INCLUDED
