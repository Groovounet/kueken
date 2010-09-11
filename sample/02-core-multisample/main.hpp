#ifndef GLF_MAIN_INCLUDED
#define GLF_MAIN_INCLUDED

#include "base.hpp"
#include <core/kueken.hpp>

class CMain : public glf::IBase
{
public:
	CMain(std::string const & Name, glm::ivec2 const & WindowSize);
	virtual ~CMain();

	virtual bool Check() const;
    virtual bool Begin(const glm::ivec2& WindowSize);
    virtual bool End();
    virtual void Render();

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

	glv::mesh mesh;
};

#endif//GLF_MAIN_INCLUDED
