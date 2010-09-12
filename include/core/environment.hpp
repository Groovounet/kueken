#ifndef KUEKEN_ENVIRONMENT_INCLUDED
#define KUEKEN_ENVIRONMENT_INCLUDED

#include "detail/detail.hpp"
#include "program.hpp"

namespace kueken{
namespace environment
{
	enum semantic
	{
		
	};

	class creator// : public kueken::detail::creator
	{
		friend class object;

	public:
		creator();
		void setUniformSampler(
			semantic const & Semantic,
			sampler const & Sampler);
		void setUniformBlock(
			semantic const & Semantic,
			block const & Block);
		void setUniformSubroutine(
			semantic const & Semantic,
			subroutine const & Subroutine);

		bool build();

	private:
		void update();

		std::vector<GLint> Samplers;
		std::vector<GLint> Blocks;
		std::vector<GLint> Subroutines;

		bool Built;
	};

	class object
	{
	public:
		object(creator const & Creator);
		~object();

		void bind();

		template <typename genType>
		void setUniform(semantic const & Semantic, genType const & Value);
		void setSampler(semantic const & Semantic, int const & Value);

	private:
		//detail::data Data;
		GLuint Name;
		std::string Log;
	};

	typedef kueken::detail::name<object> name;
	typedef kueken::detail::manager<name, object> manager;

}//namespace environment
}//namespace kueken

#include "environment.inl"

#endif//KUEKEN_ENVIRONMENT_INCLUDED
