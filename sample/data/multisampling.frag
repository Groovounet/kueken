uniform sampler2D Diffuse;

in vec2 VertTexcoord;

layout(location = FRAG_COLOR, index = 0) out vec4 Color;

vec3 saturate(vec3 Color, float Saturation)
{
	vec3 GrayXfer = vec3(0.30, 0.59, 0.11);
	vec3 Gray = vec3(dot(GrayXfer, Color));
	return mix(Gray, Color, Saturation);
}

vec3 ContrastSaturationBrightness(vec3 color, float brt, float sat, float con)
{
	// Increase or decrease theese values to adjust r, g and b color channels seperately
	const float AvgLumR = 0.5;
	const float AvgLumG = 0.5;
	const float AvgLumB = 0.5;
	
	const vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);
	
	vec3 AvgLumin = vec3(AvgLumR, AvgLumG, AvgLumB);
	vec3 brtColor = color * brt;
	vec3 intensity = vec3(dot(brtColor, LumCoeff));
	vec3 satColor = mix(intensity, brtColor, sat);
	vec3 conColor = mix(AvgLumin, satColor, con);
	return conColor;
}
/*
void main()
{
	vec3 TempColor = ContrastSaturationBrightness(texture2D(Diffuse, VertTexcoord).bgr, 1.0, 1.0, 1.0);
	Color = vec4(TempColor, 0.7);
}
*/
const float Offset = 0.0001;
const float Min = Offset * -2.0;
const float Max = Offset * 2.0;

void main(void)
{
	vec4 TempColor = vec4(0.0);
	for(float j = Min; j < Max; j += Offset) 
	for(float i = Min; i < Max; i += Offset) 
		TempColor += 1.0 / 16.0 * texture2D(Diffuse, VertTexcoord + vec2(i, j));
	Color = TempColor;
}
