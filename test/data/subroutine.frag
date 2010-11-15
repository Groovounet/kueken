layout(location = FRAG_COLOR, index = 0) out vec4 Color;

uniform sampler2D DiffuseDXT1;
uniform sampler2D DiffuseRGB8;

in vert
{
	vec2 Texcoord;
} Vert;

subroutine vec4 diffuse();

subroutine(diffuse)
vec4 diffuseLQ()
{
	return texture(DiffuseDXT1, Vert.Texcoord);
}

subroutine(diffuse)
vec4 diffuseHQ()
{
	return texture(DiffuseRGB8, Vert.Texcoord);
}

subroutine uniform diffuse Diffuse;

void main()
{
	Color = Diffuse();
}
