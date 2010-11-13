#version 120

uniform sampler2D Diffuse;
uniform sampler2D Specular;

varying vec2 VertTexcoord;
varying vec3 N;

const vec3 AmbientColor = vec3(0.4);
const float Shininess = 100.;
const vec3 L = vec3(0.0, 0.0, 1.0);
const vec3 V = vec3(0.0, 0.0, 1.0);

void main()
{
	vec3 V = normalize(V);
	vec3 N = normalize(N);
	vec3 R = reflect(-L, N);

	vec3 DiffuseColor = texture2D(Diffuse, VertTexcoord).bgr;
	float SpecularValue = texture2D(Specular, VertTexcoord).x;
	float DiffuseFactor = max(dot(L, N), 0.) * 0.5;
	float SpecularFactor = max(pow(dot(N, R), Shininess), 0.) * SpecularValue;
	
	vec3 Color = AmbientColor + DiffuseColor * vec3(DiffuseFactor + SpecularFactor);
	
	gl_FragColor = vec4(Color, 0.7);
}
