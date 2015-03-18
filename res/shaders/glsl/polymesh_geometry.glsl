#version 430
$include uniforms.glsl
$include pervertex.glsl
$include include.glsl
layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 3) out;
in PerVertex pv[];
out PerVertex pvo;
void main()
{
	float pn = 0.0 , n = 0.0;
	for( int i = 0; i < 3; ++i )
	{
		if( gl_in[i].gl_Position.z > WATERZ )
			pn++;
		else
			n++;
	}
    for( int i = 0; i < 3; ++i )
	{
		vec4 p = MAT4X4_VIEWPROJ[0] * gl_in[i].gl_Position;
		switch( PASSID )
		{
			case PASS_LIGHT:
				gl_Position = scalel( p );
				EmitVertex();
			break;
			case PASS_WATER:
				if( abs( n - pn )  < 3.0 )
				{
					gl_Position = p;
					EmitVertex();
				}
			break;
			case PASS_NORMAL:
				gl_Position = p;
				pvo.depth = p.z;
				pvo.position = pv[i].position;
				pvo.normal = pv[i].normal;
				pvo.select_id = pv[i].select_id;
				pvo.binormal = pv[i].binormal;
				pvo.tangentnormal = pv[i].tangentnormal;
				pvo.texcoord = pv[i].texcoord;
				pvo.dist_to_cam = pv[i].dist_to_cam;
				EmitVertex();
			break;
			default:
			break;
		}
    }
	EndPrimitive();
}