#version 120
#extension GL_EXT_bindable_uniform : enable

struct common
{
	mat4 MVP;
	vec4 Color;
};
bindable uniform common Common;

attribute vec2 Position;

void main()
{	
	gl_Position = Common.MVP * vec4(Position, 0.0, 1.0);
}

