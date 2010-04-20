#version 120

uniform sampler2D Diffuse;
uniform sampler2D Detail;

varying in vec2 VertTexcoord;

void main()
{
	vec3 ColorValue = texture2D(Diffuse, VertTexcoord).rgb;
	vec3 DetailValue = texture2D(Detail, VertTexcoord).rgb;
	gl_FragColor = vec4(ColorValue * DetailValue, 0.7);
}
