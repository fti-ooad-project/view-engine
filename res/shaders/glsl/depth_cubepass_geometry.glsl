#version 430
$include uniforms.glsl
//layout( location = 60 ) uniform mat4 MAT4X4_VIEWPROJ[6];

layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 18) out;

void main()
{
    for( int j = 0; j < 6; ++j )
	{
		gl_Layer = j;
        for( int i = 0; i < 3; ++i )
		{
            gl_Position = MAT4X4_VIEWPROJ[j] * vec4( gl_in[i].gl_Position.xyz , 1.0f );
            EmitVertex();
        }
        EndPrimitive();
    }
}