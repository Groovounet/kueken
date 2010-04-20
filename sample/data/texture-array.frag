#version 120
#extension GL_EXT_texture_array : enable

uniform sampler2DArray DiffuseSampler;
varying in vec2 VertTexcoord;

void main()
{
	float len = length(VertTexcoord - vec2(0.5)) * 2.0f;
	gl_FragColor = texture2DArray(DiffuseSampler, vec3(VertTexcoord, len));
}
