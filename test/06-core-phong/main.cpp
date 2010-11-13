//**********************************
// Kueken sample 06: Phong
// 20/03/2009
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
	const char* SAMPLE_NAME = "Kueken sample 06: Phong";	
	const char* VERTEX_SHADER_SOURCE = "../data/phong.vert";
	const char* FRAGMENT_SHADER_SOURCE = "../data/phong.frag";
	const char* TEXTURE_DIFFUSE = "../data/lady_snow.tga";
	const char* TEXTURE_SPECULAR = "../data/lady_snow_spe2.tga";

	kueken::renderer* Renderer = 0;
	
	kueken::blend::name Blend;
	kueken::rasterizer::name Rasterizer;
	kueken::clear::name Clear;
	kueken::draw::name Draw;
	kueken::program::name Program;
	kueken::sampler::name SamplerDiffuse;
	kueken::texture::name TextureDiffuse;
	kueken::sampler::name SamplerSpecular;
	kueken::texture::name TextureSpecular;
	kueken::buffer::name ArrayBuffer;
	kueken::test::name Test;
	kueken::assembler::name Assembler;
	kueken::buffer::name UniformBuffer;
	kueken::rendertarget::name Rendertarget;
	kueken::query::name Query;

	kueken::program::variable VariablePosition;
	kueken::program::variable VariableTexcoord;
	kueken::program::variable VariableDiffuse;
	kueken::program::variable VariableSpecular;
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
		Result = initUniformBuffer();
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
	glm::mat4 ViewTranslateZ = glm::translate(glm::mat4(1.0f), 0.0f, 0.0f, -tranlationCurrent.y);
	glm::mat4 ViewRotateX = glm::rotate(ViewTranslateZ, rotationCurrent.y,-1.f, 0.f, 0.f);
	glm::mat4 ViewRotateY = glm::rotate(ViewRotateX, rotationCurrent.x, 0.f, 1.f, 0.f);
	glm::mat4 View = ViewRotateY;
	glm::mat4 Model = glm::scale(0.5f, 1.0f, 1.0f);//glm::mat4(1.0f);
	glm::mat4 MVP = Projection * View * Model;
 
	{
		kueken::buffer::object* Uniform = Renderer->map(UniformBuffer);
		Uniform->set(sizeof(glm::mat4) * 0, sizeof(glm::mat4), &Model[0][0]);
		Uniform->set(sizeof(glm::mat4) * 1, sizeof(glm::mat4), &View[0][0]);
		Uniform->set(sizeof(glm::mat4) * 2, sizeof(glm::mat4), &Projection[0][0]);
		Renderer->unmap(UniformBuffer);
	}

	Renderer->bind(Rendertarget, kueken::rendertarget::EXEC);

	Renderer->bind(Rasterizer);
	Renderer->exec(Clear);

	Renderer->bind(Test);
	Renderer->bind(Blend);
	Renderer->bind(Assembler);

	Renderer->bind(0, kueken::texture::IMAGE2D, TextureDiffuse);
	Renderer->bind(0, kueken::sampler::SAMPLER, SamplerDiffuse);
	Renderer->bind(1, kueken::texture::IMAGE2D, TextureSpecular);
	Renderer->bind(1, kueken::sampler::SAMPLER, SamplerSpecular);
	VariableDiffuse.set(0);
	VariableSpecular.set(1);

	Renderer->bind(0, kueken::program::UNIFIED, Program);

	Renderer->exec(Draw);

	glf::checkError("Render");
}

bool CMain::initBlend()
{
	kueken::blend::creator Creator;
	Creator.setColorMask(
		kueken::blend::SLOT0, 
		glm::bvec4(true, true, true, true));
	Blend = Renderer->create(Creator);

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
	kueken::test::creator Creator;
	Creator.setDepthEnable(false);
	Test = Renderer->create(Creator);

	return glf::checkError("initTest");
}

bool CMain::initRasterizer()
{
	{
		kueken::rasterizer::creator<kueken::rasterizer::POLYGON> Creator;
		Creator.setViewport(
			glm::ivec4(0, 0, windowSize));
		Rasterizer = Renderer->create(Creator);
	}

	return glf::checkError("initRasterizer");
}

bool CMain::initTexture2D()
{
	{
		gli::image ImageFile = gli::import_as(TEXTURE_DIFFUSE);

		kueken::texture::creator Creator;
		Creator.setFormat(kueken::texture::RGB8);
		Creator.setTarget(kueken::texture::IMAGE2D);
		for(kueken::texture::level Level = 0; Level < ImageFile.levels(); ++Level)
		{
			glm::uvec3 Size(
				ImageFile[Level].dimensions().x, 
				ImageFile[Level].dimensions().y, 
				ImageFile[Level].dimensions().z);

			Creator.setImage(
				Level, 
				Size, 
				ImageFile[Level].data());
		}

		TextureDiffuse = Renderer->create(Creator);
	}

	{
		kueken::sampler::creator Creator;
		Creator.setFilter(kueken::sampler::TRILINEAR);
		Creator.setWrap(kueken::sampler::REPEAT, kueken::sampler::REPEAT, kueken::sampler::REPEAT);
		Creator.setAnisotropy(16.f);
		SamplerDiffuse = Renderer->create(Creator);
	}

	{
		gli::image ImageFile = gli::import_as(TEXTURE_SPECULAR);

		kueken::texture::creator Creator;
		Creator.setFormat(kueken::texture::RGB8);
		Creator.setTarget(kueken::texture::IMAGE2D);
		for(kueken::texture::level Level = 0; Level < ImageFile.levels(); ++Level)
		{
			glm::uvec3 Size(
				ImageFile[Level].dimensions().x, 
				ImageFile[Level].dimensions().y, 
				ImageFile[Level].dimensions().z);

			Creator.setImage(
				Level, 
				Size, 
				ImageFile[Level].data());
		}

		TextureSpecular = Renderer->create(Creator);
	}

	{
		kueken::sampler::creator Creator;
		Creator.setFilter(kueken::sampler::TRILINEAR);
		Creator.setWrap(kueken::sampler::REPEAT, kueken::sampler::REPEAT, kueken::sampler::REPEAT);
		Creator.setAnisotropy(16.f);
		SamplerSpecular = Renderer->create(Creator);
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
	VariableSpecular = Object->get("Specular", kueken::program::SAMPLER);
	VariableMVP = Object->get("Common", kueken::program::UNIFORM);
	Renderer->unmap(Program);

	VariableMVP.set(UniformBuffer);

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

bool CMain::initUniformBuffer()
{
	kueken::buffer::creator Creator;
	Creator.setSize(sizeof(glm::mat4) * 3);
	Creator.setData(0);
	Creator.setUsage(kueken::buffer::STREAM_READ);
	UniformBuffer = Renderer->create(Creator);

	return glf::checkError("initUniformBuffer");
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
