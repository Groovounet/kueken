//**********************************
// Kueken sample 04
// 17/10/2010 - 17/10/2010
//**********************************
// Christophe Riccio
// christophe@g-truc.net
//**********************************
// G-Truc Creation
// www.g-truc.net
//**********************************

#include <glf/glf.hpp>
#include <core/kueken.hpp>

namespace
{
	const char* SAMPLE_NAME = "Kueken sample 04";	
	const char* VERTEX_SHADER_SOURCE = "./data/subroutine.vert";
	const char* FRAGMENT_SHADER_SOURCE = "./data/subroutine.frag";
	const char* TEXTURE_DIFFUSE = "./data/küken256dxt5.dds";
	int const SAMPLE_SIZE_WIDTH(640);
	int const SAMPLE_SIZE_HEIGHT(480);
	int const SAMPLE_MAJOR_VERSION(4);
	int const SAMPLE_MINOR_VERSION(0);

	glf::window Window(glm::ivec2(SAMPLE_SIZE_WIDTH, SAMPLE_SIZE_HEIGHT));

	GLsizei const VertexCount(4);
	GLsizeiptr const VertexSize = VertexCount * sizeof(glf::vertex_v2fv2f);
	glf::vertex_v2fv2f const VertexData[VertexCount] =
	{
		glf::vertex_v2fv2f(glm::vec2(-1.0f,-1.0f), glm::vec2(0.0f, 1.0f)),
		glf::vertex_v2fv2f(glm::vec2( 1.0f,-1.0f), glm::vec2(1.0f, 1.0f)),
		glf::vertex_v2fv2f(glm::vec2( 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
		glf::vertex_v2fv2f(glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 0.0f))
	};

	GLsizei const ElementCount(6);
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
	kueken::texture::name TextureRGB8(kueken::texture::name::null());
	kueken::texture::name TextureBGR8(kueken::texture::name::null());
	kueken::layout::name Layout(kueken::layout::name::null());
	kueken::sampler::name Sampler(kueken::sampler::name::null());
	kueken::buffer::name ArrayBuffer(kueken::buffer::name::null());
	kueken::buffer::name ElementBuffer(kueken::buffer::name::null());
	kueken::test::name Test(kueken::test::name::null());
	kueken::framebuffer::name Framebuffer(kueken::framebuffer::name::null());

	kueken::program::semantic const SEMANTIC_SAMPLER_DIFFUSE_RGB8(0);
	kueken::program::semantic const SEMANTIC_SAMPLER_DIFFUSE_BGR8(1);
	kueken::program::semantic const SEMANTIC_UNIFORM_MVP(2);
	kueken::program::semantic const SEMANTIC_ATTR_POSITION(0);
	kueken::program::semantic const SEMANTIC_ATTR_COLOR(1);
	kueken::program::semantic const SEMANTIC_ATTR_TEXCOORD(4);
	kueken::program::semantic const SEMANTIC_FRAG_COLOR(0);
	kueken::program::semantic const SEMANTIC_FUNCTION_DIFFUSE_RGB8(0);
	kueken::program::semantic const SEMANTIC_FUNCTION_DIFFUSE_BGR8(1);
	kueken::program::semantic const SEMANTIC_SUBROUTINE_DIFFUSE(0);
	
}//namespace

bool initBlend()
{
	kueken::blend::creator Creator(*Renderer);
	Creator.setColorMask(kueken::blend::SLOT0, glm::bvec4(true));
	Blend = Renderer->create(Creator);

	return glf::checkError("initBlend");
}

bool initClear()
{
	kueken::clear::creator Creator(*Renderer);
	Creator.setColor(glm::vec4(1.0f, 0.8f, 0.6f, 1.0f));
	ClearBackground = Renderer->create(Creator);
	Creator.setColor(glm::vec4(0.6f, 0.8f, 1.0f, 1.0f));
	ClearScene = Renderer->create(Creator);

	return glf::checkError("initClear");
}

bool initDraw()
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

bool initTest()
{
	kueken::test::creator Creator(*Renderer);
	Creator.setDepthEnable(false);
	Test = Renderer->create(Creator);

	return glf::checkError("initTest");
}

bool initRasterizer()
{
	{
		kueken::rasterizer::creator<kueken::rasterizer::POLYGON> Creator(*Renderer);
		Creator.setViewport(glm::ivec4(0, 0, Window.Size));
		Creator.setScissor(false, glm::ivec4(0));
		RasterizerBackground = Renderer->create(Creator);
	}

	{
		kueken::rasterizer::creator<kueken::rasterizer::POLYGON> Creator(*Renderer);
		Creator.setViewport(glm::ivec4(0, 0, Window.Size));
		Creator.setScissor(true, glm::ivec4(glm::ivec2(8), Window.Size - glm::ivec2(16)));
		RasterizerScene = Renderer->create(Creator);
	}

	return glf::checkError("initRasterizer");
}

bool initTexture2D()
{
	gli::texture2D ImageFile = gli::load(TEXTURE_DIFFUSE);

	{
		kueken::texture::creator<kueken::texture::IMAGE> Creator(*Renderer);
		Creator.setFormat(kueken::texture::RGBA_DXT5);
		Creator.setTarget(kueken::texture::TEXTURE2D);
		Creator.setSwizzle(kueken::texture::RED, kueken::texture::GREEN, kueken::texture::BLUE, kueken::texture::ALPHA);
		for(kueken::texture::level Level = 0; Level < ImageFile.levels(); ++Level)
		{
			Creator.setImage(
				Level, 
				glm::uvec3(ImageFile[Level].dimensions(), 1), 
				ImageFile[Level].data());
		}
		TextureRGB8 = Renderer->create(Creator);

		kueken::texture::object & Object = Renderer->map(TextureRGB8);
		Object.generateMipmaps();
		Renderer->unmap(TextureRGB8);
	}

	{
		kueken::texture::creator<kueken::texture::IMAGE> Creator(*Renderer);
		Creator.setFormat(kueken::texture::RGBA_DXT5);
		Creator.setTarget(kueken::texture::TEXTURE2D);
		Creator.setSwizzle(kueken::texture::BLUE, kueken::texture::GREEN, kueken::texture::RED, kueken::texture::ALPHA);
		for(kueken::texture::level Level = 0; Level < ImageFile.levels(); ++Level)
		{
			Creator.setImage(
				Level, 
				glm::uvec3(ImageFile[Level].dimensions(), 1), 
				ImageFile[Level].data());
		}
		TextureBGR8 = Renderer->create(Creator);

		kueken::texture::object & Object = Renderer->map(TextureBGR8);
		Object.generateMipmaps();
		Renderer->unmap(TextureBGR8);
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

bool initProgram()
{
	kueken::program::creator Creator(*Renderer);
	Creator.setVersion(kueken::program::CORE_400);
	Creator.addSource(kueken::program::VERTEX, kueken::program::FILE, VERTEX_SHADER_SOURCE);
	Creator.addSource(kueken::program::FRAGMENT, kueken::program::FILE,	FRAGMENT_SHADER_SOURCE);
	Creator.addVariable(SEMANTIC_SAMPLER_DIFFUSE_RGB8, "DiffuseRGB8");
	Creator.addVariable(SEMANTIC_SAMPLER_DIFFUSE_BGR8, "DiffuseBGR8");
	Creator.addVariable(SEMANTIC_UNIFORM_MVP, "MVP");
	Creator.addSemantic(SEMANTIC_ATTR_POSITION, "ATTR_POSITION");
	Creator.addSemantic(SEMANTIC_ATTR_COLOR, "ATTR_COLOR");
	Creator.addSemantic(SEMANTIC_ATTR_TEXCOORD, "ATTR_TEXCOORD");
	Creator.addSemantic(SEMANTIC_FRAG_COLOR, "FRAG_COLOR");
	Creator.addFunction(kueken::program::FRAGMENT, SEMANTIC_FUNCTION_DIFFUSE_RGB8, "diffuseRGB8");
	Creator.addFunction(kueken::program::FRAGMENT, SEMANTIC_FUNCTION_DIFFUSE_BGR8, "diffuseBGR8");
	Creator.addSubroutine(kueken::program::FRAGMENT, SEMANTIC_SUBROUTINE_DIFFUSE, "Diffuse");
	Creator.build();
	Program = Renderer->create(Creator);

	return glf::checkError("initProgram");
}

bool initLayout()
{
	kueken::layout::creator Creator(*Renderer);
	Creator.setVertexArray(
		0, 
		SEMANTIC_ATTR_POSITION,
		kueken::layout::FLOAT,
		kueken::layout::F32VEC2,
		sizeof(glf::vertex_v2fv2f),
		0, 
		0);
	Creator.setVertexArray(
		0, 
		SEMANTIC_ATTR_TEXCOORD,
		kueken::layout::FLOAT,
		kueken::layout::F32VEC2,
		sizeof(glf::vertex_v2fv2f),
		sizeof(glm::vec2), 
		0);
	Layout = Renderer->create(Creator);

	return glf::checkError("initLayout");
}

bool initArrayBuffer()
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

bool initFramebuffer()
{
	kueken::framebuffer::creator<kueken::framebuffer::CUSTOM> Creator(*Renderer);
	Creator.setFramebuffer();
	Framebuffer = Renderer->create(Creator);

	return glf::checkError("initFramebuffer");
}

bool begin()
{
	// Check the OpenGL version
	GLint MajorVersion = 0;
	GLint MinorVersion = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &MajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &MinorVersion);
	bool Validated = glf::version(MajorVersion, MinorVersion) >= glf::version(SAMPLE_MAJOR_VERSION, SAMPLE_MINOR_VERSION);

	Renderer.reset(new kueken::renderer);

	if(Validated)
		Validated = initBlend();
	if(Validated)
		Validated = initClear();
	if(Validated)
		Validated = initRasterizer();
	if(Validated)
		Validated = initTexture2D();
	if(Validated)
		Validated = initFramebuffer();
	if(Validated)
		Validated = initArrayBuffer();
	if(Validated)
		Validated = initProgram();
	if(Validated)
		Validated = initLayout();
	if(Validated)
		Validated = initTest();
	if(Validated)
		Validated = initDraw();

	Validated = Validated || glf::checkError("Begin");
	return Validated;
}

bool end()
{
	Renderer.reset();

	return glf::checkError("End");
}

void display()
{
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Window.TranlationCurrent.y));
	glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Window.RotationCurrent.y, glm::vec3(1.f, 0.f, 0.f));
	glm::mat4 View = glm::rotate(ViewRotateX, Window.RotationCurrent.x, glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Projection * View * Model;

	kueken::program::object & Object = Renderer->map(Program);
	Object.setSampler(SEMANTIC_SAMPLER_DIFFUSE_RGB8, kueken::sampler::SLOT0);
	Object.setSampler(SEMANTIC_SAMPLER_DIFFUSE_BGR8, kueken::sampler::SLOT1);
	Object.setUniform(SEMANTIC_UNIFORM_MVP, MVP);
	Object.setSubroutine(kueken::program::FRAGMENT, SEMANTIC_SUBROUTINE_DIFFUSE, SEMANTIC_FUNCTION_DIFFUSE_BGR8);
	Renderer->unmap(Program);

	Renderer->bind(Framebuffer, kueken::framebuffer::EXEC);

	Renderer->bind(RasterizerBackground);
	Renderer->exec(ClearBackground);

	Renderer->bind(RasterizerScene);
	Renderer->exec(ClearScene);

	Renderer->bind(Test);
	Renderer->bind(Blend);
	
	Renderer->bind(Program, kueken::program::UNIFIED);
	
	Renderer->bind(TextureRGB8, kueken::texture::SLOT0);
	Renderer->bind(TextureBGR8, kueken::texture::SLOT1);
	Renderer->bind(Sampler, kueken::sampler::SLOT0);
	Renderer->bind(Sampler, kueken::sampler::SLOT1);

	Renderer->bind(ElementBuffer, kueken::buffer::ELEMENT);
	Renderer->bind(ArrayBuffer, kueken::buffer::ARRAY);
	Renderer->bind(Layout);

	Renderer->exec(Draw);

	glf::swapBuffers();
	glf::checkError("Render");
}

int main(int argc, char* argv[])
{
	if(glf::run(
		argc, argv,
		glm::ivec2(::SAMPLE_SIZE_WIDTH, ::SAMPLE_SIZE_HEIGHT), 
		::SAMPLE_MAJOR_VERSION, 
		::SAMPLE_MINOR_VERSION))
		return 0;
	return 1;
}
