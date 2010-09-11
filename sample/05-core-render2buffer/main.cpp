//**********************************
// Kueken sample 05: Render 2 buffer
// 02/04/2009
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
	const char* SAMPLE_NAME = "Kueken sample 05: Phong";	
	const char* VERTEX_SHADER_SOURCE1 = "../data/render2buffer-pass1.vert";
	const char* FRAGMENT_SHADER_SOURCE1 = "../data/render2buffer-pass1.frag";
	const char* VERTEX_SHADER_SOURCE2 = "../data/render2buffer-pass2.vert";
	const char* FRAGMENT_SHADER_SOURCE2 = "../data/render2buffer-pass2.frag";
	const char* TEXTURE_HEIGHTMAP = "../data/heightmap.tga";

	kueken::renderer* Renderer = 0;
	
	kueken::blend::name Blend;
	kueken::clear::name Clear;
	kueken::test::name Test;

	kueken::rasterizer::name RasterizerPass1;
	kueken::program::name ProgramPass1;
	kueken::program::variable VariablePositionPass1;
	kueken::program::variable VariableDiffusePass1;
	kueken::program::variable VariableMVPPass1;
	kueken::buffer::name ArrayBufferPass1;
	kueken::buffer::name ElementBufferPass1;
	kueken::draw::name DrawPass1;
	kueken::assembler::name AssemblerPass1;
	glv::mesh MeshPass1;

	kueken::rasterizer::name RasterizerPass2;
	kueken::program::name ProgramPass2;
	kueken::program::variable VariablePositionPass2;
	kueken::program::variable VariableDiffusePass2;
	kueken::program::variable VariableMVPPass2;
	kueken::buffer::name ArrayBufferPass2;
	kueken::buffer::name ElementBufferPass2;
	kueken::draw::name DrawPass2;
	kueken::assembler::name AssemblerPass2;

	kueken::sampler::name SamplerHeightmap;
	kueken::texture::name TextureHeightmap;

	kueken::rendertarget::name Rendertarget;

	kueken::readpixels::name ReadPixels;

	glm::ivec2 HeightmapSize(0);
	std::size_t HeightmapIndex(0);
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
		Result = initTest();
	if(Result)
		Result = initRendertarget();
	if(Result)
		Result = initProgram();
	if(Result)
		Result = initTexture2D();
	if(Result)
		Result = initReadPixels();
	if(Result)
		Result = initArrayBuffer();
	if(Result)
		Result = initAssembler();
	if(Result)
		Result = initRasterizer();
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
	Renderer->bind(Rendertarget, kueken::rendertarget::EXEC);
	Renderer->bind(Test);
	Renderer->bind(Blend);

	Renderer->exec(Clear);

	// Pass 1: create the vertex buffer
	{
		Renderer->bind(RasterizerPass1);

		Renderer->bind(0, kueken::program::UNIFIED, ProgramPass1);
		glm::mat4 Projection = glm::ortho(0.f, 1.f, 0.f, 1.f);
		glm::mat4 ModelView = glm::mat4(1.0f);
		glm::mat4 MVP = Projection * ModelView;
		VariableMVPPass1.set(MVP);
		VariableDiffusePass1.set(0);

		Renderer->bind(0, kueken::texture::IMAGE2D, TextureHeightmap);
		Renderer->bind(0, kueken::sampler::SAMPLER, SamplerHeightmap);

		Renderer->bind(AssemblerPass1);
		Renderer->exec(DrawPass1);
	}

	// Pass 2: Read the buffer and render as geometry
	{
		Renderer->bind(RasterizerPass2);

		Renderer->bind(0, kueken::program::UNIFIED, ProgramPass2);
		glm::mat4 Projection = glm::perspective(45.0f, float(windowSize.x) / windowSize.y, 0.1f, 100.0f);
		glm::mat4 ModelView(1.0f);
		ModelView = glm::translate(ModelView, glm::vec3(0.0f, 0.0f, -tranlationCurrent.y));
		ModelView = glm::rotate(ModelView, rotationCurrent.y, glm::vec3(-1.f, 0.f, 0.f));
		ModelView = glm::rotate(ModelView, rotationCurrent.x, glm::vec3( 0.f, 0.f, 1.f));
		ModelView = glm::translate(ModelView, glm::vec3( -0.5f,-0.5f,-0.5f));
		glm::mat4 MVP = Projection * ModelView;
		VariableMVPPass2.set(MVP);

		Renderer->bind(0, kueken::buffer::PIXEL_PACK, ArrayBufferPass2);
		Renderer->exec(ReadPixels);

		Renderer->bind(AssemblerPass2);
		Renderer->exec(DrawPass2);
	}

	glf::checkError("Render");
}

bool CMain::initBlend()
{
	{
		kueken::blend::creator Creator;
		Creator.setBlend(
			kueken::blend::SLOT0, false);
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
		Creator.setColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
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
		Creator.setCount(MeshPass1.elementCount());
		Creator.setInstances(1);
		DrawPass1 = Renderer->create(Creator);
	}

	{
		kueken::draw::creator Creator;
		Creator.setFirst(0);
		Creator.setCount(HeightmapIndex);
		Creator.setInstances(1);
		DrawPass2 = Renderer->create(Creator);
	}

	return glf::checkError("initDraw");
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
		Creator.setViewport(glm::ivec4(glm::ivec2(0), HeightmapSize));
		RasterizerPass1 = Renderer->create(Creator);
	}

	{
		kueken::rasterizer::creator<kueken::rasterizer::POLYGON> Creator;
		Creator.setViewport(glm::ivec4(glm::ivec2(0), windowSize));
		RasterizerPass2 = Renderer->create(Creator);
	}

	return glf::checkError("initRasterizer");
}

bool CMain::initReadPixels()
{
	{
		kueken::readpixels::creator Creator;
		Creator.setFormat(kueken::readpixels::RGBA8U);
		Creator.setRect(glm::uvec4(0, 0, HeightmapSize));
		ReadPixels = Renderer->create(Creator);
	}

	return glf::checkError("initReadPixels");
}

bool CMain::initTexture2D()
{
	{
		gli::image ImageFile = gli::import_as(TEXTURE_HEIGHTMAP);

		{
			kueken::texture::creator Creator;
			Creator.setFormat(kueken::texture::RGB8);
			Creator.setTarget(kueken::texture::IMAGE2D);
			for(std::size_t Level = 0; Level < ImageFile.levels(); ++Level)
			{
				Creator.setMipmap(
					Level, 
					ImageFile[Level].dimensions(),
					ImageFile[Level].data());
			}

			TextureHeightmap = Renderer->create(Creator);
			HeightmapSize = glm::ivec2(ImageFile[0].dimensions());
		}

		{
			kueken::sampler::creator Creator;
			Creator.setFilter(kueken::sampler::BILINEAR);
			Creator.setWrap(kueken::sampler::REPEAT, kueken::sampler::REPEAT, kueken::sampler::REPEAT);
			Creator.setAnisotropy(16.f);
			SamplerHeightmap = Renderer->create(Creator);
		}
	}

	return glf::checkError("initTexture2D");
}

bool CMain::initProgram()
{
	{
		kueken::program::creator Creator;
		Creator.attachShader(
			kueken::program::VERTEX, 
			glf::loadFile(VERTEX_SHADER_SOURCE1));
		Creator.attachShader(
			kueken::program::FRAGMENT, 
			glf::loadFile(FRAGMENT_SHADER_SOURCE1));
		ProgramPass1 = Renderer->create(Creator);

		kueken::program::object* Object = Renderer->map(ProgramPass1);
		VariablePositionPass1 = Object->get("Position", kueken::program::ATTRIB);
		VariableDiffusePass1 = Object->get("Diffuse", kueken::program::SAMPLER);
		VariableMVPPass1 = Object->get("MVP", kueken::program::UNIFORM);
		Renderer->unmap(ProgramPass1);
	}

	{
		kueken::program::creator Creator;
		Creator.attachShader(
			kueken::program::VERTEX, 
			glf::loadFile(VERTEX_SHADER_SOURCE2));
		Creator.attachShader(
			kueken::program::FRAGMENT, 
			glf::loadFile(FRAGMENT_SHADER_SOURCE2));
		ProgramPass2 = Renderer->create(Creator);

		kueken::program::object* Object = Renderer->map(ProgramPass2);
		VariablePositionPass2 = Object->get("Position", kueken::program::ATTRIB);
		VariableMVPPass2 = Object->get("MVP", kueken::program::UNIFORM);
		Renderer->unmap(ProgramPass2);
	}

	return glf::checkError("initProgram");
}

bool CMain::initAssembler()
{
	{
		kueken::assembler::creator Creator;

		Creator.setPrimitive(
			kueken::assembler::TRIANGLES);

		Creator.addAttribute(
			ArrayBufferPass1, 
			VariablePositionPass1, 
			kueken::assembler::F32VEC2,
			//MeshPass1.normalized(glv::POSITION2),
			MeshPass1.stride(glv::SLOT0),
			MeshPass1.offset(glv::POSITION2));

		Creator.setElements(
			kueken::assembler::U16, 
			ElementBufferPass1);

		AssemblerPass1 = Renderer->create(Creator);
	}

	{
		kueken::assembler::creator Creator;

		Creator.setPrimitive(
			kueken::assembler::TRIANGLES);

		Creator.addAttribute(
			ArrayBufferPass2, 
			VariablePositionPass2, 
			kueken::assembler::U8VEC4,
			//false,
			0,
			0);

		Creator.setElements(
			kueken::assembler::U32, 
			ElementBufferPass2);

		AssemblerPass2 = Renderer->create(Creator);
	}

	return glf::checkError("initVertexArray");
}

bool CMain::initArrayBuffer()
{
	MeshPass1 = glv::buildPlane(glv::ELEMENT_BIT | glv::POSITION2_BIT, glm::vec2(1.0f));

	{
		kueken::buffer::creator Creator;
		Creator.setSize(MeshPass1.vertexSize(glv::SLOT0));
		Creator.setData(MeshPass1.vertexData(glv::SLOT0));
		Creator.setUsage(kueken::buffer::STATIC_DRAW);
		ArrayBufferPass1 = Renderer->create(Creator);
	}

	{
		kueken::buffer::creator Creator;
		Creator.setSize(MeshPass1.elementSize());
		Creator.setData(MeshPass1.elementData());
		Creator.setUsage(kueken::buffer::STATIC_DRAW);
		ElementBufferPass1 = Renderer->create(Creator);
	}

	{
		kueken::buffer::creator Creator;
		Creator.setSize(HeightmapSize.x * HeightmapSize.y * sizeof(glm::u8vec4));
		Creator.setData(0);
		Creator.setUsage(kueken::buffer::DYNAMIC_DRAW);
		ArrayBufferPass2 = Renderer->create(Creator);
	}

	{
		std::vector<glm::uint> Indexes;

		for(int j = 0; j < HeightmapSize.y - 1; ++j)
		for(int i = 0; i < HeightmapSize.x - 1; ++i)
		{
			Indexes.push_back((i + 0) + (j + 0) * HeightmapSize.x);
			Indexes.push_back((i + 1) + (j + 0) * HeightmapSize.x);
			Indexes.push_back((i + 1) + (j + 1) * HeightmapSize.x);
			Indexes.push_back((i + 0) + (j + 0) * HeightmapSize.x);
			Indexes.push_back((i + 1) + (j + 1) * HeightmapSize.x);
			Indexes.push_back((i + 0) + (j + 1) * HeightmapSize.x);
		}

		HeightmapIndex = Indexes.size();

		kueken::buffer::creator Creator;
		Creator.setSize(HeightmapIndex * sizeof(glm::uint));
		Creator.setData(&Indexes[0]);
		Creator.setUsage(kueken::buffer::STATIC_DRAW);
		ElementBufferPass2 = Renderer->create(Creator);
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
