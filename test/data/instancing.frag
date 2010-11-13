#version 120

uniform sampler2D Diffuse;
varying in vec2 VertTexcoord;

void main()
{
	vec3 Color = texture2D(Diffuse, VertTexcoord).rgb;
	gl_FragColor = vec4(Color, 0.6);
}
