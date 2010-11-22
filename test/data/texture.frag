uniform sampler2D Diffuse;

in geom
{
	vec2 Texcoord;
} Geom;

layout(location = FRAG_COLOR, index = 0) out vec4 Color;

void main()
{
	Color = texture(Diffuse, Geom.Texcoord);
}
