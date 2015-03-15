#pragma once
#ifndef __RModels_H__
#define __RModels_H__
#include "../linalg/vec.h"
#include "../linalg/mat.h"
#include <list>
//==========================
//	REntity class 26.03.14 schreiner
//==========================
class REntity
{
public:
	f3 _v3pos;
	f3 _v3rotation;
	f3 _v3size;
	f3 _v3scale;
	mutable bool _calc_m4x4 = false;
	f3 _v3local_x;
	f3 _v3local_y;
	f3 _v3local_z;
	f4x4 _m4model_matrix;
	void pushChange() const;
	f4x4 const &calcModelm4x4();
	f4x4 const &getModelm4x4();
};
#endif
