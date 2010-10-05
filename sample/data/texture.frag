#define ATTR_POSITION	0
#define ATTR_COLOR		3
#define ATTR_TEXCOORD	4
#define FRAG_COLOR		0

uniform sampler2D Diffuse;

in vert
{
	vec2 Texcoord;
} Vert;

layout(location = FRAG_COLOR, index = 0) out vec4 Color;

void main()
{
	Color = texture(Diffuse, Vert.Texcoord);
}
