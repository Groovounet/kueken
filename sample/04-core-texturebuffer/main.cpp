//**********************************
// Kueken TextureBuffer 04
// 12/02/2009
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
	const char* SAMPLE_NAME = "Kueken TextureBuffer 04";	
	const char* VERTEX_SHADER_SOURCE = "../data/texture-buffer.vert";
	const char* FRAGMENT_SHADER_SOURCE = "../data/texture-buffer.frag";
	const char* TEXTURE_DIFFUSE = "../data/küken256.tga";

	kueken::renderer* Renderer = 0;
	
	kueken::test::name Test;
	kueken::blend::name Blend;
	kueken::rasterizer::name Rasterizer;
	kueken::clear::name Clear;
	kueken::draw::name Draw;
	kueken::program::name Program;

	kueken::image::name ImageDiffuse;
	kueken::sampler::name SamplerDiffuse;
	kueken::texture::name TextureDiffuse;
	kueken::texture::name TextureMVP;

	kueken::buffer::name ArrayBuffer;
	kueken::buffer::name ElementBuffer;
	kueken::assembler::name Assembler;

	kueken::rendertarget::name Rendertarget;

	kueken::program::variable VariablePosition;
	kueken::program::variable VariableTexcoord;
	kueken::program::variable VariableDiffuse;
	kueken::program::variable VariableMVP;

	kueken::buffer::name TextureBuffer;
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
	return 
		GLEW_EXT_framebuffer_object == GL_TRUE && 
		GLEW_EXT_direct_state_access == GL_TRUE && 
		GLEW_EXT_texture_buffer_object == GL_TRUE && 
		GLEW_EXT_texture_swizzle == GL_TRUE;
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
		Result = initImage2D();
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
		Result = initSampler();
	if(Result)
		Result = initTexture2D();
	if(Result)
		Result = initTextureBuffer();
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
		kueken::buffer::object* Object = Renderer->map(TextureBuffer);
		Object->set(0, sizeof(glm::mat4), &MVP[0][0]);
		Renderer->unmap(TextureBuffer);
	}

	Renderer->bind(Rendertarget, kueken::rendertarget::EXEC);
	Renderer->bind(Rasterizer);
	Renderer->exec(Clear);

	Renderer->bind(Test);
	Renderer->bind(Blend);
	Renderer->bind(Assembler);

	Renderer->bind(Program);
	Renderer->bind(TextureDiffuse, kueken::texture::SLOT0);
	Renderer->bind(TextureMVP, kueken::texture::SLOT1);

	Renderer->exec(Draw);

	glf::checkError("Render");
}

bool CMain::initBlend()
{
	{
		kueken::blend::creator Creator;
		Creator.setBlend(
			kueken::blend::SLOT0, false);
		Creator.setSource(
			kueken::blend::SRC_ALPHA, 
			kueken::blend::SRC_ALPHA);
		Creator.setDestination(
			kueken::blend::ONE_MINUS_SRC_ALPHA, 
			kueken::blend::ONE_MINUS_SRC_ALPHA);
		Creator.setColorMask(
			kueken::blend::SLOT0, 
			glm::bvec4(true, true, true, true));
		Blend = Renderer->create(Creator);
	}

	return glf::checkError("initBlend");
}

bool CMain::initClear()
{
	{
		kueken::clear::creator Creator;
		Creator.setColor(glm::vec4(0.0f));
		Creator.setDepth(1.0f);
		Clear = Renderer->create(Creator);
	}

	return glf::checkError("initClear");
}

bool CMain::initDraw()
{
	{
		kueken::draw::creator Creator;
		Creator.setFirst(0);
		Creator.setCount(mesh.elementCount());
		Draw = Renderer->create(Creator);
	}

	return glf::checkError("initDraw");
}

bool CMain::initImage2D()
{
	{
		gli::image ImageFile = gli::import_as(TEXTURE_DIFFUSE);
		gli::image ImageCopy = gli::duplicate(ImageFile);
		gli::image ImageCrop = gli::crop(ImageFile, glm::uvec2(0), glm::uvec2(128));
		gli::export_as(ImageFile, "../data/küken256_saved.tga");
		gli::export_as(ImageCopy, "../data/küken256_copy.tga");
		gli::export_as(ImageCrop, "../data/küken128_crop.tga");

		kueken::image::creator Creator;
		Creator.setFormat(kueken::image::RGB8);
		Creator.setTarget(kueken::image::IMAGE2D);
		Creator.setGenerateMipmaps(true);
		for(std::size_t Level = 0; Level < ImageFile.levels(); ++Level)
		{
			Creator.setMipmap(
				Level, 
				ImageFile[Level].dimensions(),
				ImageFile[Level].data());
		}

		ImageDiffuse = Renderer->create(Creator);
	}


	return glf::checkError("initImage2D");
}

bool CMain::initTest()
{
	{
		kueken::test::creator Creator;
		Creator.setDepthEnable(true);
		Creator.setDepthFunc(kueken::test::LEQUAL);
		Test = Renderer->create(Creator);
	}

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

bool CMain::initSampler()
{
	{
		kueken::sampler::creator Creator;
		Creator.setFilter(kueken::sampler::TRILINEAR);
		Creator.setWrap(kueken::sampler::REPEAT, kueken::sampler::REPEAT, kueken::sampler::REPEAT);
		Creator.setAnisotropy(16.f);
		SamplerDiffuse = Renderer->create(Creator);
	}

	return glf::checkError("initSampler");
}

bool CMain::initTexture2D()
{
	{
		kueken::texture::creator<kueken::texture::image> Creator;
		Creator.setVariable(VariableDiffuse);
		Creator.setSampler(SamplerDiffuse);
		Creator.setImage(ImageDiffuse);
		TextureDiffuse = Renderer->create(Creator);
	}

	return glf::checkError("initTexture2D");
}

bool CMain::initTextureBuffer()
{
	assert(glGetError() == GL_NO_ERROR);

	{
		kueken::buffer::creator Creator;
		Creator.setSize(sizeof(glm::mat4) * 2);
		Creator.setData(0);
		Creator.setUsage(kueken::buffer::STREAM_READ);
		TextureBuffer = Renderer->create(Creator);
	}

	assert(glGetError() == GL_NO_ERROR);

	glf::checkError("initTextureBuffer 1");

	{
		assert(glGetError() == GL_NO_ERROR);

		kueken::texture::creator<kueken::texture::buffer> Creator;
		Creator.setVariable(VariableMVP);
		Creator.setBuffer(TextureBuffer);
		Creator.setFormat(kueken::texture::RGBA32F);
		
		assert(glGetError() == GL_NO_ERROR);

		TextureMVP = Renderer->create(Creator);
	}

	return glf::checkError("initTextureBuffer");
}

bool CMain::initProgram()
{
	{
		kueken::program::creator Creator;
		Creator.attachShader(
			kueken::program::VERTEX, 
			glf::loadFile(VERTEX_SHADER_SOURCE));
		Creator.attachShader(
			kueken::program::FRAGMENT, 
			glf::loadFile(FRAGMENT_SHADER_SOURCE));
		Program = Renderer->create(Creator);
	}

	{
		kueken::program::object* Object = Renderer->map(Program);
		VariablePosition = Object->get("Position", kueken::program::ATTRIB);
		VariableTexcoord = Object->get("Texcoord", kueken::program::ATTRIB);
		VariableDiffuse = Object->get("Diffuse", kueken::program::SAMPLER);
		VariableMVP = Object->get("MVP", kueken::program::UNIFORM);
		Renderer->unmap(Program);
	}

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

	return glf::checkError("initVertexArray");
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
	{
		kueken::rendertarget::creator<kueken::rendertarget::FRAMEBUFFER> Creator;
		Rendertarget = Renderer->create(Creator);
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
