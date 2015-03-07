if( bool( FLAGS & MASK_ANIMATED ) )
{
	if( INS_TIME.z < 50.0 )
	{
		mat4 ma = mat4( 0.0 );
		float time , ltime;
		int mixing , curanim , lastanim;
		if( bool( INSTANSING ) )
		{
			time = INS_TIME.x;
			ltime = INS_TIME.y;
			mixing = INS_MIXING_ANIM_INDX.x;
			curanim = INS_MIXING_ANIM_INDX.y;
			lastanim = INS_MIXING_ANIM_INDX.z;
		}
		else
		{
			time = TIME;
			ltime = LAST_TIME;
			mixing = MIXING;
			curanim = CUR_ANIM;
			lastanim = LAST_ANIM;
		}
		float temp = 0.0;
		for( int i = 0; i < 3; i++ )
		{
			temp += invertex_bone_weight[i];
			ma += getMatFromTex( time , float( invertex_indx[i] ) , BONE_COUNT , ANIM_TEX[0] ) * invertex_bone_weight[i];
		}
		ma += getMatFromTex( time , float( invertex_indx[3] ) , BONE_COUNT , ANIM_TEX[0] ) * ( 1.0 - temp );
		for( int i = 0; i < 3; ++i )
			ma[i].w = 0.0;
		ma[3].w = 1.0;
		m = m * ma;
		//p.x += time * BONE_COUNT;
	}
}