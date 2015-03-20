mat4 getMatFromTex( float time , float bone , float bone_count , sampler2DArray anim_buffer , float anim_id )
{
	vec2 step = vec2( 1.0 / ( 4.0 * bone_count ) , 0.0 );
	vec2 tx = vec2( ( bone ) * step.x * 4.0 + step.x * 0.5 , time );
	return mat4(
		texture( anim_buffer , vec3( tx , anim_id ) ) ,
		texture( anim_buffer , vec3( tx + step , anim_id ) ) ,
		texture( anim_buffer , vec3( tx + 2.0 * step , anim_id ) ) ,
		texture( anim_buffer , vec3( tx + 3.0 * step , anim_id ) ) );
}