//**********************************
// Kueken sample 01
// 05/01/2009
//**********************************
// Christophe Riccio
// christophe@g-truc.net
//**********************************
// G-Truc Creation
// www.g-truc.net
//**********************************

#include "main.hpp"
 
namespace
{
	const char* SAMPLE_NAME = "Kueken sample 01";	
	const char* VERTEX_SHADER_SOURCE = "../data/texture.vert";
	const char* FRAGMENT_SHADER_SOURCE = "../data/texture.frag";
	const char* TEXTURE_DIFFUSE = "../data/küken256dxt5.dds";

	GLsizei const VertexCount = 4;
	GLsizeiptr const VertexSize = VertexCount * sizeof(vertex_v2fv2f);
	vertex_v2fv2f const VertexData[VertexCount] =
	{
		vertex_v2fv2f(glm::vec2(-1.0f,-1.0f), glm::vec2(0.0f, 1.0f)),
		vertex_v2fv2f(glm::vec2( 1.0f,-1.0f), glm::vec2(1.0f, 1.0f)),
		vertex_v2fv2f(glm::vec2( 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
		vertex_v2fv2f(glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 0.0f))
	};

	GLsizei const ElementCount = 6;
	GLsizeiptr const ElementSize = ElementCount * sizeof(GLuint);
	GLuint const ElementData[ElementCount] =
	{
		0, 1, 2, 
		2, 3, 0
	};

	std::unique_ptr<kueken::renderer> Renderer(nullptr);
	
	kueken::blend::name Blend(kueken::blend::name::null());
	kueken::rasterizer::name RasterizerBackground(kueken::rasterizer::name::null());
	kueken::rasterizer::name RasterizerScene(kueken::rasterizer::name::null());
	kueken::clear::name ClearBackground(kueken::clear::name::null());
	kueken::clear::name ClearScene(kueken::clear::name::null());
	kueken::draw::name Draw(kueken::draw::name::null());
	kueken::program::name Program(kueken::program::name::null());
	kueken::texture::name Texture(kueken::texture::name::null());
	kueken::layout::name Layout(kueken::layout::name::null());
	kueken::sampler::name Sampler(kueken::sampler::name::null());
	kueken::buffer::name ArrayBuffer(kueken::buffer::name::null());
	kueken::buffer::name ElementBuffer(kueken::buffer::name::null());
	kueken::test::name Test(kueken::test::name::null());
	kueken::framebuffer::name Framebuffer(kueken::framebuffer::name::null());

	kueken::program::semantic const SEMANTIC_DIFFUSE(0);
	kueken::program::semantic const SEMANTIC_MVP(1);

	kueken::program::semantic const SEMANTIC_POSITION(0);
	kueken::program::semantic const SEMANTIC_TEXCOORD(4);
}//namespace

sample::sample
(
	std::string const & Name, 
	glm::ivec2 const & WindowSize
) :
	base(Name, WindowSize)
{}

sample::~sample()
{}

bool sample::check() const
{
	return true;//GLEW_VERSION_2_1 == GL_TRUE;
}

bool sample::begin(glm::ivec2 const & WindowSize)
{
	windowSize = WindowSize;

	Renderer.reset(new kueken::renderer);

	bool Result = true;

	if(Result)
		Result = initBlend();
	if(Result)
		Result = initClear();
	if(Result)
		Result = initRasterizer();
	if(Result)
		Result = initFramebuffer();
	if(Result)
		Result = initArrayBuffer();
	if(Result)
		Result = initProgram();
	if(Result)
		Result = initLayout();
	if(Result)
		Result = initTexture2D();
	if(Result)
		Result = initTest();
	if(Result)
		Result = initDraw();

	Result = Result || glf::checkError("Begin");
	return Result;
}

bool sample::end()
{
	Renderer.reset();

	return glf::checkError("End");
}

void sample::render()
{
	static float Rotate = 0.0f;
	Rotate += 0.01f;

    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 ViewTranslateZ = glm::translate(glm::mat4(1.0f), 0.0f, 0.0f, -4.0f);//-tranlationCurrent.y);
	glm::mat4 ViewRotateX = glm::rotate(ViewTranslateZ, rotationCurrent.y,-1.f, 0.f, 0.f);
	glm::mat4 ViewRotateY = glm::rotate(ViewRotateX, rotationCurrent.x, 0.f, 1.f, 0.f);
	glm::mat4 ViewRotateZ = glm::rotate(ViewRotateY, Rotate, 0.f, 0.f, 1.f);
	glm::mat4 View = ViewRotateZ;
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Projection * View * Model;
 
	kueken::program::object & Object = Renderer->map(Program);
	Object.setSampler(SEMANTIC_DIFFUSE, 0);
	Object.setUniform(SEMANTIC_MVP, MVP);
	Renderer->unmap(Program);

	Renderer->bind(kueken::framebuffer::EXEC, Framebuffer);

	Renderer->bind(kueken::rasterizer::RASTERIZER, RasterizerBackground);
	Renderer->exec(ClearBackground);

	Renderer->bind(kueken::rasterizer::RASTERIZER, RasterizerScene);
	Renderer->exec(ClearScene);

	Renderer->bind(kueken::test::TEST, Test);
	Renderer->bind(kueken::blend::BLEND, Blend);
	
	Renderer->bind(0, kueken::program::UNIFIED, Program);
	
	Renderer->bind(0, kueken::texture::IMAGE2D, Texture);
	Renderer->bind(0, kueken::sampler::SAMPLER, Sampler);

	Renderer->bind(0, kueken::buffer::ELEMENT, ElementBuffer);
	Renderer->bind(1, kueken::buffer::ARRAY, ArrayBuffer);
	Renderer->bind(0, kueken::layout::VERTEX, Layout);

	Renderer->exec(Draw);

	glf::swapbuffers();
	glf::checkError("Render");
}

bool sample::initBlend()
{
	kueken::blend::creator Creator(*Renderer);
	Creator.setColorMask(kueken::blend::SLOT0, glm::bvec4(true));
	Blend = Renderer->create(Creator);

	return glf::checkError("initBlend");
}

bool sample::initClear()
{
	kueken::clear::creator Creator(*Renderer);
	Creator.setColor(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
	ClearBackground = Renderer->create(Creator);
	Creator.setColor(glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));
	ClearScene = Renderer->create(Creator);

	return glf::checkError("initClear");
}

bool sample::initDraw()
{
	kueken::draw::creator<kueken::draw::ELEMENT> Creator(*Renderer);
	Creator.setPrimitive(kueken::draw::TRIANGLES);
	Creator.setElementFormat(kueken::draw::UINT32);
	Creator.setFirst(0);
	Creator.setCount(ElementCount);
	Creator.setInstances(1);
	Creator.setBaseVertex(0);
	Draw = Renderer->create(Creator);

	return glf::checkError("initDraw");
}

bool sample::initTest()
{
	kueken::test::creator Creator(*Renderer);
	Creator.setDepthEnable(false);
	Test = Renderer->create(Creator);

	return glf::checkError("initTest");
}

bool sample::initRasterizer()
{
	{
		kueken::rasterizer::creator<kueken::rasterizer::POLYGON> Creator(*Renderer);
		Creator.setViewport(
			glm::ivec4(0, 0, windowSize));
		Creator.setScissor(
			false, glm::ivec4(0));
		RasterizerBackground = Renderer->create(Creator);
	}

	{
		kueken::rasterizer::creator<kueken::rasterizer::POLYGON> Creator(*Renderer);
		Creator.setViewport(
			glm::ivec4(0, 0, windowSize));
		Creator.setScissor(
			true, glm::ivec4(glm::ivec2(8), windowSize - glm::ivec2(16)));
		RasterizerScene = Renderer->create(Creator);
	}

	return glf::checkError("initRasterizer");
}

bool sample::initTexture2D()
{
	{
		gli::image ImageFile = gli::import_as(TEXTURE_DIFFUSE);

		kueken::texture::creator Creator(*Renderer);
		Creator.setFormat(kueken::texture::RGBA_DXT5);
		Creator.setTarget(kueken::texture::IMAGE2D);
		for(kueken::texture::level Level = 0; Level < ImageFile.levels(); ++Level)
		{
			Creator.setImage(
				Level, 
				ImageFile[Level].dimensions(), 
				ImageFile[Level].data());
		}
		Texture = Renderer->create(Creator);

		kueken::texture::object & Object = Renderer->map(Texture);
		Object.generateMipmaps();
		Renderer->unmap(Texture);
	}

	{
		kueken::sampler::creator Creator(*Renderer);
		Creator.setFilter(kueken::sampler::TRILINEAR);
		Creator.setWrap(kueken::sampler::REPEAT, kueken::sampler::REPEAT, kueken::sampler::REPEAT);
		Creator.setAnisotropy(16.f);
		Sampler = Renderer->create(Creator);
	}

	return glf::checkError("initTexture2D");
}

bool sample::initProgram()
{
	kueken::program::creator Creator(*Renderer);
	Creator.setVersion(kueken::program::CORE_410);
	Creator.addSource(
		kueken::program::VERTEX, 
		kueken::program::FILE,
		VERTEX_SHADER_SOURCE);
	Creator.addSource(
		kueken::program::FRAGMENT, 
		kueken::program::FILE,
		FRAGMENT_SHADER_SOURCE);
	Creator.addVariable(
		SEMANTIC_DIFFUSE, 
		"Diffuse");
	Creator.addVariable(
		SEMANTIC_MVP, 
		"MVP");

	Creator.build();
	Program = Renderer->create(Creator);

	return glf::checkError("initProgram");
}

bool sample::initLayout()
{
	kueken::layout::creator Creator(*Renderer);
	Creator.setVertexArray(
		0, 
		kueken::layout::POSITION,
		kueken::layout::F32VEC2,
		sizeof(vertex_v2fv2f),
		0, 
		0);
	Creator.setVertexArray(
		0, 
		kueken::layout::TEXCOORD,
		kueken::layout::F32VEC2,
		sizeof(vertex_v2fv2f),
		sizeof(glm::vec2), 
		0);
	Layout = Renderer->create(Creator);

	return glf::checkError("initLayout");
}

bool sample::initArrayBuffer()
{
	{
		kueken::buffer::creator Creator(*Renderer);
		Creator.setSize(VertexSize);
		Creator.setData(VertexData);
		Creator.setUsage(kueken::buffer::STATIC_DRAW);
		ArrayBuffer = Renderer->create(Creator);
	}

	{
		kueken::buffer::creator Creator(*Renderer);
		Creator.setSize(ElementSize);
		Creator.setData(ElementData);
		Creator.setUsage(kueken::buffer::STATIC_DRAW);
		ElementBuffer = Renderer->create(Creator);
	}

	return glf::checkError("initArrayBuffer");
}

bool sample::initFramebuffer()
{
	kueken::framebuffer::creator<kueken::framebuffer::CUSTOM> Creator(*Renderer);
	Creator.setFramebuffer();
	Framebuffer = Renderer->create(Creator);

	return glf::checkError("initFramebuffer");
}

int main(int argc, char* argv[])
{
	glm::ivec2 ScreenSize = glm::ivec2(640, 480);

	sample * Sample = new sample(SAMPLE_NAME, ScreenSize);
	if(Sample->check())
	{
		Sample->begin(ScreenSize);
		do
		{
			Sample->render();
		}
		while(Sample->event());
		Sample->end();

		delete Sample;
		return 0;
	}

	delete Sample;
	return 1;
}
