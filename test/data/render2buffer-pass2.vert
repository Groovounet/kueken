#version 120

uniform mat4 MVP;

attribute vec4 Position;
varying vec4 VertColor;

void main()
{	
	VertColor = Position / 255;
	gl_Position = MVP * Position;
}