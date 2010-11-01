uniform sampler2DRect Diffuse;

layout(location = FRAG_COLOR, index = 0) out vec4 Color;

void main()
{
	Color = texture(Diffuse, gl_FragCoord.xy * 1.0);
}
