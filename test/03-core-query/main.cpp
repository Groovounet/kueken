//**********************************
// 03 - Query
// 18/02/2009
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
	const char* SAMPLE_NAME = "03 - Kueken - Query";	
	const char* VERTEX_SHADER_SOURCE = "../data/texture.vert";
	const char* FRAGMENT_SHADER_SOURCE = "../data/texture.frag";
	const char* TEXTURE_DIFFUSE_DDS = "../data/küken256dxt5.dds";
	const char* TEXTURE_DIFFUSE = "../data/küken256.tga";

	kueken::renderer* Renderer = 0;
	
	kueken::blend::name Blend;
	kueken::rasterizer::name Rasterizer;
	kueken::clear::name Clear;
	kueken::draw::name Draw;
	kueken::program::name Program;
	kueken::sampler::name Sampler;
	kueken::texture::name Texture;
	kueken::buffer::name ArrayBuffer;
	kueken::buffer::name ElementBuffer;
	kueken::test::name Test;
	kueken::assembler::name Assembler;
	kueken::rendertarget::name Rendertarget;
	kueken::query::name QuerySamples;

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
		Result = initBlend();
	if(Result)
		Result = initClear();
	if(Result)
		Result = initQuery();
	if(Result)
		Result = initRasterizer();
	if(Result)
		Result = initRendertarget();
	if(Result)
		Result = initProgram();
	if(Result)
		Result = initArrayBuffer();
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
	glm::mat4 ViewTranslateZ = glm::translate(0.0f, 0.0f, -tranlationCurrent.y);
	glm::mat4 ViewRotateX = glm::rotate(ViewTranslateZ, rotationCurrent.y,-1.f, 0.f, 0.f);
	glm::mat4 ViewRotateY = glm::rotate(ViewRotateX, rotationCurrent.x, 0.f, 1.f, 0.f);
	glm::mat4 View = ViewRotateY;
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Projection * View * Model;
 
	VariableMVP.set(MVP);

	Renderer->bind(Rendertarget, kueken::rendertarget::EXEC);

	Renderer->bind(Rasterizer);
	Renderer->exec(Clear);

	Renderer->bind(Test);
	Renderer->bind(Blend);
	Renderer->bind(Assembler);

	Renderer->bind(0, kueken::program::UNIFIED, Program);
	Renderer->bind(0, kueken::sampler::SAMPLER, Sampler);
	Renderer->bind(0, kueken::texture::IMAGE2D, Texture);

	Renderer->begin(QuerySamples, kueken::query::RECORD);
		Renderer->exec(Draw);
	Renderer->end(QuerySamples, kueken::query::RECORD);
	
	printf("\rSamples: %d", Renderer->get(QuerySamples));

	//{
	//	kueken::draw::object* Object = Renderer->map(Draw);
	//	glm::uint64 Samples = Object->query(kueken::draw::SAMPLES, false);
	//	glm::uint64 Primitives = Object->query(kueken::draw::PRIMITIVES, false);
	//	Renderer->unmap(Draw);
	//}

	glf::checkError("Render");
}

bool CMain::initBlend()
{
	kueken::blend::creator Creator;
	Creator.setColorMask(kueken::blend::SLOT0, glm::bvec4(true));
	Blend = Renderer->create(Creator);

	return glf::checkError("initBlend");
}

bool CMain::initClear()
{
	kueken::clear::creator Creator;
	Creator.setColor(glm::vec4(1.0f));
	Clear = Renderer->create(Creator);

	return glf::checkError("initClear");
}

bool CMain::initDraw()
{
	kueken::draw::creator Creator;
	Creator.setFirst(0);
	Creator.setCount(mesh.elementCount());
	//Creator.setQuery(kueken::draw::SAMPLES, true);
	//Creator.setQuery(kueken::draw::PRIMITIVES, true);
	Draw = Renderer->create(Creator);

	return glf::checkError("initDraw");
}

bool CMain::initQuery()
{
	kueken::query::creator Creator;
	Creator.setTarget(kueken::query::SAMPLES);
	Creator.setWait(false);
	QuerySamples = Renderer->create(Creator);

	return glf::checkError("initQuery");
}

bool CMain::initTest()
{
	kueken::test::creator Creator;
	Creator.setDepthEnable(false);
	Test = Renderer->create(Creator);

	return glf::checkError("initTest");
}

bool CMain::initRasterizer()
{
	{
		kueken::rasterizer::creator<kueken::rasterizer::POLYGON> Creator;
		Creator.setViewport(glm::ivec4(0, 0, windowSize));
		Rasterizer = Renderer->create(Creator);
	}

	return glf::checkError("initRasterizer");
}

bool CMain::initTexture2D()
{
	{
		gli::image ImageFile = gli::import_as(TEXTURE_DIFFUSE);

		kueken::texture::creator Creator;
		//Creator.setFormat(kueken::texture::RGBA_DXT5);
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
	}

	{
		kueken::sampler::creator Creator;
		Creator.setFilter(kueken::sampler::BILINEAR);
		Creator.setWrap(kueken::sampler::REPEAT, kueken::sampler::REPEAT, kueken::sampler::REPEAT);
		Creator.setAnisotropy(16.f);
		Sampler = Renderer->create(Creator);
	}
	
	return glf::checkError("initTexture2D");
}

bool CMain::initProgram()
{
	kueken::program::creator Creator;
	Creator.addSource(
		kueken::program::VERTEX, 
		kueken::program::FILE,
		VERTEX_SHADER_SOURCE);
	Creator.addSource(
		kueken::program::FRAGMENT, 
		kueken::program::FILE,
		FRAGMENT_SHADER_SOURCE);
	Program = Renderer->create(Creator);

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
	
	Creator.setPrimitive(
		kueken::assembler::TRIANGLES);

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

	Creator.setElements(
		kueken::assembler::U16, 
		ElementBuffer);

	Assembler = Renderer->create(Creator);

	return glf::checkError("initAssembler");
}

bool CMain::initArrayBuffer()
{
	mesh = glv::buildPlane(glv::ELEMENT_BIT | glv::POSITION2_BIT | glv::TEXCOORD_BIT, glm::vec2(1.0f));

	{
		kueken::buffer::creator Creator;
		Creator.setSize(mesh.vertexSize(glv::SLOT0));
		Creator.setData(mesh.vertexData(glv::SLOT0));
		Creator.setUsage(kueken::buffer::STATIC_DRAW);
		ArrayBuffer = Renderer->create(Creator);
	}

	{
		kueken::buffer::creator Creator;
		Creator.setSize(mesh.elementSize());
		Creator.setData(mesh.elementData());
		Creator.setUsage(kueken::buffer::STATIC_DRAW);
		ElementBuffer = Renderer->create(Creator);
	}

	return glf::checkError("initArrayBuffer");
}

bool CMain::initRendertarget()
{
	kueken::rendertarget::creator<kueken::rendertarget::FRAMEBUFFER> Creator;
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
