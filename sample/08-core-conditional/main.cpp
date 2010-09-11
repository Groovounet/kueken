//**********************************
// Kueken sample 08: Conditional render
// 03/04/2009
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
	const char* SAMPLE_NAME = "Kueken sample 08: Conditional render";	
	const char* VERTEX_SHADER_SOURCE1 = "../data/conditional1.vert";
	const char* FRAGMENT_SHADER_SOURCE1 = "../data/conditional1.frag";
	const char* VERTEX_SHADER_SOURCE2 = "../data/conditional2.vert";
	const char* FRAGMENT_SHADER_SOURCE2 = "../data/conditional2.frag";
	const char* TEXTURE_DIFFUSE = "../data/küken256.tga";

	core::renderer* Renderer = 0;
	
	kueken::rasterizer::name Rasterizer;
	kueken::clear::name Clear;
	kueken::sampler::name Sampler;
	kueken::texture::name Texture;
	kueken::buffer::name ArrayBuffer;
	kueken::test::name Test;
	kueken::buffer::name UniformBuffer;
	kueken::rendertarget::name Rendertarget;
	kueken::query::name Query;
	kueken::draw::name Draw;

	kueken::blend::name Blend1;
	kueken::program::name Program1;
	kueken::program::variable VariablePosition1;
	kueken::program::variable VariableDiffuse1;
	kueken::program::variable VariableMVP1;
	kueken::assembler::name Assembler1;

	kueken::blend::name Blend2;
	kueken::program::name Program2;
	kueken::program::variable VariablePosition2;
	kueken::program::variable VariableTexcoord2;
	kueken::program::variable VariableDiffuse2;
	kueken::program::variable VariableMVP2;
	kueken::assembler::name Assembler2;
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
 
	{
		kueken::buffer::object* Uniform = Renderer->map(UniformBuffer);
		Uniform->set(0, sizeof(glm::mat4), &MVP[0][0]);
		Renderer->unmap(UniformBuffer);
	}

	Renderer->bind(Rendertarget, kueken::rendertarget::EXEC);
	Renderer->bind(Rasterizer);
	Renderer->exec(Clear);

	Renderer->bind(Test);

	Renderer->bind(Blend1);
	Renderer->bind(0, kueken::program::UNIFIED, Program1);
	Renderer->bind(Assembler1);
	
	Renderer->begin(Query, kueken::query::RECORD);
		Renderer->exec(Draw);
	Renderer->end(Query, kueken::query::RECORD);

	printf("\rSamples: %d", Renderer->get(Query));

	Renderer->bind(Blend2);
	Renderer->bind(0, kueken::program::UNIFIED, Program2);
	Renderer->bind(0, kueken::texture::IMAGE2D, Texture);
	Renderer->bind(0, kueken::sampler::SAMPLER, Sampler);
	VariableDiffuse2.set(0);

	Renderer->bind(Assembler2);

	Renderer->begin(Query, kueken::query::CONDITION);
		Renderer->exec(Draw);
	Renderer->end(Query, kueken::query::CONDITION);

	glf::checkError("Render");
}

bool CMain::initBlend()
{
	{
		kueken::blend::creator Creator;
		Creator.setColorMask(
			kueken::blend::SLOT0, 
			glm::bvec4(false));
		Blend1 = Renderer->create(Creator);
	}

	{
		kueken::blend::creator Creator;
		Creator.setColorMask(
			kueken::blend::SLOT0, 
			glm::bvec4(true, true, true, true));
		Blend2 = Renderer->create(Creator);
	}

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
	Creator.setCondition(kueken::query::QUERY_WAIT);
	Query = Renderer->create(Creator);

	return glf::checkError("initQuery");	
}

bool CMain::initDraw()
{
	{
		kueken::draw::creator Creator;
		Creator.setFirst(0);
		Creator.setCount(mesh.vertexCount());
		Draw = Renderer->create(Creator);
	}

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
		Creator.setViewport(glm::ivec4(0, 0, windowSize));
		Rasterizer = Renderer->create(Creator);
	}

	return glf::checkError("initRasterizer");
}

bool CMain::initTexture2D()
{
	{
		gli::image ImageFile = gli::import_as(TEXTURE_DIFFUSE);
		gli::image ImageCrop = gli::crop(ImageFile, glm::uvec2(0), glm::uvec2(ImageFile[0].dimensions()) / glm::uvec2(2));

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
		kueken::texture::object * Object = Renderer->map(Texture);
		Object->set(0, 
			glm::uvec2(ImageFile[0].dimensions()) / glm::uvec2(4), 
			glm::uvec2(ImageCrop[0].dimensions()), 
			ImageCrop[0].data());
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

bool CMain::initProgram()
{
	{
		kueken::program::creator Creator;
		Creator.attachShader(kueken::program::VERTEX, glf::loadFile(VERTEX_SHADER_SOURCE1));
		Creator.attachShader(kueken::program::FRAGMENT, glf::loadFile(FRAGMENT_SHADER_SOURCE1));
		Program1 = Renderer->create(Creator);

		kueken::program::object* Object = Renderer->map(Program1);
		VariablePosition1 = Object->get("Position", kueken::program::ATTRIB);
		VariableDiffuse1 = Object->get("Diffuse", kueken::program::SAMPLER);
		VariableMVP1 = Object->get("MVP", kueken::program::UNIFORM);
		Renderer->unmap(Program1);

		VariableMVP1.set(UniformBuffer);
		VariableDiffuse1.set(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
	}

	{
		kueken::program::creator Creator;
		Creator.attachShader(kueken::program::VERTEX, glf::loadFile(VERTEX_SHADER_SOURCE2));
		Creator.attachShader(kueken::program::FRAGMENT, glf::loadFile(FRAGMENT_SHADER_SOURCE2));
		Program2 = Renderer->create(Creator);

		kueken::program::object* Object = Renderer->map(Program2);
		VariablePosition2 = Object->get("Position", kueken::program::ATTRIB);
		VariableTexcoord2 = Object->get("Texcoord", kueken::program::ATTRIB);
		VariableDiffuse2 = Object->get("Diffuse", kueken::program::SAMPLER);
		VariableMVP2 = Object->get("MVP", kueken::program::UNIFORM);
		Renderer->unmap(Program2);

		VariableMVP2.set(UniformBuffer);
	}

	return glf::checkError("initProgram");
}

bool CMain::initAssembler()
{
	{
		kueken::assembler::creator Creator;
		Creator.setPrimitive(kueken::assembler::TRIANGLES);
		Creator.addAttribute(
			ArrayBuffer, 
			VariablePosition1, 
			kueken::assembler::F32VEC2,
			mesh.stride(glv::SLOT0),
			mesh.offset(glv::POSITION2));

		Assembler1 = Renderer->create(Creator);
	}

	{
		kueken::assembler::creator Creator;
		Creator.setPrimitive(kueken::assembler::TRIANGLES);
		Creator.addAttribute(
			ArrayBuffer, 
			VariablePosition2, 
			kueken::assembler::F32VEC2,
			mesh.stride(glv::SLOT0),
			mesh.offset(glv::POSITION2));
		Creator.addAttribute(
			ArrayBuffer, 
			VariableTexcoord2, 
			kueken::assembler::F32VEC2,
			mesh.stride(glv::SLOT0),
			mesh.offset(glv::TEXCOORD));

		Assembler2 = Renderer->create(Creator);
	}

	return glf::checkError("initAssembler");
}

bool CMain::initUniformBuffer()
{
	kueken::buffer::creator Creator;
	Creator.setSize(sizeof(glm::mat4));
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
