#version 120

uniform sampler2D Diffuse;
varying in vec2 VertTexcoord;

void main(void)
{
	gl_FragColor = texture2D(Diffuse, VertTexcoord);
}
