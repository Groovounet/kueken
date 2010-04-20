#version 120
#extension GL_EXT_bindable_uniform : enable
#extension GL_EXT_gpu_shader4 : enable

bindable uniform mat4 MVP;

attribute vec2 Position;
attribute vec2 Texcoord;

uniform sampler2D Heightmap;

varying vec2 VertTexcoord;

void main()
{	
	VertTexcoord = Texcoord;

	vec3 P = vec3(Position, texture2D(Heightmap, VertTexcoord).x);

	gl_Position = MVP * vec4(P, 1.0);
}