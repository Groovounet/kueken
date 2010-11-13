#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect DiffuseSampler;
varying in vec2 VertTexcoord;

void main()
{
	gl_FragColor = textureRect(DiffuseSampler, VertTexcoord);
}
