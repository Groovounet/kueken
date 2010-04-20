#version 120
#extension GL_EXT_bindable_uniform : enable

struct common
{
	mat4 MVP;
	vec4 Color;
};
bindable uniform common Common;

void main()
{
	gl_FragColor = Common.Color;
}
