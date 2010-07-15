//**********************************
// Kueken sample 12: Blur
// 11/04/2009
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
	const char* SAMPLE_NAME = "Kueken sample 12: Blur";	
	const char* VERTEX_SHADER_SOURCE_BLUR = "../data/blur.vert";
	const char* FRAGMENT_SHADER_SOURCE_BLUR = "../data/blur.frag";
	const char* VERTEX_SHADER_SOURCE_TEX = "../data/texture.vert";
	const char* FRAGMENT_SHADER_SOURCE_TEX = "../data/texture.frag";
	const char* TEXTURE_DIFFUSE = "../data/küken256.tga";

	kueken::renderer* Renderer = 0;
	
	kueken::blend::name Blend;
	kueken::rasterizer::name Rasterizer;
	kueken::clear::name Clear;
	kueken::draw::name Draw;
	kueken::buffer::name ArrayBuffer;
	kueken::buffer::name UniformBuffer;
	kueken::test::name Test;
	kueken::query::name Query;

	kueken::rendertarget::name Rendertarget;
	kueken::rendertarget::name RendertargetBlurH;
	kueken::rendertarget::name RendertargetBlurV;

	kueken::sampler::name Sampler;

	kueken::image::name ImageDiffuse;
	kueken::image::name ImageBlurredH;
	kueken::image::name ImageBlurred;

	kueken::program::name ProgramTex;
	kueken::program::variable VariableTexPosition;
	kueken::program::variable VariableTexTexcoord;
	kueken::program::variable VariableTexDiffuse;
	kueken::program::variable VariableTexMVP;
	//kueken::texture::name TextureTexDiffuse;
	kueken::assembler::name AssemblerTex;

	kueken::program::name ProgramBlur;
	kueken::program::variable VariableBlurPosition;
	kueken::program::variable VariableBlurTexcoord;
	kueken::program::variable VariableBlurDiffuse;
	kueken::program::variable VariableBlurMVP;
	kueken::program::variable VariableBlurWeight;
	kueken::program::variable VariableBlurOffset;
	//kueken::texture::name TextureBlurredH;
	//kueken::texture::name TextureBlurred;
	kueken::assembler::name AssemblerBlur;

	struct blur
	{
		glm::vec2 Offset[16];
		float Weight[16];
	};

	blur Blur[2];
}

const float BLUR_SIZE = 3.f;

float gaussianDistribution(float x, float y, float rho)
{
	rho *= rho;
	float g = 1.0f / sqrt(2.0f * 3.14159f * rho);
	return g * exp((x * x + y * y) / (-2.0f * rho) );
}

void getGaussianOffsets(const float texelsz[2], glm::vec2 offset[16], float weight[16], bool bHorizontal)
{
	// Get the center texel offset and weight
	weight[0] = 1.0f * gaussianDistribution(0.0f, 0.f, BLUR_SIZE);
	offset[0][0] = offset[0][1] = 0.0f;

	// Get the offsets and weights for the remaining taps
	if ( bHorizontal )
	{
		for( int i = 1; i < 15; i += 2 )
		{
			offset[i+0][0] = float(i) * texelsz[0];
			offset[i+0][1] = 0.0f;
			weight[i+0] = 2.0f * gaussianDistribution(i, 0.0f, BLUR_SIZE);
			offset[i+1][0] = float(-i) * texelsz[0];
			offset[i+1][1] = 0.0f;
			weight[i+1] = 2.0f * gaussianDistribution(i + 1.0f, 0.0f, BLUR_SIZE);
		}
	} else 
	{
		for( int i = 1; i < 15; i += 2 )
		{
			offset[i+0][0] = 0.0f;
			offset[i+0][1] = float(i) * texelsz[1];
			weight[i+0] = 2.0f * gaussianDistribution(0.0f, i, BLUR_SIZE);
			offset[i+1][0] = 0.0f;
			offset[i+1][1] = float(-i) * texelsz[1];
			weight[i+1] = 2.0f * gaussianDistribution(0.0f, i + 1.0f, BLUR_SIZE);
		}
	}
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

	float texel_size[2] = { 1.0f / 256.f, 1.0f / 256.f }; 
	getGaussianOffsets(texel_size, Blur[0].Offset, Blur[0].Weight, true);
	getGaussianOffsets(texel_size, Blur[1].Offset, Blur[1].Weight, false);

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
	glm::mat4 ViewTranslateZ = glm::translate(glm::mat4(1.0f), 0.0f, 0.0f, -tranlationCurrent.y);
	glm::mat4 ViewRotateX = glm::rotate(ViewTranslateZ, rotationCurrent.y,-1.f, 0.f, 0.f);
	glm::mat4 ViewRotateY = glm::rotate(ViewRotateX, rotationCurrent.x, 0.f, 1.f, 0.f);
	glm::mat4 View = ViewRotateY;
	glm::mat4 Model = glm::mat4(1.0f);
 
	Renderer->bind(Test);
	Renderer->bind(Blend);
	Renderer->bind(Rasterizer);

	{
		Renderer->bind(0, kueken::program::UNIFIED, ProgramBlur);

		glm::mat4 Projection = glm::ortho(0.0f, float(), 0.0f, float(),-1.0f, 1.0f);//glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		glm::mat4 MVP = Projection * View * Model;
		VariableBlurMVP.set(MVP);

		//Renderer->bind(RendertargetBlurH, kueken::rendertarget::EXEC);
		Renderer->bind(Rendertarget, kueken::rendertarget::EXEC);
		VariableBlurWeight.set(Blur[0].Weight, 16);
		VariableBlurOffset.set(Blur[0].Offset, 16);
		Renderer->bind(0, kueken::image::IMAGE2D, ImageDiffuse);
		Renderer->bind(0, kueken::sampler::SAMPLER, Sampler);
		VariableTexDiffuse.set(0);

		Renderer->bind(AssemblerBlur);

		assert(glGetError() == GL_NO_ERROR);

		Renderer->exec(Draw);

		assert(glGetError() == GL_NO_ERROR);
		//Renderer->bind(RendertargetBlurV, kueken::rendertarget::EXEC);
		Renderer->bind(Rendertarget, kueken::rendertarget::EXEC);
		VariableBlurWeight.set(Blur[1].Weight, 16);
		VariableBlurOffset.set(Blur[1].Offset, 16);
		Renderer->bind(0, kueken::image::IMAGE2D, ImageBlurredH);
		Renderer->bind(0, kueken::sampler::SAMPLER, Sampler);
		VariableBlurDiffuse.set(0);
		
		assert(glGetError() == GL_NO_ERROR);
		Renderer->bind(AssemblerBlur);
		assert(glGetError() == GL_NO_ERROR);
		Renderer->exec(Draw);
		assert(glGetError() == GL_NO_ERROR);
	}

	{
		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		glm::mat4 MVP = Projection * View * Model;

		VariableTexMVP.set(MVP);

		assert(glGetError() == GL_NO_ERROR);
		Renderer->bind(Rendertarget, kueken::rendertarget::EXEC);
		assert(glGetError() == GL_NO_ERROR);
		Renderer->exec(Clear);

		assert(glGetError() == GL_NO_ERROR);

		Renderer->bind(0, kueken::program::UNIFIED, ProgramTex);
		Renderer->bind(0, kueken::image::IMAGE2D, ImageBlurred);
		Renderer->bind(0, kueken::sampler::SAMPLER, Sampler);
		VariableBlurDiffuse.set(0);

		assert(glGetError() == GL_NO_ERROR);

		Renderer->bind(AssemblerTex);
		Renderer->exec(Draw);
	}

	glf::checkError("Render");
}

bool CMain::initBlend()
{
	kueken::blend::creator Creator;
	Creator.setColorMask(kueken::blend::SLOT0, glm::bvec4(true, true, true, true));
	Blend = Renderer->create(Creator);

	return glf::checkError("initBlend");
}

bool CMain::initClear()
{
	kueken::clear::creator Creator;
	Creator.setColor(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
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
		Creator.setViewport(glm::ivec4(0, 0, windowSize));
		Creator.setScissor(false, glm::ivec4(0));
		Rasterizer = Renderer->create(Creator);
	}

	return glf::checkError("initRasterizer");
}

bool CMain::initTexture2D()
{
	gli::image ImageFile = gli::import_as(TEXTURE_DIFFUSE);

	{
		kueken::sampler::creator Creator;
		Creator.setFilter(kueken::sampler::TRILINEAR);
		Creator.setWrap(kueken::sampler::REPEAT, kueken::sampler::REPEAT, kueken::sampler::REPEAT);
		Creator.setAnisotropy(16.f);
		Sampler = Renderer->create(Creator);
	}

	{
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
		Creator.setMipmap(0, ImageFile[0].dimensions(), 0);
		ImageBlurredH = Renderer->create(Creator);
	}

	{
		kueken::image::creator Creator;
		Creator.setFormat(kueken::image::RGB8);
		Creator.setTarget(kueken::image::IMAGE2D);
		Creator.setMipmap(0, ImageFile[0].dimensions(), 0);
		ImageBlurred = Renderer->create(Creator);
	}

	//{
	//	kueken::texture::creator<kueken::texture::image> Creator;
	//	Creator.setVariable(VariableTexDiffuse);
	//	Creator.setSampler(Sampler);
	//	Creator.setImage(ImageDiffuse);
	//	TextureTexDiffuse = Renderer->create(Creator);
	//}

	//{
	//	kueken::texture::creator<kueken::texture::image> Creator;
	//	Creator.setVariable(VariableBlurDiffuse);
	//	Creator.setSampler(Sampler);
	//	Creator.setImage(ImageBlurredH);
	//	TextureBlurredH = Renderer->create(Creator);
	//}

	//{
	//	kueken::texture::creator<kueken::texture::image> Creator;
	//	Creator.setVariable(VariableBlurDiffuse);
	//	Creator.setSampler(Sampler);
	//	Creator.setImage(ImageBlurred);
	//	TextureBlurred = Renderer->create(Creator);
	//}

	return glf::checkError("initTexture2D");
}

bool CMain::initProgram()
{
	{
		kueken::program::creator Creator;
		Creator.attachShader(kueken::program::VERTEX, glf::loadFile(VERTEX_SHADER_SOURCE_BLUR));
		Creator.attachShader(kueken::program::FRAGMENT, glf::loadFile(FRAGMENT_SHADER_SOURCE_BLUR));
		ProgramBlur = Renderer->create(Creator);
	}

	{
		kueken::program::object* Object = Renderer->map(ProgramBlur);
		VariableBlurMVP = Object->get("MVP", kueken::program::UNIFORM);
		VariableBlurWeight = Object->get("Weight", kueken::program::UNIFORM);
		VariableBlurOffset = Object->get("Offset", kueken::program::UNIFORM);
		VariableBlurPosition = Object->get("Position", kueken::program::ATTRIB);
		VariableBlurTexcoord = Object->get("Texcoord", kueken::program::ATTRIB);
		VariableBlurDiffuse = Object->get("Diffuse", kueken::program::SAMPLER);
		Renderer->unmap(ProgramBlur);
	}

	{
		kueken::program::creator Creator;
		Creator.attachShader(kueken::program::VERTEX, glf::loadFile(VERTEX_SHADER_SOURCE_TEX));
		Creator.attachShader(kueken::program::FRAGMENT, glf::loadFile(FRAGMENT_SHADER_SOURCE_TEX));
		ProgramTex = Renderer->create(Creator);
	}

	{
		kueken::program::object* Object = Renderer->map(ProgramTex);
		VariableTexMVP = Object->get("MVP", kueken::program::UNIFORM);
		VariableTexPosition = Object->get("Position", kueken::program::ATTRIB);
		VariableTexTexcoord = Object->get("Texcoord", kueken::program::ATTRIB);
		VariableTexDiffuse = Object->get("Diffuse", kueken::program::SAMPLER);
		Renderer->unmap(ProgramTex);
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
			VariableTexPosition, 
			kueken::assembler::F32VEC2,
			mesh.stride(glv::SLOT0),
			mesh.offset(glv::POSITION2));
		Creator.addAttribute(
			ArrayBuffer, 
			VariableTexTexcoord, 
			kueken::assembler::F32VEC2,
			mesh.stride(glv::SLOT0),
			mesh.offset(glv::TEXCOORD));

		AssemblerTex = Renderer->create(Creator);
	}

	{
		kueken::assembler::creator Creator;
		Creator.setPrimitive(kueken::assembler::TRIANGLES);
		Creator.addAttribute(
			ArrayBuffer, 
			VariableBlurPosition, 
			kueken::assembler::F32VEC2,
			mesh.stride(glv::SLOT0),
			mesh.offset(glv::POSITION2));
		Creator.addAttribute(
			ArrayBuffer, 
			VariableBlurTexcoord, 
			kueken::assembler::F32VEC2,
			mesh.stride(glv::SLOT0),
			mesh.offset(glv::TEXCOORD));

		AssemblerBlur = Renderer->create(Creator);
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
	{
		kueken::rendertarget::creator<kueken::rendertarget::CUSTOM> Creator;
		Creator.setFramebuffer();
		Rendertarget = Renderer->create(Creator);
	}

	{
		kueken::rendertarget::creator<kueken::rendertarget::CUSTOM> Creator;
		Creator.setImage(kueken::rendertarget::COLOR0, ImageBlurredH, 0);
		RendertargetBlurH = Renderer->create(Creator);
	}

	{
		kueken::rendertarget::creator<kueken::rendertarget::CUSTOM> Creator;
		Creator.setImage(kueken::rendertarget::COLOR0, ImageBlurred, 0);
		RendertargetBlurV = Renderer->create(Creator);
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
