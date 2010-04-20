#version 120

varying vec2 VertTexcoord;
varying vec3 L_;
varying vec3 V_;
varying vec3 T_;
varying vec3 B_;
varying vec3 N_;
varying vec3 C_;

uniform sampler2D DiffuseSpecular;
uniform sampler2D Normalmap;

const float scale = 0.1;
const float ambient = 0.4;
const float shininess = 10.;
const float STEP = 0.01;

void main()
{
	vec3 L = vec3(0.0, 0.0, 1.0) + normalize(L_) * 0.01;
	vec3 V = vec3(0.0, 0.0, 1.0) + normalize(V_) * 0.01;

	vec3 N = normalize(texture2D(Normalmap, VertTexcoord).xyz * 2. - vec3(1.));
	vec3 R = reflect(-L, N);

	float DiffuseFactor = max(dot(L, N), 0.);
	float SpecularFactor = max(pow(dot(N, R), shininess), 0.0);

	vec4 Color = texture2D(DiffuseSpecular, VertTexcoord);

	gl_FragColor = Color * DiffuseFactor + (DiffuseFactor * SpecularFactor) * 0.001;
}
