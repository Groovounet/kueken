layout(location = FRAG_COLOR, index = 0) out vec4 Color;

uniform sampler2D DiffuseRGB8;
uniform sampler2D DiffuseBGR8;

in geom
{
	smooth vec2 Texcoord;
	flat int Invocation;
} Geom;

subroutine vec4 diffuse();

subroutine(diffuse)
vec4 diffuseRGB8()
{
	return texture(DiffuseRGB8, Vert.Texcoord);
}

subroutine(diffuse)
vec4 diffuseBGR8()
{
	return texture(DiffuseBGR8, Vert.Texcoord) * vec4(1.0, 0.5, 0.0, 1.0);
}

subroutine uniform diffuse Diffuse;

void main()
{
	Color = Diffuse();
}
