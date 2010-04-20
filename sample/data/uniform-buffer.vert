#version 120
#extension GL_EXT_bindable_uniform : enable
#extension GL_EXT_gpu_shader4 : enable

bindable uniform mat4 MVP;

attribute vec2 Position;
attribute vec2 Texcoord;

varying vec2 VertTexcoord;

void main()
{	
	VertTexcoord = Texcoord;

	gl_Position = MVP * vec4(Position, 0.0, 1.0);
}