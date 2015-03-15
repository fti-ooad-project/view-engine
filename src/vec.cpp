#include "../linalg/vec.h"
/*inline float wrap(float x, float min, float max)
{
	if (x > max) return max;
	if (x < min) return min;
	return x;
}
inline float sqr(float i)
{
	return i * i;
}
template< int N, typename T >
T &array<  N, T >::getValue(int i)
{
	return __data[i];
}
template< int N, typename T >
T const &array<  N, T >::getValue(int i) const
{
	return __data[i];
}
template< int N, typename T >
T &array<  N, T >::x()
{
	return __data[0];
}
template< int N, typename T >
T const &array<  N, T >::x() const
{
	return __data[0];
}
template< int N, typename T >
T &array<  N, T >::y()
{
	return __data[1];
}
template< int N, typename T >
T const &array<  N, T >::y() const
{
	return __data[1];
}
template< int N, typename T >
T &array<  N, T >::z()
{
	return __data[2];
}
template< int N, typename T >
T const &array<  N, T >::z() const
{
	return __data[2];
}
template< int N, typename T >
T const *array<  N, T >::getArray() const
{
	return __data;
}
template< int N >
struct unpacker
{
	template< typename T, typename P, typename ...S >
	static void unpack(T *data, int C, P x, S ...arg)
	{
		data[C - N] = static_cast<T>(x);
		unpacker< N - 1 >::unpack(data, C, arg...);
	}
};
template<>
struct unpacker< 1 >
{
	template< typename T, typename P >
	static void unpack(T *data, int C, P x)
	{//PRINT( x ) PRINT( C - 1 )
		data[C - 1] = static_cast<T>(x);
	}
};
template< int N, typename T >
T const &vec< N, T >::getX() const { return this->getValue(0); }
template< int N, typename T >
T &vec< N, T >::getX() { return this->getValue(0); }
template< int N, typename T >
T const &vec< N, T >::getY() const { return this->getValue(1); }
template< int N, typename T >
T &vec< N, T >::getY() { return this->getValue(1); }
template< int N, typename T >
void vec< N, T >::print() const
{
	ito(N)
		std::cout << this->getValue(i) << " ";
	std::cout << "\n";
}
template< int N, typename T >
vec< N, T >::vec()
{
	//memset( this->__data , 0 , N * sizeof( T ) );
}
template< int N, typename T >
vec< N, T >::vec(T d)
{
	ito(N)
		this->__data[i] = d;
}
template< int N, typename T >
vec< N, T >::vec(vec< N, T > const &v)
{
	ito(N)
		this->getValue(i) = v.getValue(i);
}
template< int N, typename T, typename P >
void vec< N, T >::operator=(vec< N, P > const &v)
{
	ito(N)
		this->getValue(i) = static_cast<T>(v.getValue(i));
}
template< int N, typename T, typename ...D >
vec< N, T >::vec(D ...arg)
{
	unpacker< N >::unpack(this->__data, N, arg...);
}
template< int N, int M, typename T >
vec< M, T > vec< N, T >::getVec(int a, int b) const
{
	vec< M, T > out;
	for (int i = a; i < b; i++)
		out.getValue(i) = this->getValue(i);
	return out;
}
template< int N, typename T >
void vec< N, T >::mul(float k)
{
	for (int i = 0; i < N; i++)
		this->getValue() *= k;
}
template< int N, typename T >
vec< N, T > vec< N, T >::g_mul(float k) const
{
	vec< N, T > out;
	for (int i = 0; i < N; i++)
		out.getValue(i) = this->getValue(i) * k;
	return out;
}
template< int N, typename T >
float vec< N, T >::g_dist2(vec< N, T > const &v) const
{
	float d = 0.0f;
	for (int i = 0; i < N; i++)
		d += sqr(this->getValue(i) - v.getValue(i));
	return d;
}
template< int N, typename T >
vec< N, T > vec< N, T >::g_add(vec< N, T > const &v) const
{
	vec< N, T > out;
	for (int i = 0; i < N; i++)
		out.getValue(i) = this->getValue(i) + v.getValue(i);
	return out;
}
template< int N, typename T >
void vec< N, T >::add(vec< N, T > const &v)
{
	for (int i = 0; i < N; i++)
		this->getValue(i) = this->getValue(i) + v.getValue(i);
}
template< int N, typename T >
float vec< N, T >::g_dist(vec< N, T > const &v) const
{
	return sqrtf(this->g_dist2(v));
}
template< int N, typename T >
float vec< N, T >::g_mod2() const
{
	float d = 0.0f;
	for (int i = 0; i < N; i++)
		d += sqr(this->getValue(i));
	return d;
}
template< int N, typename T >
float vec< N, T >::g_mod() const
{
	return sqrtf(g_mod2());
}
template< int N, typename T >
vec< N, T > vec< N, T >::g_norm() const
{
	float m = this->g_mod();
	if (m < DELTA)
		return vec< N, T >(static_cast<T>(0));
	if (fabsf(m - 1.0f) < DELTA)
		return *this;
	return this->g_mul(1.0f / m);
}
template< int N, typename T >
vec< N, T > vec< N, T >::operator&(vec< N, T > const &v) const
{
	vec< N, T > out;
	for (int i = 0; i < N; i++)
		out.getValue(i) = this->getValue(i) * v.getValue(i);
	return out;
}
template< int N, typename T >
vec< N, T > vec< N, T >::operator-(vec< N, T > const &v) const
{
	vec< N, T > out;
	for (int i = 0; i < N; i++)
		out.getValue(i) = this->getValue(i) - v.getValue(i);
	return out;
}
template< int N, typename T >
vec< N, T > vec< N, T >::operator+(vec< N, T > const &v) const
{
	vec< N, T > out;
	for (int i = 0; i < N; i++)
		out.getValue(i) = this->getValue(i) + v.getValue(i);
	return out;
}
template< int N, typename T >
vec< N, T > vec< N, T >::operator*(float k) const
{
	vec< N, T > out;
	for (int i = 0; i < N; i++)
		out.getValue(i) = this->getValue(i) * k;
	return out;
}
template< int N, typename T >
vec< N, T > vec< N, T >::operator/(float k) const
{
	vec< N, T > out;
	float t = 1.0f / k;
	for (int i = 0; i < N; i++)
		out.getValue(i) = this->getValue(i) * t;
	return out;
}
template< int N, typename T >
void vec< N, T >::operator/=(float k)
{
	*this = *this / k;
}
template< int N, typename T >
float vec< N, T >::operator*(vec< N, T > const &v) const
{
	float out = 0.0f;
	for (int i = 0; i < N; i++)
		out += this->getValue(i) * v.getValue(i);
	return out;
}
template< int N, typename T >
float vec< N, T >::operator^(vec< N, T > const &v) const
{
	if (this->g_dist2(v) < DELTA) return 0.0f;
	auto v1 = (*this).g_norm();
	auto v2 = v.g_norm();
	float scl = acos(wrap(v1 * v2, -1.0f, 1.0f));
	float sgn = lperp(v1) * v2;
	if (sgn < 0.0f)
		scl = -scl;
	return scl;
}
template< int N, typename T >
void vec< N, T >::operator+=(vec< N, T > const &v)
{
	*this = *this + v;
}
template< int N, typename T >
void vec< N, T >::operator-=(vec< N, T > const &v)
{
	*this = *this - v;
}
template< int N, typename T >
vec< N, T > vec< N, T >::operator-() const
{
	return *this * -1.0f;
}
template< int N, typename T >
vec< N, T > operator*(float k, vec< N, T > const &v)
{
	vec< N, T > out;
	for (int i = 0; i < N; i++)
		out.getValue(i) = v.getValue(i) * k;
	return out;
}
template< typename T >
vec< 2, T > lperp(vec< 2, T > const &v)
{
	return vec< 2, T >(-v.getValue(1), v.getValue(0));
}
template< typename T >
vec< 3, T > perp_norm(vec< 3, T > const &v, vec< 3, T > const &p)
{
	return (p - v * (p * v)).g_norm();
}
template< typename T >
vec< 3, T > vecx(vec< 3, T > const &a, vec< 3, T > const &b)
{
	return vec< 3, T >(a.getValue(1) * b.getValue(2) - b.getValue(1) * a.getValue(2),
		b.getValue(0) * a.getValue(2) - a.getValue(0) * b.getValue(2),
		a.getValue(0) * b.getValue(1) - b.getValue(0) * a.getValue(1));
}
template< typename T >
vec< 2, T > rotate(vec< 2, T > const &v, float a)
{
	float c = cosf(a);
	float s = sinf(a);
	return vec< 2, T >(X(v) * c - Y(v) * s, Y(v) * c + X(v) * s);
}
template< int N, typename T >
vec< N, T > wrap(vec< N, T > const &v, float min, float max)
{
	return v.g_norm() * wrap(v.g_mod(), min, max);
}*/
float RVectorFactory::getRandom()
{
	return static_cast<float>(dev()) / std::random_device::max();
}
f3 RVectorFactory::getRandomHalfSphere()
{
	float phi = getRandom() * PI * 2.0f;
	float cp = cosf(phi);
	float sp = sinf(phi);
	float ct = getRandom();
	float st = sqrtf(1.0f - ct * ct);
	return f3(st * cp, st * sp, ct);
}
f3 RVectorFactory::getRandomSphere()
{
	float phi = getRandom() * PI * 2.0f;
	float theta = PI * 0.5f + asinf(2.0f * getRandom() - 1.0f);
	float cp = cosf(phi);
	float sp = sinf(phi);
	float ct = //-1.0f + 2.0f * getRandom();
		cosf(theta);
	float st = //sqrtf( 1.0f - ct * ct );
		sinf(theta);
	return f3(st * cp, st * sp, ct);
}
f2 RVectorFactory::getRandomCircle()
{
	float phi = getRandom() * PI * 2.0f;
	float r = powf(getRandom(), 0.5f);
	return f2(cos(phi), sin(phi)) * r;
}
f3 RVectorFactory::getReflected(f3 const &v, f3 const &n)
{
	return v - 2.0f * n * (n * v);
}
std::random_device RVectorFactory::dev;
const float RVectorFactory::PI = cosf( -1.0f );