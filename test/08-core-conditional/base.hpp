#ifndef GLF_BASE_INCLUDED
#define GLF_BASE_INCLUDED

// GLF libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_projection.hpp>
#include <glm/gtx/transform.hpp>

#include <gli/gli.hpp>
#include <glv/glv.hpp>

#define GLEW_STATIC
#include <GL/glew.h>

// STL
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

namespace glf
{
	enum EMouseButton
	{
		MOUSE_BUTTON_NONE = 0,
		MOUSE_BUTTON_LEFT = (1 << 0),
		MOUSE_BUTTON_RIGHT = (1 << 1),
		MOUSE_BUTTON_MIDDLE = (1 << 2)
	};

	class IBase
	{
	public:
		IBase(std::string const & Title, glm::ivec2 const & WindowSise);
		virtual ~IBase();

		virtual bool Check() const = 0;
		virtual bool Begin(glm::ivec2 const & WindowSize) = 0;
		virtual bool End() = 0;
		virtual void Render() = 0;

		bool Run();
		void OnMouseMove(glm::vec2 const & _MouseCurrent);
		void OnMouseDown(EMouseButton MouseButton);
		void OnMouseUp(EMouseButton MouseButton);

		std::string Title(){return title;}

	protected:
		glm::uint mouseButtonFlags;
		glm::vec2 rotationOrigin;
		glm::vec2 rotationCurrent;
		glm::vec2 tranlationOrigin;
		glm::vec2 tranlationCurrent;
		glm::vec2 mouseCurrent;
		glm::vec2 mouseOrigin;
		int indexCurrent;
		int indexMax;
		glm::ivec2 windowSize;

	private:
		std::string title;
	};

	std::string loadFile(const char* Filename);
	bool checkError(const char* Title);
	bool checkProgram(GLuint ProgramName);
	bool checkShader(GLuint ShaderName, char const* Source);
	GLenum cast(glv::type Type);
}

int run();

#define GLF_BUFFER_OFFSET(i) ((char *)NULL + (i))

#endif//GLF_BASE_INCLUDED
