#version 120

uniform sampler2D Diffuse;
varying vec2 VertTexcoord;

void main()
{
	float Height = texture2D(Diffuse, VertTexcoord).x;
	gl_FragColor = vec4(VertTexcoord, Height, 1.0);
}
