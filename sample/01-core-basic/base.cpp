#include "base.hpp"
#include <SDL.h>
#include <SDL_syswm.h>
//#include "GL/wglew.h"
//#include <windows.hpp>

namespace 
{
	int const WINDOW_WIDTH = 640;
	int const WINDOW_HEIGHT = 480;
	bool const WINDOW_FULLSCREEN = false;

	SDL_Surface* Surface = 0;

	SDL_Surface* CreateWindowSDL(const char* Name, int Width, int Height, bool Fullscreen)
	{
		if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0)
			return NULL;

		unsigned int VideoFlags = SDL_OPENGL | SDL_DOUBLEBUF;
		if(Fullscreen)
			VideoFlags |= SDL_FULLSCREEN;

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 0);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 0);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 0);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		if((Surface = SDL_SetVideoMode(Width, Height, 32, VideoFlags)) == 0)
			return NULL;

		SDL_WM_SetCaption(Name, Name);

		glewInit();
		glf::checkError("");


/*
typedef void (GLAPIENTRY * PFNGLBINDMULTITEXTUREEXTPROC) (GLenum texunit, GLenum target, GLuint texture);
typedef GLenum (GLAPIENTRY * PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC) (GLuint framebuffer, GLenum target);
typedef void (GLAPIENTRY * PFNGLCLIENTATTRIBDEFAULTEXTPROC) (GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLDISABLECLIENTSTATEINDEXEDEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLDISABLECLIENTSTATEIEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXARRAYATTRIBEXTPROC) (GLuint vaobj, GLuint index);
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXARRAYEXTPROC) (GLuint vaobj, GLenum array);
typedef void (GLAPIENTRY * PFNGLENABLECLIENTSTATEINDEXEDEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLECLIENTSTATEIEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXARRAYATTRIBEXTPROC) (GLuint vaobj, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXARRAYEXTPROC) (GLuint vaobj, GLenum array);
typedef void (GLAPIENTRY * PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC) (GLuint framebuffer, GLenum mode);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC) (GLuint framebuffer, GLsizei n, const GLenum* bufs);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERREADBUFFEREXTPROC) (GLuint framebuffer, GLenum mode);
typedef void (GLAPIENTRY * PFNGLGENERATEMULTITEXMIPMAPEXTPROC) (GLenum texunit, GLenum target);
typedef void (GLAPIENTRY * PFNGLGENERATETEXTUREMIPMAPEXTPROC) (GLuint texture, GLenum target);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDMULTITEXIMAGEEXTPROC) (GLenum texunit, GLenum target, GLint level, void* img);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC) (GLuint texture, GLenum target, GLint level, void* img);
typedef void (GLAPIENTRY * PFNGLGETDOUBLEINDEXEDVEXTPROC) (GLenum target, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETDOUBLEI_VEXTPROC) (GLenum pname, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETFLOATINDEXEDVEXTPROC) (GLenum target, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETFLOATI_VEXTPROC) (GLenum pname, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXENVFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXENVIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXGENDVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXGENFVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXGENIVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXIMAGEEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXLEVELPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXLEVELPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERIIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERIUIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC) (GLuint buffer, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERPOINTERVEXTPROC) (GLuint buffer, GLenum pname, void** params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, void* data);
typedef void (GLAPIENTRY * PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXTPROC) (GLuint program, GLenum target, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXTPROC) (GLuint program, GLenum target, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMSTRINGEXTPROC) (GLuint program, GLenum target, GLenum pname, void* string);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMIVEXTPROC) (GLuint program, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC) (GLuint renderbuffer, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETPOINTERINDEXEDVEXTPROC) (GLenum target, GLuint index, GLvoid** params);
typedef void (GLAPIENTRY * PFNGLGETPOINTERI_VEXTPROC) (GLenum pname, GLuint index, GLvoid** params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREIMAGEEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
typedef void (GLAPIENTRY * PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIUIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYINTEGERI_VEXTPROC) (GLuint vaobj, GLuint index, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYINTEGERVEXTPROC) (GLuint vaobj, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYPOINTERI_VEXTPROC) (GLuint vaobj, GLuint index, GLenum pname, GLvoid** param);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYPOINTERVEXTPROC) (GLuint vaobj, GLenum pname, GLvoid** param);
typedef GLvoid * (GLAPIENTRY * PFNGLMAPNAMEDBUFFEREXTPROC) (GLuint buffer, GLenum access);
typedef GLvoid * (GLAPIENTRY * PFNGLMAPNAMEDBUFFERRANGEEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (GLAPIENTRY * PFNGLMATRIXFRUSTUMEXTPROC) (GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADIDENTITYEXTPROC) (GLenum matrixMode);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADTRANSPOSEDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADTRANSPOSEFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTTRANSPOSEDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTTRANSPOSEFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void (GLAPIENTRY * PFNGLMATRIXORTHOEXTPROC) (GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
typedef void (GLAPIENTRY * PFNGLMATRIXPOPEXTPROC) (GLenum matrixMode);
typedef void (GLAPIENTRY * PFNGLMATRIXPUSHEXTPROC) (GLenum matrixMode);
typedef void (GLAPIENTRY * PFNGLMATRIXROTATEDEXTPROC) (GLenum matrixMode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLMATRIXROTATEFEXTPROC) (GLenum matrixMode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLMATRIXSCALEDEXTPROC) (GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLMATRIXSCALEFEXTPROC) (GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLMATRIXTRANSLATEDEXTPROC) (GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLMATRIXTRANSLATEFEXTPROC) (GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLMULTITEXBUFFEREXTPROC) (GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDPOINTEREXTPROC) (GLenum texunit, GLint size, GLenum type, GLsizei stride, const void* pointer);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVFEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVIEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENDEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLdouble param);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENDVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENFEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENFVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENIEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENIVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIUIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERFEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLfloat* param);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* param);
typedef void (GLAPIENTRY * PFNGLMULTITEXRENDERBUFFEREXTPROC) (GLenum texunit, GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERDATAEXTPROC) (GLuint buffer, GLsizeiptr size, const void* data, GLenum usage);
typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data);
typedef void (GLAPIENTRY * PFNGLNAMEDCOPYBUFFERSUBDATAEXTPROC) (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC) (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXTPROC) (GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXTPROC) (GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXTPROC) (GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXTPROC) (GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMSTRINGEXTPROC) (GLuint program, GLenum target, GLenum format, GLsizei len, const void* string);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC) (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC) (GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1FEXTPROC) (GLuint program, GLint location, GLfloat v0);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1IEXTPROC) (GLuint program, GLint location, GLint v0);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UIEXTPROC) (GLuint program, GLint location, GLuint v0);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPUSHCLIENTATTRIBDEFAULTEXTPROC) (GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLTEXTUREBUFFEREXTPROC) (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIUIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERFEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLfloat* param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLint* param);
typedef void (GLAPIENTRY * PFNGLTEXTURERENDERBUFFEREXTPROC) (GLuint texture, GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
typedef GLboolean (GLAPIENTRY * PFNGLUNMAPNAMEDBUFFEREXTPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYCOLOROFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYEDGEFLAGOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYFOGCOORDOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYINDEXOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYMULTITEXCOORDOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYNORMALOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYSECONDARYCOLOROFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYTEXCOORDOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBIOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);

#define glBindMultiTextureEXT GLEW_GET_FUN(__glewBindMultiTextureEXT)
#define glCheckNamedFramebufferStatusEXT GLEW_GET_FUN(__glewCheckNamedFramebufferStatusEXT)
#define glClientAttribDefaultEXT GLEW_GET_FUN(__glewClientAttribDefaultEXT)
#define glCompressedMultiTexImage1DEXT GLEW_GET_FUN(__glewCompressedMultiTexImage1DEXT)
#define glCompressedMultiTexImage2DEXT GLEW_GET_FUN(__glewCompressedMultiTexImage2DEXT)
#define glCompressedMultiTexImage3DEXT GLEW_GET_FUN(__glewCompressedMultiTexImage3DEXT)
#define glCompressedMultiTexSubImage1DEXT GLEW_GET_FUN(__glewCompressedMultiTexSubImage1DEXT)
#define glCompressedMultiTexSubImage2DEXT GLEW_GET_FUN(__glewCompressedMultiTexSubImage2DEXT)
#define glCompressedMultiTexSubImage3DEXT GLEW_GET_FUN(__glewCompressedMultiTexSubImage3DEXT)
#define glCompressedTextureImage1DEXT GLEW_GET_FUN(__glewCompressedTextureImage1DEXT)
#define glCompressedTextureImage2DEXT GLEW_GET_FUN(__glewCompressedTextureImage2DEXT)
#define glCompressedTextureImage3DEXT GLEW_GET_FUN(__glewCompressedTextureImage3DEXT)
#define glCompressedTextureSubImage1DEXT GLEW_GET_FUN(__glewCompressedTextureSubImage1DEXT)
#define glCompressedTextureSubImage2DEXT GLEW_GET_FUN(__glewCompressedTextureSubImage2DEXT)
#define glCompressedTextureSubImage3DEXT GLEW_GET_FUN(__glewCompressedTextureSubImage3DEXT)
#define glCopyMultiTexImage1DEXT GLEW_GET_FUN(__glewCopyMultiTexImage1DEXT)
#define glCopyMultiTexImage2DEXT GLEW_GET_FUN(__glewCopyMultiTexImage2DEXT)
#define glCopyMultiTexSubImage1DEXT GLEW_GET_FUN(__glewCopyMultiTexSubImage1DEXT)
#define glCopyMultiTexSubImage2DEXT GLEW_GET_FUN(__glewCopyMultiTexSubImage2DEXT)
#define glCopyMultiTexSubImage3DEXT GLEW_GET_FUN(__glewCopyMultiTexSubImage3DEXT)
#define glCopyTextureImage1DEXT GLEW_GET_FUN(__glewCopyTextureImage1DEXT)
#define glCopyTextureImage2DEXT GLEW_GET_FUN(__glewCopyTextureImage2DEXT)
#define glCopyTextureSubImage1DEXT GLEW_GET_FUN(__glewCopyTextureSubImage1DEXT)
#define glCopyTextureSubImage2DEXT GLEW_GET_FUN(__glewCopyTextureSubImage2DEXT)
#define glCopyTextureSubImage3DEXT GLEW_GET_FUN(__glewCopyTextureSubImage3DEXT)
#define glDisableClientStateIndexedEXT GLEW_GET_FUN(__glewDisableClientStateIndexedEXT)
#define glDisableClientStateiEXT GLEW_GET_FUN(__glewDisableClientStateiEXT)
#define glDisableVertexArrayAttribEXT GLEW_GET_FUN(__glewDisableVertexArrayAttribEXT)
#define glDisableVertexArrayEXT GLEW_GET_FUN(__glewDisableVertexArrayEXT)
#define glEnableClientStateIndexedEXT GLEW_GET_FUN(__glewEnableClientStateIndexedEXT)
#define glEnableClientStateiEXT GLEW_GET_FUN(__glewEnableClientStateiEXT)
#define glEnableVertexArrayAttribEXT GLEW_GET_FUN(__glewEnableVertexArrayAttribEXT)
#define glEnableVertexArrayEXT GLEW_GET_FUN(__glewEnableVertexArrayEXT)
#define glFlushMappedNamedBufferRangeEXT GLEW_GET_FUN(__glewFlushMappedNamedBufferRangeEXT)
#define glFramebufferDrawBufferEXT GLEW_GET_FUN(__glewFramebufferDrawBufferEXT)
#define glFramebufferDrawBuffersEXT GLEW_GET_FUN(__glewFramebufferDrawBuffersEXT)
#define glFramebufferReadBufferEXT GLEW_GET_FUN(__glewFramebufferReadBufferEXT)
#define glGenerateMultiTexMipmapEXT GLEW_GET_FUN(__glewGenerateMultiTexMipmapEXT)
#define glGenerateTextureMipmapEXT GLEW_GET_FUN(__glewGenerateTextureMipmapEXT)
#define glGetCompressedMultiTexImageEXT GLEW_GET_FUN(__glewGetCompressedMultiTexImageEXT)
#define glGetCompressedTextureImageEXT GLEW_GET_FUN(__glewGetCompressedTextureImageEXT)
#define glGetDoubleIndexedvEXT GLEW_GET_FUN(__glewGetDoubleIndexedvEXT)
#define glGetDoublei_vEXT GLEW_GET_FUN(__glewGetDoublei_vEXT)
#define glGetFloatIndexedvEXT GLEW_GET_FUN(__glewGetFloatIndexedvEXT)
#define glGetFloati_vEXT GLEW_GET_FUN(__glewGetFloati_vEXT)
#define glGetFramebufferParameterivEXT GLEW_GET_FUN(__glewGetFramebufferParameterivEXT)
#define glGetMultiTexEnvfvEXT GLEW_GET_FUN(__glewGetMultiTexEnvfvEXT)
#define glGetMultiTexEnvivEXT GLEW_GET_FUN(__glewGetMultiTexEnvivEXT)
#define glGetMultiTexGendvEXT GLEW_GET_FUN(__glewGetMultiTexGendvEXT)
#define glGetMultiTexGenfvEXT GLEW_GET_FUN(__glewGetMultiTexGenfvEXT)
#define glGetMultiTexGenivEXT GLEW_GET_FUN(__glewGetMultiTexGenivEXT)
#define glGetMultiTexImageEXT GLEW_GET_FUN(__glewGetMultiTexImageEXT)
#define glGetMultiTexLevelParameterfvEXT GLEW_GET_FUN(__glewGetMultiTexLevelParameterfvEXT)
#define glGetMultiTexLevelParameterivEXT GLEW_GET_FUN(__glewGetMultiTexLevelParameterivEXT)
#define glGetMultiTexParameterIivEXT GLEW_GET_FUN(__glewGetMultiTexParameterIivEXT)
#define glGetMultiTexParameterIuivEXT GLEW_GET_FUN(__glewGetMultiTexParameterIuivEXT)
#define glGetMultiTexParameterfvEXT GLEW_GET_FUN(__glewGetMultiTexParameterfvEXT)
#define glGetMultiTexParameterivEXT GLEW_GET_FUN(__glewGetMultiTexParameterivEXT)
#define glGetNamedBufferParameterivEXT GLEW_GET_FUN(__glewGetNamedBufferParameterivEXT)
#define glGetNamedBufferPointervEXT GLEW_GET_FUN(__glewGetNamedBufferPointervEXT)
#define glGetNamedBufferSubDataEXT GLEW_GET_FUN(__glewGetNamedBufferSubDataEXT)
#define glGetNamedFramebufferAttachmentParameterivEXT GLEW_GET_FUN(__glewGetNamedFramebufferAttachmentParameterivEXT)
#define glGetNamedProgramLocalParameterIivEXT GLEW_GET_FUN(__glewGetNamedProgramLocalParameterIivEXT)
#define glGetNamedProgramLocalParameterIuivEXT GLEW_GET_FUN(__glewGetNamedProgramLocalParameterIuivEXT)
#define glGetNamedProgramLocalParameterdvEXT GLEW_GET_FUN(__glewGetNamedProgramLocalParameterdvEXT)
#define glGetNamedProgramLocalParameterfvEXT GLEW_GET_FUN(__glewGetNamedProgramLocalParameterfvEXT)
#define glGetNamedProgramStringEXT GLEW_GET_FUN(__glewGetNamedProgramStringEXT)
#define glGetNamedProgramivEXT GLEW_GET_FUN(__glewGetNamedProgramivEXT)
#define glGetNamedRenderbufferParameterivEXT GLEW_GET_FUN(__glewGetNamedRenderbufferParameterivEXT)
#define glGetPointerIndexedvEXT GLEW_GET_FUN(__glewGetPointerIndexedvEXT)
#define glGetPointeri_vEXT GLEW_GET_FUN(__glewGetPointeri_vEXT)
#define glGetTextureImageEXT GLEW_GET_FUN(__glewGetTextureImageEXT)
#define glGetTextureLevelParameterfvEXT GLEW_GET_FUN(__glewGetTextureLevelParameterfvEXT)
#define glGetTextureLevelParameterivEXT GLEW_GET_FUN(__glewGetTextureLevelParameterivEXT)
#define glGetTextureParameterIivEXT GLEW_GET_FUN(__glewGetTextureParameterIivEXT)
#define glGetTextureParameterIuivEXT GLEW_GET_FUN(__glewGetTextureParameterIuivEXT)
#define glGetTextureParameterfvEXT GLEW_GET_FUN(__glewGetTextureParameterfvEXT)
#define glGetTextureParameterivEXT GLEW_GET_FUN(__glewGetTextureParameterivEXT)
#define glGetVertexArrayIntegeri_vEXT GLEW_GET_FUN(__glewGetVertexArrayIntegeri_vEXT)
#define glGetVertexArrayIntegervEXT GLEW_GET_FUN(__glewGetVertexArrayIntegervEXT)
#define glGetVertexArrayPointeri_vEXT GLEW_GET_FUN(__glewGetVertexArrayPointeri_vEXT)
#define glGetVertexArrayPointervEXT GLEW_GET_FUN(__glewGetVertexArrayPointervEXT)
#define glMapNamedBufferEXT GLEW_GET_FUN(__glewMapNamedBufferEXT)
#define glMapNamedBufferRangeEXT GLEW_GET_FUN(__glewMapNamedBufferRangeEXT)
#define glMatrixFrustumEXT GLEW_GET_FUN(__glewMatrixFrustumEXT)
#define glMatrixLoadIdentityEXT GLEW_GET_FUN(__glewMatrixLoadIdentityEXT)
#define glMatrixLoadTransposedEXT GLEW_GET_FUN(__glewMatrixLoadTransposedEXT)
#define glMatrixLoadTransposefEXT GLEW_GET_FUN(__glewMatrixLoadTransposefEXT)
#define glMatrixLoaddEXT GLEW_GET_FUN(__glewMatrixLoaddEXT)
#define glMatrixLoadfEXT GLEW_GET_FUN(__glewMatrixLoadfEXT)
#define glMatrixMultTransposedEXT GLEW_GET_FUN(__glewMatrixMultTransposedEXT)
#define glMatrixMultTransposefEXT GLEW_GET_FUN(__glewMatrixMultTransposefEXT)
#define glMatrixMultdEXT GLEW_GET_FUN(__glewMatrixMultdEXT)
#define glMatrixMultfEXT GLEW_GET_FUN(__glewMatrixMultfEXT)
#define glMatrixOrthoEXT GLEW_GET_FUN(__glewMatrixOrthoEXT)
#define glMatrixPopEXT GLEW_GET_FUN(__glewMatrixPopEXT)
#define glMatrixPushEXT GLEW_GET_FUN(__glewMatrixPushEXT)
#define glMatrixRotatedEXT GLEW_GET_FUN(__glewMatrixRotatedEXT)
#define glMatrixRotatefEXT GLEW_GET_FUN(__glewMatrixRotatefEXT)
#define glMatrixScaledEXT GLEW_GET_FUN(__glewMatrixScaledEXT)
#define glMatrixScalefEXT GLEW_GET_FUN(__glewMatrixScalefEXT)
#define glMatrixTranslatedEXT GLEW_GET_FUN(__glewMatrixTranslatedEXT)
#define glMatrixTranslatefEXT GLEW_GET_FUN(__glewMatrixTranslatefEXT)
#define glMultiTexBufferEXT GLEW_GET_FUN(__glewMultiTexBufferEXT)
#define glMultiTexCoordPointerEXT GLEW_GET_FUN(__glewMultiTexCoordPointerEXT)
#define glMultiTexEnvfEXT GLEW_GET_FUN(__glewMultiTexEnvfEXT)
#define glMultiTexEnvfvEXT GLEW_GET_FUN(__glewMultiTexEnvfvEXT)
#define glMultiTexEnviEXT GLEW_GET_FUN(__glewMultiTexEnviEXT)
#define glMultiTexEnvivEXT GLEW_GET_FUN(__glewMultiTexEnvivEXT)
#define glMultiTexGendEXT GLEW_GET_FUN(__glewMultiTexGendEXT)
#define glMultiTexGendvEXT GLEW_GET_FUN(__glewMultiTexGendvEXT)
#define glMultiTexGenfEXT GLEW_GET_FUN(__glewMultiTexGenfEXT)
#define glMultiTexGenfvEXT GLEW_GET_FUN(__glewMultiTexGenfvEXT)
#define glMultiTexGeniEXT GLEW_GET_FUN(__glewMultiTexGeniEXT)
#define glMultiTexGenivEXT GLEW_GET_FUN(__glewMultiTexGenivEXT)
#define glMultiTexImage1DEXT GLEW_GET_FUN(__glewMultiTexImage1DEXT)
#define glMultiTexImage2DEXT GLEW_GET_FUN(__glewMultiTexImage2DEXT)
#define glMultiTexImage3DEXT GLEW_GET_FUN(__glewMultiTexImage3DEXT)
#define glMultiTexParameterIivEXT GLEW_GET_FUN(__glewMultiTexParameterIivEXT)
#define glMultiTexParameterIuivEXT GLEW_GET_FUN(__glewMultiTexParameterIuivEXT)
#define glMultiTexParameterfEXT GLEW_GET_FUN(__glewMultiTexParameterfEXT)
#define glMultiTexParameterfvEXT GLEW_GET_FUN(__glewMultiTexParameterfvEXT)
#define glMultiTexParameteriEXT GLEW_GET_FUN(__glewMultiTexParameteriEXT)
#define glMultiTexParameterivEXT GLEW_GET_FUN(__glewMultiTexParameterivEXT)
#define glMultiTexRenderbufferEXT GLEW_GET_FUN(__glewMultiTexRenderbufferEXT)
#define glMultiTexSubImage1DEXT GLEW_GET_FUN(__glewMultiTexSubImage1DEXT)
#define glMultiTexSubImage2DEXT GLEW_GET_FUN(__glewMultiTexSubImage2DEXT)
#define glMultiTexSubImage3DEXT GLEW_GET_FUN(__glewMultiTexSubImage3DEXT)
#define glNamedBufferDataEXT GLEW_GET_FUN(__glewNamedBufferDataEXT)
#define glNamedBufferSubDataEXT GLEW_GET_FUN(__glewNamedBufferSubDataEXT)
#define glNamedCopyBufferSubDataEXT GLEW_GET_FUN(__glewNamedCopyBufferSubDataEXT)
#define glNamedFramebufferRenderbufferEXT GLEW_GET_FUN(__glewNamedFramebufferRenderbufferEXT)
#define glNamedFramebufferTexture1DEXT GLEW_GET_FUN(__glewNamedFramebufferTexture1DEXT)
#define glNamedFramebufferTexture2DEXT GLEW_GET_FUN(__glewNamedFramebufferTexture2DEXT)
#define glNamedFramebufferTexture3DEXT GLEW_GET_FUN(__glewNamedFramebufferTexture3DEXT)
#define glNamedFramebufferTextureEXT GLEW_GET_FUN(__glewNamedFramebufferTextureEXT)
#define glNamedFramebufferTextureFaceEXT GLEW_GET_FUN(__glewNamedFramebufferTextureFaceEXT)
#define glNamedFramebufferTextureLayerEXT GLEW_GET_FUN(__glewNamedFramebufferTextureLayerEXT)
#define glNamedProgramLocalParameter4dEXT GLEW_GET_FUN(__glewNamedProgramLocalParameter4dEXT)
#define glNamedProgramLocalParameter4dvEXT GLEW_GET_FUN(__glewNamedProgramLocalParameter4dvEXT)
#define glNamedProgramLocalParameter4fEXT GLEW_GET_FUN(__glewNamedProgramLocalParameter4fEXT)
#define glNamedProgramLocalParameter4fvEXT GLEW_GET_FUN(__glewNamedProgramLocalParameter4fvEXT)
#define glNamedProgramLocalParameterI4iEXT GLEW_GET_FUN(__glewNamedProgramLocalParameterI4iEXT)
#define glNamedProgramLocalParameterI4ivEXT GLEW_GET_FUN(__glewNamedProgramLocalParameterI4ivEXT)
#define glNamedProgramLocalParameterI4uiEXT GLEW_GET_FUN(__glewNamedProgramLocalParameterI4uiEXT)
#define glNamedProgramLocalParameterI4uivEXT GLEW_GET_FUN(__glewNamedProgramLocalParameterI4uivEXT)
#define glNamedProgramLocalParameters4fvEXT GLEW_GET_FUN(__glewNamedProgramLocalParameters4fvEXT)
#define glNamedProgramLocalParametersI4ivEXT GLEW_GET_FUN(__glewNamedProgramLocalParametersI4ivEXT)
#define glNamedProgramLocalParametersI4uivEXT GLEW_GET_FUN(__glewNamedProgramLocalParametersI4uivEXT)
#define glNamedProgramStringEXT GLEW_GET_FUN(__glewNamedProgramStringEXT)
#define glNamedRenderbufferStorageEXT GLEW_GET_FUN(__glewNamedRenderbufferStorageEXT)
#define glNamedRenderbufferStorageMultisampleCoverageEXT GLEW_GET_FUN(__glewNamedRenderbufferStorageMultisampleCoverageEXT)
#define glNamedRenderbufferStorageMultisampleEXT GLEW_GET_FUN(__glewNamedRenderbufferStorageMultisampleEXT)
#define glProgramUniform1fEXT GLEW_GET_FUN(__glewProgramUniform1fEXT)
#define glProgramUniform1fvEXT GLEW_GET_FUN(__glewProgramUniform1fvEXT)
#define glProgramUniform1iEXT GLEW_GET_FUN(__glewProgramUniform1iEXT)
#define glProgramUniform1ivEXT GLEW_GET_FUN(__glewProgramUniform1ivEXT)
#define glProgramUniform1uiEXT GLEW_GET_FUN(__glewProgramUniform1uiEXT)
#define glProgramUniform1uivEXT GLEW_GET_FUN(__glewProgramUniform1uivEXT)
#define glProgramUniform2fEXT GLEW_GET_FUN(__glewProgramUniform2fEXT)
#define glProgramUniform2fvEXT GLEW_GET_FUN(__glewProgramUniform2fvEXT)
#define glProgramUniform2iEXT GLEW_GET_FUN(__glewProgramUniform2iEXT)
#define glProgramUniform2ivEXT GLEW_GET_FUN(__glewProgramUniform2ivEXT)
#define glProgramUniform2uiEXT GLEW_GET_FUN(__glewProgramUniform2uiEXT)
#define glProgramUniform2uivEXT GLEW_GET_FUN(__glewProgramUniform2uivEXT)
#define glProgramUniform3fEXT GLEW_GET_FUN(__glewProgramUniform3fEXT)
#define glProgramUniform3fvEXT GLEW_GET_FUN(__glewProgramUniform3fvEXT)
#define glProgramUniform3iEXT GLEW_GET_FUN(__glewProgramUniform3iEXT)
#define glProgramUniform3ivEXT GLEW_GET_FUN(__glewProgramUniform3ivEXT)
#define glProgramUniform3uiEXT GLEW_GET_FUN(__glewProgramUniform3uiEXT)
#define glProgramUniform3uivEXT GLEW_GET_FUN(__glewProgramUniform3uivEXT)
#define glProgramUniform4fEXT GLEW_GET_FUN(__glewProgramUniform4fEXT)
#define glProgramUniform4fvEXT GLEW_GET_FUN(__glewProgramUniform4fvEXT)
#define glProgramUniform4iEXT GLEW_GET_FUN(__glewProgramUniform4iEXT)
#define glProgramUniform4ivEXT GLEW_GET_FUN(__glewProgramUniform4ivEXT)
#define glProgramUniform4uiEXT GLEW_GET_FUN(__glewProgramUniform4uiEXT)
#define glProgramUniform4uivEXT GLEW_GET_FUN(__glewProgramUniform4uivEXT)
#define glProgramUniformMatrix2fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix2fvEXT)
#define glProgramUniformMatrix2x3fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix2x3fvEXT)
#define glProgramUniformMatrix2x4fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix2x4fvEXT)
#define glProgramUniformMatrix3fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix3fvEXT)
#define glProgramUniformMatrix3x2fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix3x2fvEXT)
#define glProgramUniformMatrix3x4fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix3x4fvEXT)
#define glProgramUniformMatrix4fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix4fvEXT)
#define glProgramUniformMatrix4x2fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix4x2fvEXT)
#define glProgramUniformMatrix4x3fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix4x3fvEXT)
#define glPushClientAttribDefaultEXT GLEW_GET_FUN(__glewPushClientAttribDefaultEXT)
#define glTextureBufferEXT GLEW_GET_FUN(__glewTextureBufferEXT)
#define glTextureImage1DEXT GLEW_GET_FUN(__glewTextureImage1DEXT)
#define glTextureImage2DEXT GLEW_GET_FUN(__glewTextureImage2DEXT)
#define glTextureImage3DEXT GLEW_GET_FUN(__glewTextureImage3DEXT)
#define glTextureParameterIivEXT GLEW_GET_FUN(__glewTextureParameterIivEXT)
#define glTextureParameterIuivEXT GLEW_GET_FUN(__glewTextureParameterIuivEXT)
#define glTextureParameterfEXT GLEW_GET_FUN(__glewTextureParameterfEXT)
#define glTextureParameterfvEXT GLEW_GET_FUN(__glewTextureParameterfvEXT)
#define glTextureParameteriEXT GLEW_GET_FUN(__glewTextureParameteriEXT)
#define glTextureParameterivEXT GLEW_GET_FUN(__glewTextureParameterivEXT)
#define glTextureRenderbufferEXT GLEW_GET_FUN(__glewTextureRenderbufferEXT)
#define glTextureSubImage1DEXT GLEW_GET_FUN(__glewTextureSubImage1DEXT)
#define glTextureSubImage2DEXT GLEW_GET_FUN(__glewTextureSubImage2DEXT)
#define glTextureSubImage3DEXT GLEW_GET_FUN(__glewTextureSubImage3DEXT)
#define glUnmapNamedBufferEXT GLEW_GET_FUN(__glewUnmapNamedBufferEXT)
#define glVertexArrayColorOffsetEXT GLEW_GET_FUN(__glewVertexArrayColorOffsetEXT)
#define glVertexArrayEdgeFlagOffsetEXT GLEW_GET_FUN(__glewVertexArrayEdgeFlagOffsetEXT)
#define glVertexArrayFogCoordOffsetEXT GLEW_GET_FUN(__glewVertexArrayFogCoordOffsetEXT)
#define glVertexArrayIndexOffsetEXT GLEW_GET_FUN(__glewVertexArrayIndexOffsetEXT)
#define glVertexArrayMultiTexCoordOffsetEXT GLEW_GET_FUN(__glewVertexArrayMultiTexCoordOffsetEXT)
#define glVertexArrayNormalOffsetEXT GLEW_GET_FUN(__glewVertexArrayNormalOffsetEXT)
#define glVertexArraySecondaryColorOffsetEXT GLEW_GET_FUN(__glewVertexArraySecondaryColorOffsetEXT)
#define glVertexArrayTexCoordOffsetEXT GLEW_GET_FUN(__glewVertexArrayTexCoordOffsetEXT)
#define glVertexArrayVertexAttribIOffsetEXT GLEW_GET_FUN(__glewVertexArrayVertexAttribIOffsetEXT)
#define glVertexArrayVertexAttribOffsetEXT GLEW_GET_FUN(__glewVertexArrayVertexAttribOffsetEXT)
#define glVertexArrayVertexOffsetEXT GLEW_GET_FUN(__glewVertexArrayVertexOffsetEXT)
*/



		return Surface;
	}

	void DeleteWindowSDL()
	{
		if(Surface)
			SDL_FreeSurface(Surface);
		SDL_Quit();
	}

}//namespace

namespace glf
{
	void swapbuffers()
	{
		SDL_GL_SwapBuffers();
		glGetError();
	}

	std::string loadFile(const char* Filename)
	{
		std::ifstream stream(Filename, std::ios::in);

		if(!stream.is_open())
			return "";

		std::string Line = "";
		std::string Text = "";

		while(getline(stream, Line))
			Text += "\n" + Line;

		stream.close();

		return Text;
	}

	bool checkError(const char* Title)
	{
	    int Error;
	    if((Error = glGetError()) != GL_NO_ERROR)
			fprintf(stdout, "OpenGL Error: %s\n", Title);
		return Error == GL_NO_ERROR;
	}

	bool checkProgram(GLuint ProgramName)
	{
		if(!ProgramName)
			return false;

		GLint Result = GL_FALSE;
		glGetProgramiv(ProgramName, GL_LINK_STATUS, &Result);

		fprintf(stdout, "Linking program\n");
		int InfoLogLength;
		glGetProgramiv(ProgramName, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> Buffer(InfoLogLength);
		glGetProgramInfoLog(ProgramName, InfoLogLength, NULL, &Buffer[0]);
		fprintf(stdout, "%s\n", &Buffer[0]);

		return Result == GL_TRUE;
	}

	bool checkShader(GLuint ShaderName, const char* Source)
	{
		if(!ShaderName)
			return false;

		GLint Result = GL_FALSE;
		glGetShaderiv(ShaderName, GL_COMPILE_STATUS, &Result);

		fprintf(stdout, "Compiling shader\n%s...\n", Source);
		int InfoLogLength;
		glGetShaderiv(ShaderName, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> Buffer(InfoLogLength);
		glGetShaderInfoLog(ShaderName, InfoLogLength, NULL, &Buffer[0]);
		fprintf(stdout, "%s\n", &Buffer[0]);

		return Result == GL_TRUE;
	}

	GLenum cast(glv::type Type)
	{
		static GLenum Cast[glv::TYPE_MAX] =
		{
			GL_NONE,			//TYPE_NULL,
			GL_HALF_FLOAT,		//FLOAT16,
			GL_FLOAT,			//FLOAT32,
			GL_BYTE,			//INT8,
			GL_SHORT,			//INT16,
			GL_INT,				//INT32,
			GL_UNSIGNED_BYTE,	//UINT8,
			GL_UNSIGNED_SHORT,	//UINT16,
			GL_UNSIGNED_INT		//UINT32
		};

		return Cast[Type];
	}

	base::base
	(
		std::string const & Title,
		glm::ivec2 const & WindowSize
	) :
		mouseButtonFlags(0),
		mouseCurrent(0),
		mouseOrigin(0),
		rotationCurrent(0.f),
		rotationOrigin(0.f),
		tranlationOrigin(0.f, 8.f),
		tranlationCurrent(0.f, 0.f),
		indexCurrent(0),
		indexMax(10),
		title(Title),
		windowSize(WindowSize)
	{
		CreateWindowSDL(title.c_str(), WindowSize.x, WindowSize.y, WINDOW_FULLSCREEN);
	}

	base::~base()
	{
		DeleteWindowSDL();
	}

	void base::onMouseMove(glm::vec2 const & MouseCurrent)
	{
		mouseCurrent = MouseCurrent;
		tranlationCurrent = mouseButtonFlags & MOUSE_BUTTON_LEFT ? tranlationOrigin + (mouseCurrent - mouseOrigin) / 10.f : tranlationOrigin;
		rotationCurrent = mouseButtonFlags & MOUSE_BUTTON_RIGHT ? rotationOrigin + (mouseCurrent - mouseOrigin) : rotationOrigin;
	}

	void base::onMouseDown(EMouseButton MouseButton)
	{
		mouseButtonFlags |= MouseButton;

		switch(MouseButton)
		{
		default:
			break;
		case MOUSE_BUTTON_LEFT:
		case MOUSE_BUTTON_RIGHT:
		case MOUSE_BUTTON_MIDDLE:
			mouseOrigin = mouseCurrent;
			break;
		}
	}

	void base::onMouseUp(EMouseButton MouseButton)
	{
		mouseButtonFlags &= ~MouseButton;

		switch(MouseButton)
		{
		default:
			break;
		case MOUSE_BUTTON_LEFT:
			tranlationOrigin += (mouseCurrent - mouseOrigin) / 10.f;
			break;
		case MOUSE_BUTTON_RIGHT:
			rotationOrigin += mouseCurrent - mouseOrigin;
			break;
		case MOUSE_BUTTON_MIDDLE:
			++indexCurrent;
			indexCurrent %= indexMax;
			break;
		}
	}

	bool base::event()
	{
		SDL_Event Event;
		while(SDL_PollEvent(&Event))
		{
			switch(Event.type)
			{
			case SDL_QUIT:
			case SDL_KEYUP:
				return false;
			case SDL_MOUSEMOTION:
				{
					SDL_MouseMotionEvent* MotionEvent = (SDL_MouseMotionEvent*) &Event;
					onMouseMove(glm::vec2(float(MotionEvent->x), float(WINDOW_HEIGHT - MotionEvent->y)));
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch(((SDL_MouseButtonEvent*)&Event)->button)
				{
				default:
					break;
				case SDL_BUTTON_LEFT:
					onMouseDown(MOUSE_BUTTON_LEFT);
					break;
				case SDL_BUTTON_RIGHT:
					onMouseDown(MOUSE_BUTTON_RIGHT);
					break;
				case SDL_BUTTON_MIDDLE:
					onMouseDown(MOUSE_BUTTON_MIDDLE);
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch(((SDL_MouseButtonEvent*)&Event)->button)
				{
				default:
					break;
				case SDL_BUTTON_LEFT:
					onMouseUp(MOUSE_BUTTON_LEFT);
					break;
				case SDL_BUTTON_RIGHT:
					onMouseUp(MOUSE_BUTTON_RIGHT);
					break;
				case SDL_BUTTON_MIDDLE:
					onMouseUp(MOUSE_BUTTON_MIDDLE);
					break;
				}
				break;
			break;
			}
		}

		return true;
	}
}//namespace glf
