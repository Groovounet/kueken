#ifndef KUEKEN_MANAGER_INCLUDED
#define KUEKEN_MANAGER_INCLUDED

#include "buffer.hpp"
#include "blend.hpp"
#include "blit.hpp"
#include "clear.hpp"
#include "draw.hpp"
#include "layout.hpp"
#include "program.hpp"
#include "query.hpp"
#include "rasterizer.hpp"
#include "read_pixels.hpp"
#include "renderbuffer.hpp"
#include "framebuffer.hpp"
#include "sampler.hpp"
#include "test.hpp"
#include "texture.hpp"

namespace kueken{
namespace detail{

}//namespace detail

struct manager
{
public:
	manager();
	
	static manager & instance();
	static void destroy();

	blend::manager Blend; 
	blit::manager Blit; 
	buffer::manager Buffer; 
	clear::manager Clear; 
	draw::manager Draw;
	layout::manager Layout; 
	program::manager Program;
	query::manager Query; 
	rasterizer::manager Rasterizer; 
	readpixels::manager ReadPixels;
	renderbuffer::manager Renderbuffer;
	framebuffer::manager Framebuffer;
	sampler::manager Sampler; 
	test::manager Test; 
	texture::manager Texture; 

private:
	static std::unique_ptr<manager> Manager;
};

}//namespace kueken

#endif//KUEKEN_MANAGER_INCLUDED
