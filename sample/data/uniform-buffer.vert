#define ATTR_POSITION	0
#define ATTR_COLOR		3
#define ATTR_TEXCOORD	4
#define FRAG_COLOR		0

uniform transform
{
	mat4 MVP;
} Transform;

layout(location = ATTR_POSITION) in vec2 Position;
layout(location = ATTR_TEXCOORD) in vec2 Texcoord;

out vec2 VertTexcoord;

void main()
{	
	VertTexcoord = Texcoord;

	gl_Position = Transform.MVP * vec4(Position, 0.0, 1.0);
}
