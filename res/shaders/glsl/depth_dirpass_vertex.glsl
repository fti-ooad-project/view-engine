#version 430
$include include.glsl
$include attributes.glsl
$include uniforms.glsl

void main() 
{
	vec4 p;
	if( bool( HEIGTMAP ) )
	{
		float h = texture2D( RGB_NORMAL_A_HEIGHT_TEXTURE , 0.5 + 0.5 * invertex_position.xy ).x;
		p = vec4( 100.0 * invertex_position.xy , ( h - 0.5 ) * 100.0 , 1.0 );
	}else
	{
		p = vec4( invertex_position , 1.0 );
		mat4 m;
		if( bool( INSTANSING ) )
		{
			m = INS_MODEL4x4;
		}
		else
		{
			m = MAT4X4_MODEL;
		}
		$include anim_func.glsl
		p = m * p;
	}
	gl_Position =  scalel( MAT4X4_VIEWPROJ[0] * vec4( p.xyz , 1.0 ) );
}