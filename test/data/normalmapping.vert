#version 120
#extension GL_EXT_bindable_uniform : enable
#extension GL_EXT_gpu_shader4 : enable

struct common
{
	mat4 M;
	mat4 V;
	mat4 P;
	vec4 L;
};

bindable uniform common Common;

attribute vec2 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec2 Texcoord;

varying vec2 VertTexcoord;
varying vec3 L_;
varying vec3 V_;
varying vec3 T_;
varying vec3 B_;
varying vec3 N_;
varying vec3 C_;

void main()
{
	mat4 NodelView = Common.V * Common.M;
	mat3 NormalMatrix = mat3(NodelView);
	
	vec4 P = NodelView * vec4(Position, 0.0, 1.0);

	N_ = NormalMatrix * Normal;
	T_ = NormalMatrix * Tangent;
	B_ = cross(N_, T_);

	mat3 TBN = transpose(mat3(T_, B_, N_));

	L_ = TBN * mat3(Common.V) * -vec3(Common.L);
	V_ = TBN * -P.xyz;
	C_ = normalize((Common.M * vec4(Position, 0.0, 1.0)).xyz);

	gl_Position = Common.P * Common.V * Common.M * vec4(Position, 0.0, 1.0);
	VertTexcoord = Texcoord;
}
