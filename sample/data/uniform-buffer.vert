#define ATTR_POSITION	0
#define ATTR_COLOR		3
#define ATTR_TEXCOORD	4
#define FRAG_COLOR		0
#define FRAG_RED		0
#define FRAG_GREEN		1
#define FRAG_BLUE		2
#define FRAG_ALPHA		3

bindable uniform mat4 MVP;

layout(location = ATTR_POSITION) in vec2 Position;
layout(location = ATTR_TEXCOORD) in vec2 Texcoord;

out vec2 VertTexcoord;

void main()
{	
	VertTexcoord = Texcoord;

	gl_Position = MVP * vec4(Position, 0.0, 1.0);
}
