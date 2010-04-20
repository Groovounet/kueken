#version 120
#extension GL_EXT_gpu_shader4 : enable

uniform samplerBuffer MVP;

attribute vec2 Position;
attribute vec2 Texcoord;

varying vec2 VertTexcoord;

void main()
{	
	VertTexcoord = Texcoord;
	mat4 InstanceMVP;
	for(int i = 0; i < 4; ++i)
		InstanceMVP[i] = texelFetchBuffer(MVP, i);
	gl_Position = InstanceMVP * vec4(Position, 0.0, 1.0);
}