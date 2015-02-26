if( bool( FLAGS & MASK_ANIMATED ) )
	{
		mat4 ma = mat4( 0.0 );
		float temp = 0.0;
		for( int i = 0; i < 3; i++ )
		{
			temp += invertex_bone_weight[i];
			ma += getMatFromTex( TIME , float( invertex_indx[i] ) , BONE_COUNT , ANIM_TEX ) * invertex_bone_weight[i];
		}
		//ma += getMatFromTex( TIME , float( invertex_indx[3] ) , BONE_COUNT , ANIM_TEX ) * ( 1.0 - temp );
		for( int i = 0; i < 3; ++i )
			ma[i].w = 0.0;
		m = m * ma;
	}