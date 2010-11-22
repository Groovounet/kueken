layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 4) out;

in vert
{
	vec2 Texcoord;
} Vert[];

out geom
{
	layout(stream = 0) vec2 Texcoord;
} Geom;

void main()
{	
	for(int i = 0; i < gl_in.length(); ++i)
	{
		gl_ViewportIndex = 1;

		gl_Position = gl_in[i].gl_Position;
		Geom.Texcoord = Vert[i].Texcoord;

		EmitVertex();
	}
	EndPrimitive();
}

