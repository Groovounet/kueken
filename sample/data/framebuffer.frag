uniform sampler2DRect Diffuse;

//layout(origin_upper_left) in vec4 gl_FragCoord;
layout(location = FRAG_COLOR, index = 0) out vec4 Color;

void main()
{
	Color = texture(Diffuse, gl_FragCoord.xy);
}
