#version 120

uniform mat4 MVP;
attribute vec2 Position;

void main()
{	
	gl_Position = MVP * vec4(Position, 0.0, 1.0);
}