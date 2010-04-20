#include "base.hpp"
#include <SDL.h>
#include <SDL_syswm.h>

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

		SDL_Surface* Surface = NULL;
		if((Surface = SDL_SetVideoMode(Width, Height, 32, VideoFlags)) == 0)
			return NULL;

		SDL_WM_SetCaption(Name, Name);

		glewInit();

		//SDL_SysWMinfo wmInfo;
		//SDL_GetWMInfo(&wmInfo);
		//HWND hWnd = wmInfo.window;
		//DeviceContext = GetDC(hWnd);

		//// Create an OpenGL 3.0 context.
		//int AttribList[6] =
		//{
		//	WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		//	WGL_CONTEXT_MINOR_VERSION_ARB, 0,
		//	0, 0
		//};
		//
		//RenderContext = wglCreateContextAttribsARB(DeviceContext, 0, AttribList);
		//wglMakeCurrent(DeviceContext, RenderContext);				

		return Surface;
	}

	void DeleteWindowSDL()
	{
		//wglDeleteContext(RenderContext);
		//RenderContext = NULL;

		if(Surface)
			SDL_FreeSurface(Surface);
		SDL_Quit();
	}

	void MakeCurrent()
	{
		//wglMakeCurrent(DeviceContext, RenderContext);
	}

	void SwapBufferSDL()
	{
		//SwapBuffers(DeviceContext);
		SDL_GL_SwapBuffers();
	}

	//gluxDisplay* Display = NULL;

	//void CreateWindowGLUX(const char* Name, int Width, int Height, bool Fullscreen)
	//{
	//	gluxDisplaySettings Settings;
	//	Settings.BitsPerPixel = 32;
	//	Settings.RedSize = 8;
	//	Settings.GreenSize = 8;
	//	Settings.BlueSize = 8;
	//	Settings.AlphaSize = 8;
	//	Settings.MultisampleBuffers = 1;
	//	Settings.MultisampleSamples = 2;

	//	Display = gluxCreateDisplay(
	//		Width, 
	//		Height, 
	//		Settings,
	//		GLUX_DOUBLEBUFFER | GLUX_RGBA | GLUX_MULTISAMPLE, 
	//		"Coucou");
	//}

	//void DestroyDisplayGLUX()
	//{
	//	gluxDestroyDisplay(Display);
	//}

	//void SwapBufferGLUX()
	//{
	//	gluxSwapBuffers(Display);
	//}

}//namespace

namespace glf
{
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

	IBase::IBase
	(
		std::string const & Title,
		glm::ivec2 const & WindowSize
	) :
		mouseButtonFlags(0),
		mouseCurrent(0),
		mouseOrigin(0),
		rotationCurrent(0.f),
		rotationOrigin(0.f),
		tranlationOrigin(0.f, 4.f),
		tranlationCurrent(0.f, 0.f),
		indexCurrent(0),
		indexMax(10),
		title(Title),
		windowSize(WindowSize)
	{
		//CreateWindowGLUX(title.c_str(), WindowSize.x, WindowSize.y, WINDOW_FULLSCREEN);
		CreateWindowSDL(title.c_str(), WindowSize.x, WindowSize.y, WINDOW_FULLSCREEN);
	}

	IBase::~IBase()
	{
		DeleteWindowSDL();
		//DestroyDisplayGLUX();
	}

	void IBase::OnMouseMove(glm::vec2 const & MouseCurrent)
	{
		mouseCurrent = MouseCurrent;
		tranlationCurrent = mouseButtonFlags & MOUSE_BUTTON_LEFT ? tranlationOrigin + (mouseCurrent - mouseOrigin) / 10.f : tranlationOrigin;
		rotationCurrent = mouseButtonFlags & MOUSE_BUTTON_RIGHT ? rotationOrigin + (mouseCurrent - mouseOrigin) : rotationOrigin;
	}

	void IBase::OnMouseDown(EMouseButton MouseButton)
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

	void IBase::OnMouseUp(EMouseButton MouseButton)
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

	bool IBase::Run()
	{
		bool Exit = false;
		while(!Exit)
		{
			//gluxEvent Event;
			//while(gluxGrabEvent(&Event))
			//{
			//	switch(Event.Type)
			//	{
			//		case GLUX_QUIT:
			//		{
			//			Exit = true;
			//			break;
			//		}
			//		case GLUX_MOUSEDOWN:
			//		{
			//			switch(Event.Mouse.Button)
			//			{
			//			default:
			//				break;
			//			case gluxButton::Left:
			//				OnMouseDown(MOUSE_BUTTON_LEFT);
			//				break;
			//			case gluxButton::Right:
			//				OnMouseDown(MOUSE_BUTTON_RIGHT);
			//				break;
			//			case gluxButton::Middle:
			//				OnMouseDown(MOUSE_BUTTON_MIDDLE);
			//				break;
			//			}
			//		}
			//		case GLUX_MOUSEUP:
			//		{
			//			switch(Event.Mouse.Button)
			//			{
			//			default:
			//				break;
			//			case gluxButton::Left:
			//				OnMouseUp(MOUSE_BUTTON_LEFT);
			//				break;
			//			case gluxButton::Right:
			//				OnMouseUp(MOUSE_BUTTON_RIGHT);
			//				break;
			//			case gluxButton::Middle:
			//				OnMouseUp(MOUSE_BUTTON_MIDDLE);
			//				break;
			//			}
			//		}
			//		case GLUX_MOUSEMOTION:
			//		{
			//			glm::ivec2 Coord;
			//			gluxGrabRelativeMousePos(&Coord.x, &Coord.y);
			//			OnMouseMove(glm::vec2(Coord.x, WINDOW_HEIGHT - Coord.y));
			//		}
			//	}
			//}

			SDL_Event Event;
			while(SDL_PollEvent(&Event))
			{
				switch(Event.type)
				{
				case SDL_QUIT:
				case SDL_KEYUP:
					Exit = true;
					break;
				case SDL_MOUSEMOTION:
					{
					SDL_MouseMotionEvent* MotionEvent = (SDL_MouseMotionEvent*) &Event;
					OnMouseMove(glm::vec2(float(MotionEvent->x), float(WINDOW_HEIGHT - MotionEvent->y)));
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					switch(((SDL_MouseButtonEvent*)&Event)->button)
					{
					default:
						break;
					case SDL_BUTTON_LEFT:
						OnMouseDown(MOUSE_BUTTON_LEFT);
						break;
					case SDL_BUTTON_RIGHT:
						OnMouseDown(MOUSE_BUTTON_RIGHT);
						break;
					case SDL_BUTTON_MIDDLE:
						OnMouseDown(MOUSE_BUTTON_MIDDLE);
						break;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					switch(((SDL_MouseButtonEvent*)&Event)->button)
					{
					default:
						break;
					case SDL_BUTTON_LEFT:
						OnMouseUp(MOUSE_BUTTON_LEFT);
						break;
					case SDL_BUTTON_RIGHT:
						OnMouseUp(MOUSE_BUTTON_RIGHT);
						break;
					case SDL_BUTTON_MIDDLE:
						OnMouseUp(MOUSE_BUTTON_MIDDLE);
						break;
					}
					break;
				break;
				}
			}

			Render();

			//SwapBufferGLUX();
			SwapBufferSDL();
		}

		return Exit;
	}
}//namespace glf

int main(int argc, char* argv[])
{
	return run();
}
