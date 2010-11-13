uniform mat4 MVP;
uniform float Displacement;

layout(location = ATTR_POSITION) in vec2 AttrPosition;
layout(location = ATTR_TEXCOORD) in vec2 AttrTexcoord;

out vec2 Texcoord;

void main()
{	
	Texcoord = AttrTexcoord;
	gl_Position = MVP * vec4(AttrPosition, Displacement, 1.0);
}

