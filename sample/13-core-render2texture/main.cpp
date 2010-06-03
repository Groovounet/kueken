//**********************************
// Kueken sample 13 - Render2Texture
// 14/04/2009
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
	const char* SAMPLE_NAME = "Kueken sample 13 - Render2Texture";	
	const char* VERTEX_SHADER_SOURCE = "../data/texture.vert";
	const char* FRAGMENT_SHADER_SOURCE = "../data/texture.frag";
	const char* TEXTURE_DIFFUSE = "../data/küken256.tga";

	kueken::renderer* Renderer = 0;
	
	kueken::blend::name Blend;
	kueken::draw::name Draw;
	kueken::program::name Program;
	kueken::sampler::name Sampler;
	kueken::buffer::name ArrayBuffer;
	kueken::test::name Test;
	kueken::assembler::name Assembler;
	kueken::query::name Query;

	kueken::image::name ImageDiffuse;
	//kueken::texture::name TextureDiffuse;

	kueken::image::name ImageTexture;
	//kueken::texture::name TextureTexture;

	kueken::rendertarget::name RendertargetTexture;
	kueken::rasterizer::name RasterizerTexture;
	kueken::clear::name ClearTexture;

	kueken::rendertarget::name RendertargetFramebuffer;
	kueken::rasterizer::name RasterizerFramebuffer;
	kueken::clear::name ClearFramebuffer;

	kueken::program::variable VariablePosition;
	kueken::program::variable VariableTexcoord;
	kueken::program::variable VariableDiffuse;
	kueken::program::variable VariableMVP;

	glm::uvec2 RendertargetSize = glm::uvec2(512);
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
		Result = initTest();
	if(Result)
		Result = initClear();
	if(Result)
		Result = initRasterizer();
	if(Result)
		Result = initArrayBuffer();
	if(Result)
		Result = initProgram();
	if(Result)
		Result = initAssembler();
	if(Result)
		Result = initTexture2D();
	if(Result)
		Result = initRendertarget();
	if(Result)
		Result = initDraw();

	Result = Result || glf::checkError("Begin");
	return Result;
}

bool CMain::End()
{
	delete Renderer;
	Renderer = 0;

	//delete Effect;
	//Effect = 0;

	return glf::checkError("End");
}

void CMain::Render()
{
	glm::mat4 ViewTranslateZ = glm::translate(glm::mat4(1.0f), 0.0f, 0.0f, -tranlationCurrent.y);
	glm::mat4 ViewRotateX = glm::rotate(ViewTranslateZ, rotationCurrent.y,-1.f, 0.f, 0.f);
	glm::mat4 ViewRotateY = glm::rotate(ViewRotateX, rotationCurrent.x, 0.f, 1.f, 0.f);
	glm::mat4 View = ViewRotateY;
	glm::mat4 Model = glm::mat4(1.0f);

	{
		Renderer->bind(Program);
		Renderer->bind(Test);
		Renderer->bind(Blend);
		Renderer->bind(Assembler);
	}

	{
		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 4.0f, 0.1f, 100.0f);
		glm::mat4 MVP = Projection * View * Model;

		Renderer->bind(RendertargetTexture, kueken::rendertarget::EXEC);
		Renderer->bind(RasterizerTexture);
		Renderer->exec(ClearTexture);

		VariableMVP.set(MVP);
		Renderer->bind(ImageDiffuse, kueken::image::SLOT0);
		Renderer->bind(Sampler, kueken::sampler::SLOT0);
		VariableDiffuse.set(0);
		Renderer->exec(Draw);

		kueken::image::object* Object = Renderer->map(ImageTexture);
		Object->generateMipmaps();
		Renderer->unmap(ImageTexture);
	}

	{
		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		glm::mat4 MVP = Projection * View * Model;

		Renderer->bind(RendertargetFramebuffer, kueken::rendertarget::EXEC);
		Renderer->bind(RasterizerFramebuffer);
		Renderer->exec(ClearFramebuffer);

		VariableMVP.set(MVP);
		Renderer->bind(ImageTexture, kueken::image::SLOT0);
		Renderer->bind(Sampler, kueken::sampler::SLOT0);
		//VariableTexture.set(0);
		Renderer->exec(Draw);
	}

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
	{
		kueken::clear::creator Creator;
		Creator.setColor(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
		ClearTexture = Renderer->create(Creator);
	}

	{
		kueken::clear::creator Creator;
		Creator.setColor(glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));
		ClearFramebuffer = Renderer->create(Creator);
	}

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
		Creator.setViewport(glm::ivec4(0, 0, RendertargetSize));
		RasterizerTexture = Renderer->create(Creator);
	}

	{
		kueken::rasterizer::creator<kueken::rasterizer::POLYGON> Creator;
		Creator.setViewport(glm::ivec4(0, 0, windowSize));
		RasterizerFramebuffer = Renderer->create(Creator);
	}

	return glf::checkError("initRasterizer");
}

bool CMain::initTexture2D()
{
	{
		kueken::sampler::creator Creator;
		Creator.setFilter(kueken::sampler::TRILINEAR);
		Creator.setWrap(kueken::sampler::REPEAT, kueken::sampler::REPEAT, kueken::sampler::REPEAT);
		Creator.setAnisotropy(16.f);
		Sampler = Renderer->create(Creator);
	}

	{
		gli::image ImageFile = gli::import_as(TEXTURE_DIFFUSE);

		kueken::image::creator Creator;
		Creator.setFormat(kueken::image::RGB8);
		Creator.setTarget(kueken::image::IMAGE2D);
		for(std::size_t Level = 0; Level < ImageFile.levels(); ++Level)
		{
			Creator.setMipmap(
				Level, 
				ImageFile[Level].dimensions(), 
				ImageFile[Level].data());
		}

		ImageDiffuse = Renderer->create(Creator);
	}

	{
		kueken::image::creator Creator;
		Creator.setFormat(kueken::image::RGB8);
		Creator.setTarget(kueken::image::IMAGE2D);
		Creator.setMipmap(0, glm::uvec3(RendertargetSize, 1), 0);

		ImageTexture = Renderer->create(Creator);
	}

	//{
	//	kueken::texture::creator<kueken::texture::image> Creator;
	//	Creator.setVariable(VariableDiffuse);
	//	Creator.setSampler(Sampler);
	//	Creator.setImage(ImageDiffuse);
	//	TextureDiffuse = Renderer->create(Creator);
	//}

	//{
	//	kueken::texture::creator<kueken::texture::image> Creator;
	//	Creator.setVariable(VariableDiffuse);
	//	Creator.setSampler(Sampler);
	//	Creator.setImage(ImageTexture);
	//	TextureTexture = Renderer->create(Creator);
	//}

	return glf::checkError("initTexture2D");
}

bool CMain::initProgram()
{
	kueken::program::creator Creator;
	Creator.attachShader(
		kueken::program::VERTEX, 
		glf::loadFile(VERTEX_SHADER_SOURCE));
	Creator.attachShader(
		kueken::program::FRAGMENT, 
		glf::loadFile(FRAGMENT_SHADER_SOURCE));
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
	{
		kueken::rendertarget::creator<kueken::rendertarget::FRAMEBUFFER> Creator;
		RendertargetFramebuffer = Renderer->create(Creator);
	}

	{
		kueken::rendertarget::creator<kueken::rendertarget::CUSTOM> Creator;
		Creator.setImage(kueken::rendertarget::COLOR0, ImageTexture, 0);
		RendertargetTexture = Renderer->create(Creator);
	}

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
