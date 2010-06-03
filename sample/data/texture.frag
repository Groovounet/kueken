#version 330

uniform sampler2D Diffuse;
in vec2 VertTexcoord;

void main(void)
{
	gl_FragColor = texture2D(Diffuse, VertTexcoord);
}
