#define ATTR_POSITION	0
#define ATTR_COLOR		3
#define ATTR_TEXCOORD	4
#define FRAG_COLOR		0

uniform sampler2D Diffuse;
in vec2 VertTexcoord;

layout(location = FRAG_COLOR, index = 0) out vec4 Color;

void main()
{
	Color = vec4(texture2D(Diffuse, VertTexcoord).rgb, 0.7);
}
