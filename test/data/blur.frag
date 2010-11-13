#version 120
#extension GL_EXT_bindable_uniform : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2D	Diffuse;

uniform vec2 Offset[16];
uniform float Weight[16];

varying vec2 VertTexcoord;

void main()
{
	vec4 Color = vec4(0.0);
	for(int i = 0; i < 16; i++) 
		Color += texture2D(Diffuse, VertTexcoord + Offset[i]) * Weight[i];
	gl_FragColor = Color;
}
