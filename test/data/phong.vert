#version 120
#extension GL_EXT_bindable_uniform : enable
#extension GL_EXT_gpu_shader4 : enable

struct common
{
	mat4 M;
	mat4 V;
	mat4 P;
};

bindable uniform common Common;

attribute vec2 Position;
attribute vec2 Texcoord;
attribute vec3 Normal;

varying vec2 VertTexcoord;
varying vec3 N;
varying vec3 L;

void main()
{	
	mat3 NormalMatrix = mat3(Common.V) * mat3(Common.M);

	N = normalize(NormalMatrix * Normal);
	
	gl_Position = Common.P * Common.V * Common.M * vec4(Position, 0.0, 1.0);
	VertTexcoord = Texcoord;
}
