#ifndef GLF_MAIN_INCLUDED
#define GLF_MAIN_INCLUDED

#include "base.hpp"
#include <core/kueken.hpp>
#include <fx/effect.hpp>

class sample : public glf::base
{
public:
	sample(std::string const & Name, glm::ivec2 const & WindowSize);
	virtual ~sample();

	virtual bool check() const;
    virtual bool begin(glm::ivec2 const & WindowSize);
    virtual bool end();
    virtual void render();

private:
	bool initArrayBuffer();
	bool initAssembler();
	bool initBlend();
	bool initClear();
	bool initDraw();
	bool initLayout();
	bool initProgram();
	bool initQuery();
	bool initRendertarget();
	bool initRasterizer();
	bool initTest();
	bool initTexture2D();

	glv::mesh mesh;
};

#endif//GLF_MAIN_INCLUDED
