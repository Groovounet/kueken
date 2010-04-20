#version 120

uniform sampler2D Diffuse;
varying in vec2 VertTexcoord;

void main()
{
	gl_FragColor = vec4(texture2D(Diffuse, VertTexcoord).rgb, 0.7);
}
