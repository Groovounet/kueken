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
	const char* TEXTURE_DIFFUSE = "../data/kueken256.tga";

	kueken::renderer * Renderer = 0;
	
	kueken::blend::name Blend;
	kueken::rasterizer::name RasterizerBackground;
	kueken::rasterizer::name RasterizerScene;
	kueken::clear::name ClearBackground;
	kueken::clear::name ClearScene;
	kueken::draw::name Draw;
	kueken::program::name Program;
	kueken::texture::name Texture;
	kueken::layout::name Layout;
	kueken::sampler::name Sampler;
	kueken::buffer::name ArrayBuffer;
	kueken::test::name Test;
	kueken::assembler::name Assembler;
	kueken::rendertarget::name Rendertarget;
	kueken::query::name Query;

	kueken::program::variable VariablePosition;
	kueken::program::variable VariableTexcoord;
}

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

	Renderer = new kueken::renderer;

	//Effect = new kueken::effect("test.kfx");

	bool Result = true;
	if(Result)
		Result = initQuery();
	if(Result)
		Result = initBlend();
	if(Result)
		Result = initClear();
	if(Result)
		Result = initRasterizer();
	if(Result)
		Result = initRendertarget();
	if(Result)
		Result = initArrayBuffer();
	if(Result)
		Result = initProgram();
	if(Result)
		Result = initLayout();
	if(Result)
		Result = initAssembler();
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
	delete Renderer;
	Renderer = 0;

	//delete Effect;
	//Effect = 0;

	return glf::checkError("End");
}

void sample::render()
{
	glf::checkError("Render 0");

    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 ViewTranslateZ = glm::translate(glm::mat4(1.0f), 0.0f, 0.0f, -8.0f);//-tranlationCurrent.y);
	glm::mat4 ViewRotateX = glm::rotate(ViewTranslateZ, rotationCurrent.y,-1.f, 0.f, 0.f);
	glm::mat4 ViewRotateY = glm::rotate(ViewRotateX, rotationCurrent.x, 0.f, 1.f, 0.f);
	glm::mat4 View = ViewRotateY;
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Projection * View * Model;
 
	kueken::program::object * Object = Renderer->map(Program);
	Object->setSampler("Diffuse", 0);
	Object->setUniform("MVP", MVP);
	Renderer->unmap(Program);

	glf::checkError("Render 2");

	Renderer->bind(Rendertarget, kueken::rendertarget::EXEC);
	glf::checkError("Render 3");

	Renderer->bind(RasterizerBackground);
	Renderer->exec(ClearBackground);
	glf::checkError("Render 4");

	Renderer->bind(RasterizerScene);
	Renderer->exec(ClearScene);
	glf::checkError("Render 5");

	Renderer->bind(Test);
	Renderer->bind(Blend);
	
	//Renderer->bind(0, kueken::buffer::ARRAY, ArrayBuffer);
	//Renderer->bind(0, kueken::layout::LAYOUT, Layout);
	Renderer->bind(Assembler);
	glf::checkError("Render 6");

	Renderer->bind(0, kueken::program::UNIFIED, Program);
	glf::checkError("Render 7");
	
	Renderer->bind(0, kueken::texture::IMAGE2D, Texture);
	Renderer->bind(0, kueken::sampler::SAMPLER, Sampler);

	glf::checkError("Render 8");

	//glDrawArraysInstanced(GL_TRIANGLES, 0, mesh.vertexCount(), 1);
	Renderer->exec(Draw);

	glf::checkError("Render 9");
	glf::swapbuffers();
	glf::checkError("Render");
}

bool sample::initBlend()
{
	kueken::blend::creator Creator;
	Creator.setColorMask(kueken::blend::SLOT0, glm::bvec4(true));
	Blend = Renderer->create(Creator);

	return glf::checkError("initBlend");
}

bool sample::initClear()
{
	kueken::clear::creator Creator;
	Creator.setColor(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
	ClearBackground = Renderer->create(Creator);
	Creator.setColor(glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));
	ClearScene = Renderer->create(Creator);

	return glf::checkError("initClear");
}

bool sample::initQuery()
{
	kueken::query::creator Creator;
	Creator.setTarget(kueken::query::SAMPLES);
	Query = Renderer->create(Creator);

	return glf::checkError("initQuery");	
}

bool sample::initDraw()
{
	kueken::draw::creator Creator;
	Creator.setFirst(0);
	Creator.setCount(Mesh.vertexCount());
	Draw = Renderer->create(Creator);

	return glf::checkError("initDraw");
}

bool sample::initTest()
{
	kueken::test::creator Creator;
	Creator.setDepthEnable(false);
	Test = Renderer->create(Creator);

	return glf::checkError("initTest");
}

bool sample::initRasterizer()
{
	{
		kueken::rasterizer::creator<kueken::rasterizer::POLYGON> Creator;
		Creator.setViewport(
			glm::ivec4(0, 0, windowSize));
		Creator.setScissor(
			false, glm::ivec4(0));
		RasterizerBackground = Renderer->create(Creator);
	}

	{
		kueken::rasterizer::creator<kueken::rasterizer::POLYGON> Creator;
		Creator.setViewport(
			glm::ivec4(0, 0, windowSize));
		Creator.setScissor(
			true, glm::ivec4(windowSize >> 2, windowSize >> 1));
		RasterizerScene = Renderer->create(Creator);
	}

	return glf::checkError("initRasterizer");
}

bool sample::initTexture2D()
{
	{
		gli::image ImageFile = gli::import_as(TEXTURE_DIFFUSE);

		kueken::texture::creator Creator;
		Creator.setFormat(kueken::texture::RGB8);
		Creator.setTarget(kueken::texture::IMAGE2D);
		for(kueken::texture::level Level = 0; Level < ImageFile.levels(); ++Level)
		{
			Creator.setImage(
				Level, 
				ImageFile[Level].dimensions(), 
				ImageFile[Level].data());
		}
		Texture = Renderer->create(Creator);

		kueken::texture::object* Object = Renderer->map(Texture);
		Object->generateMipmaps();
		Renderer->unmap(Texture);
	}

	{
		kueken::sampler::creator Creator;
		Creator.setFilter(kueken::sampler::TRILINEAR);
		Creator.setWrap(kueken::sampler::REPEAT, kueken::sampler::REPEAT, kueken::sampler::REPEAT);
		Creator.setAnisotropy(16.f);
		Sampler = Renderer->create(Creator);
	}

	return glf::checkError("initTexture2D");
}

bool sample::initProgram()
{
	kueken::program::creator Creator;
	Creator.setVersion(kueken::program::CORE_400);
	Creator.addSource(
		kueken::program::VERTEX, 
		kueken::program::FILE,
		VERTEX_SHADER_SOURCE);
	Creator.addSource(
		kueken::program::FRAGMENT, 
		kueken::program::FILE,
		FRAGMENT_SHADER_SOURCE);
	Creator.build();
	Program = Renderer->create(Creator);

	kueken::program::object* Object = Renderer->map(Program);
	VariablePosition = Object->get("Position", kueken::program::ATTRIB);
	VariableTexcoord = Object->get("Texcoord", kueken::program::ATTRIB);
	Renderer->unmap(Program);

	return glf::checkError("initProgram");
}

bool sample::initLayout()
{
	kueken::layout::creator Creator;
	Creator.setVertexArray(
		0, 
		kueken::layout::POSITION,
		kueken::layout::F32VEC2,
		Mesh.stride(glv::SLOT0),
		Mesh.offset(glv::POSITION2), 
		1);
	Creator.setVertexArray(
		0, 
		kueken::layout::TEXCOORD,
		kueken::layout::F32VEC2,
		Mesh.stride(glv::SLOT0),
		Mesh.offset(glv::TEXCOORD), 
		1);
	Layout = Renderer->create(Creator);

	return glf::checkError("initLayout");
}

bool sample::initAssembler()
{
	kueken::assembler::creator Creator;
	Creator.setPrimitive(kueken::assembler::TRIANGLES);
	Creator.addAttribute(
		ArrayBuffer, 
		VariablePosition, 
		kueken::assembler::F32VEC2,
		Mesh.stride(glv::SLOT0),
		Mesh.offset(glv::POSITION2));
	Creator.addAttribute(
		ArrayBuffer, 
		VariableTexcoord, 
		kueken::assembler::F32VEC2,
		Mesh.stride(glv::SLOT0),
		Mesh.offset(glv::TEXCOORD));

	Assembler = Renderer->create(Creator);

	return glf::checkError("initAssembler");
}

bool sample::initArrayBuffer()
{
	Mesh = glv::buildPlane(glv::POSITION2_BIT | glv::TEXCOORD_BIT, glm::vec2(1.0f));

	kueken::buffer::creator Creator;
	Creator.setSize(Mesh.vertexSize(glv::SLOT0));
	Creator.setData(Mesh.vertexData(glv::SLOT0));
	Creator.setUsage(kueken::buffer::STATIC_DRAW);
	ArrayBuffer = Renderer->create(Creator);

	return glf::checkError("initArrayBuffer");
}

bool sample::initRendertarget()
{
	kueken::rendertarget::creator<kueken::rendertarget::CUSTOM> Creator;
	Creator.setFramebuffer();
	Rendertarget = Renderer->create(Creator);

	return glf::checkError("initRendertarget");
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
