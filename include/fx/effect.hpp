#ifndef KUEKEN_FX_EFFECT_INCLUDED
#define KUEKEN_FX_EFFECT_INCLUDED

#include "../core/renderer.hpp"
//#include "blend.hpp"
//#include "image.hpp"
//#include "program.hpp"
//#include "rasterizer.hpp"
//#include "sampler.hpp"
//#include "test.hpp"

namespace fx{
namespace effect
{
	class object
	{
	public:
		object() :
			Renderer(0)
		{}
		object(kueken::renderer & Renderer, std::string const Filename);

		void load(void* Pointer);

		kueken::blend::name loadBlend(void * Node);
		kueken::blit::name loadBlit(void * Node);
		kueken::buffer::name loadBuffer(void * Node);
		kueken::clear::name loadClear(void * Node);
		kueken::draw::name loadDraw(void * Node);
		//kueken::feedback::name loadFeedback(void * Node);
		kueken::texture::name loadTexture(void * Node);
		kueken::program::name loadProgram(void * Node);
		kueken::query::name loadQuery(void * Node);
		kueken::rasterizer::name loadRasterizer(void * Node);
		kueken::readpixels::name loadReadpixel(void * Node);
		kueken::renderbuffer::name loadRenderbuffer(void * Node);
		kueken::rendertarget::name loadRendertarget(void * Node);
		kueken::sampler::name loadSampler(void * Node);
		kueken::test::name loadTest(void * Node);

		kueken::blend::name loadBlend(std::string const & Filename);
		kueken::blit::name loadBlit(std::string const & Filename);
		kueken::buffer::name loadBuffer(std::string const & Filename);
		kueken::clear::name loadClear(std::string const & Filename);
		kueken::draw::name loadDraw(std::string const & Filename);
		//kueken::feedback::name loadFeedback(std::string const & Filename);
		kueken::texture::name loadTexture(std::string const & Filename);
		kueken::program::name loadProgram(std::string const & Filename);
		kueken::query::name loadQuery(std::string const & Filename);
		kueken::rasterizer::name loadRasterizer(std::string const & Filename);
		kueken::readpixels::name loadReadpixel(std::string const & Filename);
		kueken::renderbuffer::name loadRenderbuffer(std::string const & Filename);
		kueken::rendertarget::name loadRendertarget(std::string const & Filename);
		kueken::sampler::name loadSampler(std::string const & Filename);
		kueken::test::name loadTest(std::string const & Filename);

		kueken::blend::name findBlend(glm::uint Id) const;
		kueken::blit::name findBlit(glm::uint Id) const;
		kueken::buffer::name findBuffer(glm::uint Id) const;
		kueken::clear::name findClear(glm::uint Id) const;
		kueken::draw::name findDraw(glm::uint Id) const;
		//kueken::feedback::name findFeedback(glm::uint Id) const;
		kueken::texture::name findTexture(glm::uint Id) const;
		kueken::program::name findProgram(glm::uint Id) const;
		kueken::query::name findQuery(glm::uint Id) const;
		kueken::rasterizer::name findRasterizer(glm::uint Id) const;
		kueken::readpixels::name findReadpixel(glm::uint Id) const;
		kueken::renderbuffer::name findRenderbuffer(glm::uint Id) const;
		kueken::rendertarget::name findRendertarget(glm::uint Id) const;
		kueken::sampler::name findSampler(glm::uint Id) const;
		kueken::test::name findTest(glm::uint Id) const;

	private:
		void loadAssets(void* Pointer);

		std::map<glm::uint, kueken::blend::name> Blends;
		std::map<glm::uint, kueken::blit::name> Blits;
		std::map<glm::uint, kueken::buffer::name> Buffers;
		std::map<glm::uint, kueken::clear::name> Clears;
		std::map<glm::uint, kueken::draw::name> Draws;
		//std::map<glm::uint, kueken::feedback::name> Feedbacks;
		std::map<glm::uint, kueken::texture::name> Textures;
		std::map<glm::uint, kueken::program::name> Programs;
		std::map<glm::uint, kueken::query::name> Querys;
		std::map<glm::uint, kueken::rasterizer::name> Rasterizers;
		std::map<glm::uint, kueken::readpixels::name> Readpixels;
		std::map<glm::uint, kueken::renderbuffer::name> Renderbuffers;
		std::map<glm::uint, kueken::rendertarget::name> Rendertargets;
		std::map<glm::uint, kueken::sampler::name> Samplers;
		std::map<glm::uint, kueken::test::name> Tests;

		kueken::renderer * Renderer;
	};

	//class object
	//{
	//public:
	//	void load(kueken::renderer & Renderer, std::string const & Filename);
	//	void exec();

	//private:
	//	std::vector<fx::pass::object> Passes;

	//	std::vector<kueken::assembler::name> Assemblers;
	//	std::vector<kueken::blend::name> Blends;
	//	std::vector<kueken::blit::name> Blits;
	//	std::vector<kueken::buffer::name> Buffers;
	//	std::vector<kueken::clear::name> Clears;
	//	std::vector<kueken::draw::name> Draws;
	//	std::vector<kueken::feedback::name> Feedbacks;
	//	std::vector<kueken::image::name> Images;
	//	std::vector<kueken::program::name> Programs;
	//	std::vector<kueken::query::name> Querys;
	//	std::vector<kueken::rasterizer::name> Rasterizers;
	//	std::vector<kueken::readpixels::name> Readpixels;
	//	std::vector<kueken::renderbuffer::name> Renderbuffers;
	//	std::vector<kueken::rendertarget::name> Rendertargets;
	//	std::vector<kueken::sampler::name> Samplers;
	//	std::vector<kueken::shader::name> Shaders;
	//	std::vector<kueken::test::name> Tests;
	//	std::vector<kueken::texture::name> Textures;
	//	std::vector<kueken::varable::name> Variables;
	//};

}//namespace effect
}//namespace fx

//#include "effect.inl"

#endif//KUEKEN_FX_EFFECT_INCLUDED
