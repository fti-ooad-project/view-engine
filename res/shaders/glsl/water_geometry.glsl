#version 430
$include uniforms.glsl
$include pervertex.glsl
layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 3) out;
in PerVertex pvte[];
void main()
{
	int p = 0 , n = 0;
	for( int i = 0; i < 3; ++i )
	{
		if( gl_in[i].gl_Position.z > WATERZ )
			p++;
		else
			n++;
	}
	if( p == 3 || n == 3 ) return;
    for( int i = 0; i < 3; ++i )
	{
		vec4 p = MAT4X4_VIEWPROJ[0] * vec4( gl_in[i].gl_Position.xyz , 1.0 );
        gl_Position = p;
        EmitVertex();
    }
	EndPrimitive();
}