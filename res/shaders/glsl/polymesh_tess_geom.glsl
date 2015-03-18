#version 430
$include uniforms.glsl
$include pervertex.glsl
$include include.glsl
layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 3) out;
in PerVertex pvte[];
out PerVertex pvo;
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
    for( int i = 0; i < 3; ++i )
	{
		vec4 p;// = gl_in[i].gl_Position;
		float h = texture2D( RGB_NORMAL_A_HEIGHT_TEXTURE , pvte[i].texcoord ).w;
		p = MAT4X4_VIEWPROJ[0] * vec4( gl_in[i].gl_Position.xyz + pvte[i].normal * 0.08 * ( -0.3 + h ) , 1.0 );
		switch( PASSID )
		{
			case PASS_LIGHT:
				gl_Position = scalel( p );
				EmitVertex();
			break;
			case PASS_WATER:
				if( n != 3 && p != 3 )
				{
					gl_Position = p;
					EmitVertex();
				}
			break;
			case PASS_NORMAL:
				gl_Position = p;
				pvo.depth = p.z;
				pvo.position = pvte[i].position;
				pvo.normal = pvte[i].normal;
				pvo.select_id = pvte[i].select_id;
				pvo.binormal = pvte[i].binormal;
				pvo.tangentnormal = pvte[i].tangentnormal;
				pvo.texcoord = pvte[i].texcoord;
				pvo.dist_to_cam = pvte[i].dist_to_cam;
				EmitVertex();
			break;
			default:
			break;
		}
    }
	EndPrimitive();
}