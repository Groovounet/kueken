#ifndef GLF_MAIN_INCLUDED
#define GLF_MAIN_INCLUDED

#include "base.hpp"
#include <core/kueken.hpp>

struct vertex_v2fv2f
{
	vertex_v2fv2f
	(
		glm::vec2 const & Position,
		glm::vec2 const & Texcoord
	) :
		Position(Position),
		Texcoord(Texcoord)
	{}

	glm::vec2 Position;
	glm::vec2 Texcoord;
};

class sample : public glf::base
{
public:
	sample(std::string const & Name, glm::ivec2 const & WindowSize);
	virtual ~sample();

	virtual bool check() const;
    virtual bool begin(const glm::ivec2& WindowSize);
    virtual bool end();
    virtual void render();

private:
	bool initArrayBuffer();
	bool initAssembler();
	bool initBlend();
	bool initBlit();
	bool initClear();
	bool initDraw();
	bool initProgram();
	bool initUniformBuffer();
	bool initRendertarget();
	bool initRasterizer();
	bool initSampler();
	bool initTest();
	bool initTexture2D();
};

#endif//GLF_MAIN_INCLUDED
