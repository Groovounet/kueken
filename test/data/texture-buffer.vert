#define ATTR_POSITION	0
#define ATTR_COLOR		3
#define ATTR_TEXCOORD	4
#define FRAG_COLOR		0

uniform samplerBuffer MVP;

layout(location = ATTR_POSITION) in vec2 Position;
layout(location = ATTR_TEXCOORD) in vec2 Texcoord;

out vec2 VertTexcoord;

void main()
{	
	VertTexcoord = Texcoord;
	mat4 InstanceMVP;
	for(int i = 0; i < 4; ++i)
		InstanceMVP[i] = texelFetch(MVP, i);
	gl_Position = InstanceMVP * vec4(Position, 0.0, 1.0);
}