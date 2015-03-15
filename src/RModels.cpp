#include "../base/RModels.h"
void REntity::pushChange() const
{
	_calc_m4x4 = false;
}
f4x4 const &REntity::calcModelm4x4()
{
	if (_calc_m4x4) return _m4model_matrix;
	_calc_m4x4 = true;
	f3x3 rot = rotation(f3(1.0f, 0.0f, 0.0f), _v3rotation.x()) *
		rotation(f3(0.0f, 1.0f, 0.0f), _v3rotation.y()) *
		rotation(f3(0.0f, 0.0f, 1.0f), _v3rotation.z());
	_v3local_x = f3(rot._data[0], rot._data[3], rot._data[6]);
	_v3local_y = f3(rot._data[1], rot._data[4], rot._data[7]);
	_v3local_z = f3(rot._data[2], rot._data[5], rot._data[8]);
	_m4model_matrix = f4x4(
		_v3scale.x() * _v3local_x.x(), _v3scale.y() * _v3local_y.x(), _v3scale.z() * _v3local_z.x(), 0.0f,
		_v3scale.x() * _v3local_x.y(), _v3scale.y() * _v3local_y.y(), _v3scale.z() * _v3local_z.y(), 0.0f,
		_v3scale.x() * _v3local_x.z(), _v3scale.y() * _v3local_y.z(), _v3scale.z() * _v3local_z.z(), 0.0f,
		_v3pos.x(), _v3pos.y(), _v3pos.z(), 1.0f);
	return _m4model_matrix;
}
f4x4 const &REntity::getModelm4x4()
{
	return _m4model_matrix;
}