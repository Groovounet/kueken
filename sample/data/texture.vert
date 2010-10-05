#define ATTR_POSITION	0
#define ATTR_COLOR		3
#define ATTR_TEXCOORD	4
#define FRAG_COLOR		0

uniform mat4 MVP;

layout(location = ATTR_POSITION) in vec2 Position;
layout(location = ATTR_TEXCOORD) in vec2 Texcoord;

out vert
{
	vec2 Texcoord;
} Vert;

void main()
{	
	Vert.Texcoord = Texcoord;
	gl_Position = MVP * vec4(Position, 0.0, 1.0);
}
