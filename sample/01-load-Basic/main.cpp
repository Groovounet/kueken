//**********************************
// Kueken Load sample 01
// 25/06/2009
//**********************************
// Christophe Riccio
// christophe@g-truc.net
//**********************************
// G-Truc Creation
// www.g-truc.net
//**********************************

#include "main.hpp"
#include <load/load.hpp>

namespace
{
	const char* SAMPLE_NAME = "Kueken Load Sample 01";	
	std::string const RASTERIZER_DEFAULT("../data/rasterizer-default.xml");
	std::string const RASTERIZER("../data/rasterizer1.xml");
	std::string const BLEND("../data/blend-default.xml");
	std::string const TEST("../data/test-default.xml");
	std::string const SAMPLER("../data/sampler1.xml");
	std::string const IMAGE("../data/image1.xml");
	std::string const PROGRAM("../data/program1.xml");

	kueken::renderer* Renderer = 0;
	
	kueken::blend::name Blend;
	kueken::rasterizer::name RasterizerBackground;
	kueken::rasterizer::name RasterizerScene;
	kueken::clear::name Clear;
	kueken::draw::name Draw;
	kueken::program::name Program;
	kueken::image::name Image;
	kueken::sampler::name Sampler;
	kueken::buffer::name ArrayBuffer;
	kueken::test::name Test;
	kueken::assembler::name Assembler;
	kueken::rendertarget::name Rendertarget;
	kueken::query::name Query;

	kueken::program::variable VariablePosition;
	kueken::program::variable VariableTexcoord;
	kueken::program::variable VariableDiffuse;
	kueken::program::variable VariableMVP;
}

CMain::CMain
(
	std::string const & Name, 
	glm::ivec2 const & WindowSize
) :
	IBase(Name, WindowSize)
{}

CMain::~CMain()
{}

bool CMain::Check() const
{
	return true;//GLEW_VERSION_2_1 == GL_TRUE;
}

bool CMain::Begin(glm::ivec2 const & WindowSize)
{
	windowSize = WindowSize;

	Renderer = new kueken::renderer;

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

bool CMain::End()
{
	delete Renderer;
	Renderer = 0;

	return glf::checkError("End");
}

void CMain::Render()
{
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 ViewRotateY = glm::rotate(rotationCurrent.x, 0.f, 1.f, 0.f);
	glm::mat4 ViewRotateX = glm::rotate(rotationCurrent.y,-1.f, 0.f, 0.f);
	glm::mat4 ViewTranslateZ = glm::translate(0.0f, 0.0f, -tranlationCurrent.y);
	glm::mat4 View = ViewTranslateZ * ViewRotateX * ViewRotateY;
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Projection * View * Model;
 
	VariableMVP.set(MVP);

	Renderer->bind(Rendertarget, kueken::rendertarget::EXEC);

	Renderer->bind(RasterizerBackground);
	Renderer->exec(Clear);

	Renderer->bind(RasterizerScene);
	Renderer->bind(Test);
	Renderer->bind(Blend);
	Renderer->bind(Assembler);

	Renderer->bind(0, kueken::program::UNIFIED, Program);
	Renderer->bind(1, kueken::image::IMAGE2D, Image);
	Renderer->bind(1, kueken::sampler::SAMPLER, Sampler);

	Renderer->exec(Draw);

	glf::checkError("Render");
}

bool CMain::initBlend()
{
	Blend = load::blend(*Renderer, BLEND);

	return glf::checkError("initBlend");
}

bool CMain::initClear()
{
	kueken::clear::creator Creator;
	Creator.setColor(glm::vec4(0.0f));
	Clear = Renderer->create(Creator);

	return glf::checkError("initClear");
}

bool CMain::initQuery()
{
	kueken::query::creator Creator;
	Creator.setTarget(kueken::query::SAMPLES);
	Query = Renderer->create(Creator);

	return glf::checkError("initQuery");	
}

bool CMain::initDraw()
{
	kueken::draw::creator Creator;
	Creator.setFirst(0);
	Creator.setCount(mesh.vertexCount());
	Draw = Renderer->create(Creator);

	return glf::checkError("initDraw");
}

bool CMain::initTest()
{
	Test = load::test(*Renderer, TEST);

	return glf::checkError("initTest");
}

bool CMain::initRasterizer()
{
	RasterizerBackground = load::rasterizer(*Renderer, RASTERIZER);
	RasterizerScene = load::rasterizer(*Renderer, RASTERIZER_DEFAULT);

	return glf::checkError("initRasterizer");
}

bool CMain::initTexture2D()
{
	{
		Image = load::image(*Renderer, IMAGE);

		kueken::image::object* Object = Renderer->map(Image);
		Object->generateMipmaps();
		Renderer->unmap(Image);
	}

	{
		Sampler = load::sampler(*Renderer, SAMPLER);
	}
	
	{
		//kueken::texture::creator<kueken::texture::image> Creator;
		//Creator.setVariable(VariableDiffuse);
		//Creator.setSampler(Sampler);
		//Creator.setImage(Image);
		//Texture = Renderer->create(Creator);
	}


	return glf::checkError("initTexture2D");
}

bool CMain::initProgram()
{
	Program = load::program(*Renderer, PROGRAM);

	kueken::program::object* Object = Renderer->map(Program);
	VariablePosition = Object->get("Position", kueken::program::ATTRIB);
	VariableTexcoord = Object->get("Texcoord", kueken::program::ATTRIB);
	VariableDiffuse = Object->get("Diffuse", kueken::program::SAMPLER);
	VariableMVP = Object->get("MVP", kueken::program::UNIFORM);
	Renderer->unmap(Program);

	return glf::checkError("initProgram");
}

bool CMain::initAssembler()
{
	kueken::assembler::creator Creator;
	Creator.setPrimitive(kueken::assembler::TRIANGLES);
	Creator.addAttribute(
		ArrayBuffer, 
		VariablePosition, 
		kueken::assembler::F32VEC2,
		mesh.stride(glv::SLOT0),
		mesh.offset(glv::POSITION2));
	Creator.addAttribute(
		ArrayBuffer, 
		VariableTexcoord, 
		kueken::assembler::F32VEC2,
		mesh.stride(glv::SLOT0),
		mesh.offset(glv::TEXCOORD));

	Assembler = Renderer->create(Creator);

	return glf::checkError("initAssembler");
}

bool CMain::initArrayBuffer()
{
	mesh = glv::buildPlane(glv::POSITION2_BIT | glv::TEXCOORD_BIT, glm::vec2(1.0f));

	kueken::buffer::creator Creator;
	Creator.setSize(mesh.vertexSize(glv::SLOT0));
	Creator.setData(mesh.vertexData(glv::SLOT0));
	Creator.setUsage(kueken::buffer::STATIC_DRAW);
	ArrayBuffer = Renderer->create(Creator);

	return glf::checkError("initArrayBuffer");
}

bool CMain::initRendertarget()
{
	kueken::rendertarget::creator<kueken::rendertarget::CUSTOM> Creator;
	Creator.setFramebuffer();
	Rendertarget = Renderer->create(Creator);

	return glf::checkError("initRendertarget");
}

int run()
{
	glm::ivec2 ScreenSize = glm::ivec2(640, 480);

	CMain* Main = new CMain(SAMPLE_NAME, ScreenSize);
	if(Main->Check())
	{
		Main->Begin(ScreenSize);
		Main->Run();
		Main->End();

		delete Main;
		return 0;
	}

	delete Main;
	return 1;
}
