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

	GLsizei const VertexCount = 4;
	GLsizeiptr const VertexSize = VertexCount * sizeof(vertex_v2fv2f);
	vertex_v2fv2f const VertexData[VertexCount] =
	{
		vertex_v2fv2f(glm::vec2(-1.0f,-1.0f), glm::vec2(0.0f, 1.0f)),
		vertex_v2fv2f(glm::vec2( 1.0f,-1.0f), glm::vec2(1.0f, 1.0f)),
		vertex_v2fv2f(glm::vec2( 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
		vertex_v2fv2f(glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 0.0f))
	};

	GLsizei const ElementCount = 6;
	GLsizeiptr const ElementSize = ElementCount * sizeof(GLuint);
	GLuint const ElementData[ElementCount] =
	{
		0, 1, 2, 
		2, 3, 0
	};

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
	kueken::buffer::name ElementBuffer;
	kueken::test::name Test;
	kueken::assembler::name Assembler;
	kueken::rendertarget::name Rendertarget;
	kueken::query::name Query;

	kueken::program::semantic const SEMANTIC_DIFFUSE = 0;
	kueken::program::semantic const SEMANTIC_MVP = 1;

	kueken::program::semantic const SEMANTIC_POSITION = 0;
	kueken::program::semantic const SEMANTIC_TEXCOORD = 4;

	namespace buffer
	{
		enum type
		{
			VERTEX,
			ELEMENT,
			MAX
		};
	}//namespace buffer

	GLuint VertexArrayName = 0;
}

bool initVertexArray()
{
	glGenVertexArrays(1, &VertexArrayName);
    glBindVertexArray(VertexArrayName);

	glBindVertexArray(0);

	return glf::checkError("initVertexArray");
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
		initVertexArray();
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
	//if(Result)
	//	Result = initAssembler();
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
	static float Rotate = 0.0f;
	//Rotate += 0.01f;

    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 ViewTranslateZ = glm::translate(glm::mat4(1.0f), 0.0f, 0.0f, -4.0f);//-tranlationCurrent.y);
	glm::mat4 ViewRotateX = glm::rotate(ViewTranslateZ, rotationCurrent.y,-1.f, 0.f, 0.f);
	glm::mat4 ViewRotateY = glm::rotate(ViewRotateX, rotationCurrent.x, 0.f, 1.f, 0.f);
	glm::mat4 ViewRotateZ = glm::rotate(ViewRotateY, Rotate, 0.f, 0.f, 1.f);
	glm::mat4 View = ViewRotateZ;
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Projection * View * Model;
 
	kueken::program::object * Object = Renderer->map(Program);
	Object->setSampler(SEMANTIC_DIFFUSE, 0);
	Object->setUniform(SEMANTIC_MVP, MVP);
	Renderer->unmap(Program);

	Renderer->bind(Rendertarget, kueken::rendertarget::EXEC);

	Renderer->bind(RasterizerBackground);
	Renderer->exec(ClearBackground);

	Renderer->bind(RasterizerScene);
	Renderer->exec(ClearScene);

	Renderer->bind(Test);
	Renderer->bind(Blend);
	
	Renderer->bind(0, kueken::program::UNIFIED, Program);
	
	Renderer->bind(0, kueken::texture::IMAGE2D, Texture);
	Renderer->bind(0, kueken::sampler::SAMPLER, Sampler);

	//kueken::draw::creator Creator;
	//Creator.setPrimitives(kueken::draw::TRIANGLES);
	//Creator.setPrimitives(kueken::draw::TRIANGLES);
	//Creator.setLayout(0, kueken::draw::VERTEX, Layout);
	//Creator.setBuffer(0, kueken::buffer::ARRAY, ArrayBuffer);
	//Creator.setBuffer(1, kueken::buffer::ARRAY, ArrayBuffer);
	//Creator.setBuffer(0, kueken::buffer::ELEMENT, ElementBuffer);
	//Object->setBuffer(0, kueken::buffer::INDIRECT, IndirectBuffer);

	Renderer->bind(0, kueken::buffer::ELEMENT, ElementBuffer);
	Renderer->bind(0, kueken::buffer::ARRAY, ArrayBuffer);
	Renderer->bind(0, kueken::layout::VERTEX, Layout);

	glDrawElementsInstancedBaseVertex(GL_TRIANGLES, ElementCount, GL_UNSIGNED_INT, 0, 1, 0);
	//Renderer->exec(Draw);

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
			true, glm::ivec4(glm::ivec2(8), windowSize - glm::ivec2(16)));
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
	Creator.addVariable(
		SEMANTIC_DIFFUSE, 
		"Diffuse");
	Creator.addVariable(
		SEMANTIC_MVP, 
		"MVP");

	Creator.build();
	Program = Renderer->create(Creator);

	return glf::checkError("initProgram");
}

bool sample::initLayout()
{
	kueken::layout::creator Creator;
	Creator.setVertexArray(
		0, 
		kueken::layout::POSITION,
		kueken::layout::F32VEC2,
		sizeof(vertex_v2fv2f),
		0, 
		0);
	Creator.setVertexArray(
		0, 
		kueken::layout::TEXCOORD,
		kueken::layout::F32VEC2,
		sizeof(vertex_v2fv2f),
		sizeof(glm::vec2), 
		0);
	Layout = Renderer->create(Creator);

	return glf::checkError("initLayout");
}

bool sample::initAssembler()
{
	kueken::assembler::creator Creator;
	Creator.setPrimitive(kueken::assembler::TRIANGLES);
	Creator.addAttribute(
		ArrayBuffer, 
		SEMANTIC_POSITION, 
		kueken::assembler::F32VEC2,
		Mesh.stride(glv::SLOT0),
		Mesh.offset(glv::POSITION2));
	Creator.addAttribute(
		ArrayBuffer, 
		SEMANTIC_TEXCOORD, 
		kueken::assembler::F32VEC2,
		Mesh.stride(glv::SLOT0),
		Mesh.offset(glv::TEXCOORD));

	Assembler = Renderer->create(Creator);

	return glf::checkError("initAssembler");
}

/*
bool sample::initAssembler()
{
	kueken::assembler::creator Creator;
	Creator.setPrimitive(kueken::assembler::TRIANGLES);
	Creator.addAttribute(
		ArrayBuffer, 
		SEMANTIC_POSITION, 
		kueken::assembler::F32VEC2,
		Mesh.stride(glv::SLOT0),
		Mesh.offset(glv::POSITION2));
	Creator.addAttribute(
		ArrayBuffer, 
		SEMANTIC_TEXCOORD, 
		kueken::assembler::F32VEC2,
		Mesh.stride(glv::SLOT0),
		Mesh.offset(glv::TEXCOORD));

	Assembler = Renderer->create(Creator);

	return glf::checkError("initAssembler");
}
*/
/*
bool sample::initArrayBuffer()
{
	Mesh = glv::buildPlane(glv::POSITION2_BIT | glv::TEXCOORD_BIT | glv::ELEMENT_BIT, glm::vec2(1.0f));

	{
		kueken::buffer::creator Creator;
		Creator.setSize(Mesh.vertexSize(glv::SLOT0));
		Creator.setData(Mesh.vertexData(glv::SLOT0));
		Creator.setUsage(kueken::buffer::STATIC_DRAW);
		ArrayBuffer = Renderer->create(Creator);
	}

	{
		kueken::buffer::creator Creator;
		Creator.setSize(Mesh.elementSize());
		Creator.setData(Mesh.elementData());
		Creator.setUsage(kueken::buffer::STATIC_DRAW);
		ElementBuffer = Renderer->create(Creator);
	}

	return glf::checkError("initArrayBuffer");
}
*/

bool sample::initArrayBuffer()
{
	{
		kueken::buffer::creator Creator;
		Creator.setSize(VertexSize);
		Creator.setData(VertexData);
		Creator.setUsage(kueken::buffer::STATIC_DRAW);
		ArrayBuffer = Renderer->create(Creator);
	}

	{
		kueken::buffer::creator Creator;
		Creator.setSize(ElementSize);
		Creator.setData(ElementData);
		Creator.setUsage(kueken::buffer::STATIC_DRAW);
		ElementBuffer = Renderer->create(Creator);
	}

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
