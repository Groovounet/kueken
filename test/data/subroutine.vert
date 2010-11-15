uniform mat4 MVP;
uniform float Displacement;

layout(location = ATTR_POSITION) in vec2 Position;
layout(location = ATTR_TEXCOORD) in vec2 Texcoord;

out vert
{
	vec2 Texcoord;
} Vert;

void main()
{	
	Vert.Texcoord = Texcoord;
	gl_Position = MVP * vec4(Position, Displacement, 1.0);
}

