#include "../linalg/mat.h"
f3x3 rotation(const f3 &nax, float ang)
{
	float sa = sin(ang), ca = cos(ang);
	float oa = 1.0 - ca;
	return f3x3(
		ca + oa * nax.x() * nax.x(),
		oa * nax.y() * nax.x() + sa * nax.z(),
		oa * nax.z() * nax.x() - sa * nax.y(),

		oa * nax.x() * nax.y() - sa * nax.z(),
		ca + oa * nax.y() * nax.y(),
		oa * nax.z() * nax.y() + sa * nax.x(),

		oa * nax.x() * nax.z() + sa * nax.y(),
		oa * nax.y() * nax.z() - sa * nax.x(),
		ca + oa * nax.z() * nax.z()
		);
}