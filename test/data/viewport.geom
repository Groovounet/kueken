in vert
{
	vec2 Texcoord;
} Vert[];

out geom
{
	smooth vec2 Texcoord;
	flat int Invocation;
} Geom;

void main()
{	
	for(int i = 0; i < gl_in.length(); ++i)
	{
		gl_ViewportIndex = gl_InvocationID;

		gl_Position = gl_in[i].gl_Position;
		Geom.Texcoord = Vert[i].Texcoord;
		Geom.Invocation = gl_InvocationID;

		EmitVertex();
	}
	EndPrimitive();
}

