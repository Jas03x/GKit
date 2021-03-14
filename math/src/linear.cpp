#include <gk/math/linear.hpp>

#include <cstdio>
#include <cstring>

#include <algorithm>

#define _SQR(x) ((x) * (x))
#define _DOT2(x0, y0, x1, y1) ((x0) * (x1) + (y0) * (y1))
#define _DOT3(x0, y0, z0, x1, y1, z1) ((x0) * (x1) + (y0) * (y1) + (z0) * (z1))
#define _DOT4(x0, y0, z0, w0, x1, y1, z1, w1) ((x0) * (x1) + (y0) * (y1) + (z0) * (z1) + (w0) * (w1))

// -----------------------------------------------------------------------------------------------------------------------------------
// ------------------------------ Scalar ---------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------

template <typename T>
T step(T v0, T v1, T step)
{
    T d = v1 - v0;
    return (std::abs(d) > step) ? (v0 + copysign(step, d)) : v1;
}

template float  step(float  v0, float  v1, float  step);
template double step(double v0, double v1, double step);

template <typename T>
T clamp(T val, T min, T max)
{
    return std::max(std::min(val, max), min);
}

#define INSTANTIATE_SCALAR_FUNCTION_TEMPLATES(x) \
    template x clamp(x val, x min, x max);       \

INSTANTIATE_SCALAR_FUNCTION_TEMPLATES(char);
INSTANTIATE_SCALAR_FUNCTION_TEMPLATES(short);
INSTANTIATE_SCALAR_FUNCTION_TEMPLATES(int);
INSTANTIATE_SCALAR_FUNCTION_TEMPLATES(long);
INSTANTIATE_SCALAR_FUNCTION_TEMPLATES(unsigned char);
INSTANTIATE_SCALAR_FUNCTION_TEMPLATES(unsigned short);
INSTANTIATE_SCALAR_FUNCTION_TEMPLATES(unsigned int);
INSTANTIATE_SCALAR_FUNCTION_TEMPLATES(unsigned long);
INSTANTIATE_SCALAR_FUNCTION_TEMPLATES(float);
INSTANTIATE_SCALAR_FUNCTION_TEMPLATES(double);

// -----------------------------------------------------------------------------------------------------------------------------------
// ------------------------------ Vector 2 -------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------

template <typename T> Vector2<T>::Vector2() { x = y = 0; }
template <typename T> Vector2<T>::Vector2(T v) : Vector2(v, v) {}
template <typename T> Vector2<T>::Vector2(const Vector2<T>& v) : Vector2(v.x, v.y) {}
template <typename T> Vector2<T>::Vector2(T _x, T _y) { x = _x; y = _y; }

template <typename T> T&       Vector2<T>::operator [] (unsigned int i)       { return values[i]; }
template <typename T> const T& Vector2<T>::operator [] (unsigned int i) const { return values[i]; }

template <typename T> Vector2<T> Vector2<T>::operator - () const { return ((*this) * -1); }
template <typename T> Vector2<T> Vector2<T>::operator + () const { return ((*this) * +1); }

template <typename T> Vector2<T>  Vector2<T>::operator +  (const Vector2<T>& v) const { return Vector2<T>(x + v.x, y + v.y); }
template <typename T> Vector2<T>  Vector2<T>::operator -  (const Vector2<T>& v) const { return Vector2<T>(x - v.x, y - v.y); }
template <typename T> Vector2<T>& Vector2<T>::operator += (const Vector2<T>& v)       { x += v.x; y += v.y; return *this; }
template <typename T> Vector2<T>& Vector2<T>::operator -= (const Vector2<T>& v)       { x -= v.x; y -= v.y; return *this;}

template <typename T> Vector2<T>  Vector2<T>::operator *  (T v) const { return Vector2<T>(x * v, y * v); }
template <typename T> Vector2<T>& Vector2<T>::operator *= (T v)       { x *= v; y *= v; return *this; }

template <typename T> bool Vector2<T>::operator < (const Vector2<T>& v) const { return (x < v.x) || ((x == v.x) && (y < v.y)); }

template <typename T> bool Vector2<T>::operator == (const Vector2<T>& v) const { return (x == v.x) && (y == v.y); }
template <typename T> bool Vector2<T>::operator != (const Vector2<T>& v) const { return !(*this == v); }

// -----------------------------------------------------------------------------------------------------------------------------------
// ------------------------------ Vector 3 -------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------

template <typename T> Vector3<T>::Vector3() { x = y = z = 0; }
template <typename T> Vector3<T>::Vector3(T v) : Vector3(v, v, v) {}
template <typename T> Vector3<T>::Vector3(const Vector2<T>& v, T z) : Vector3(v.x, v.y, z) {}
template <typename T> Vector3<T>::Vector3(const Vector3<T>& v) : Vector3(v.x, v.y, v.z) {}
template <typename T> Vector3<T>::Vector3(T _x, T _y, T _z) { x = _x; y = _y; z = _z; }

template <typename T> Vector2<T> Vector3<T>::xy() const { return Vector2<T>(x, y); }

template <typename T> T&        Vector3<T>::operator [] (unsigned int i)       { return values[i]; }
template <typename T> const T&  Vector3<T>::operator [] (unsigned int i) const { return values[i]; }

template <typename T> Vector3<T> Vector3<T>::operator - () const { return ((*this) * -1); }
template <typename T> Vector3<T> Vector3<T>::operator + () const { return ((*this) * +1); }

template <typename T> Vector3<T>  Vector3<T>::operator +  (const Vector3<T>& v) const { return Vector3<T>(x + v.x, y + v.y, z + v.z); }
template <typename T> Vector3<T>  Vector3<T>::operator -  (const Vector3<T>& v) const { return Vector3<T>(x - v.x, y - v.y, z - v.z); }
template <typename T> Vector3<T>& Vector3<T>::operator += (const Vector3<T>& v)       { x += v.x; y += v.y; z += v.z; return *this; }
template <typename T> Vector3<T>& Vector3<T>::operator -= (const Vector3<T>& v)       { x -= v.x; y -= v.y; z -= v.z; return *this; }

template <typename T> Vector3<T>  Vector3<T>::operator *  (T v) const { return Vector3<T>(x * v, y * v, z * v); }
template <typename T> Vector3<T>& Vector3<T>::operator *= (T v)       { x *= v; y *= v; z *= v; return *this; }

template <typename T> bool Vector3<T>::operator < (const Vector3<T>& v) const { return (x < v.x) || ((x == v.x) && (y < v.y)) || ((x == v.x) && (y == v.y) && (z < v.z)); }

template <typename T> bool Vector3<T>::operator == (const Vector3<T>& v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
template <typename T> bool Vector3<T>::operator != (const Vector3<T>& v) const { return !(*this == v); }

// -----------------------------------------------------------------------------------------------------------------------------------
// ------------------------------ Vector 4 -------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------

template <typename T> Vector4<T>::Vector4() { x = y = z = w = 0; }
template <typename T> Vector4<T>::Vector4(T v) : Vector4(v, v, v, v) {}
template <typename T> Vector4<T>::Vector4(const Vector3<T>& v, T w) : Vector4(v.x, v.y, v.z, w) {}
template <typename T> Vector4<T>::Vector4(const Vector4& v) : Vector4(v.x, v.y, v.z, v.w) {}
template <typename T> Vector4<T>::Vector4(T _x, T _y, T _z, T _w) { x = _x; y = _y; z = _z; w = _w;}

template <typename T> Vector3<T> Vector4<T>::xyz() const { return Vector3<T>(x, y, z); }

template <typename T> T&        Vector4<T>::operator [] (unsigned int i)       { return values[i]; }
template <typename T> const T&  Vector4<T>::operator [] (unsigned int i) const { return values[i]; }

template <typename T> Vector4<T> Vector4<T>::operator - () const { return ((*this) * -1); }
template <typename T> Vector4<T> Vector4<T>::operator + () const { return ((*this) * +1); }

template <typename T> Vector4<T>  Vector4<T>::operator +  (const Vector4& v) const { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
template <typename T> Vector4<T>  Vector4<T>::operator -  (const Vector4& v) const { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
template <typename T> Vector4<T>& Vector4<T>::operator += (const Vector4& v)       { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
template <typename T> Vector4<T>& Vector4<T>::operator -= (const Vector4& v)       { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }

template <typename T> Vector4<T>  Vector4<T>::operator *  (T v) const { return Vector4(x * v, y * v, z * v, w * v); }
template <typename T> Vector4<T>& Vector4<T>::operator *= (T v)       { x *= v; y *= v; z *= v; w *= v; return *this; }

template <typename T> bool Vector4<T>::operator < (const Vector4<T>& v) const { return (x < v.x) || ((x == v.x) && (y < v.y)) || ((x == v.x) && (y == v.y) && (z < v.z)) || ((x == v.x) && (y == v.y) && (z == v.z) && (w < v.w)); }

template <typename T> bool Vector4<T>::operator == (const Vector4<T>& v) const { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
template <typename T> bool Vector4<T>::operator != (const Vector4<T>& v) const { return !(*this == v); }

// -----------------------------------------------------------------------------------------------------------------------------------
// ------------------------------ Vector Functions -----------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------

template <typename T> T Vector::Length(const Vector2<T>& v) { return static_cast<T>(sqrtf(static_cast<float>(_SQR(v.x) + _SQR(v.y)))); }
template <typename T> T Vector::Length(const Vector3<T>& v) { return static_cast<T>(sqrtf(static_cast<float>(_SQR(v.x) + _SQR(v.y) + _SQR(v.z)))); }
template <typename T> T Vector::Length(const Vector4<T>& v) { return static_cast<T>(sqrtf(static_cast<float>(_SQR(v.x) + _SQR(v.y) + _SQR(v.z) + _SQR(v.w)))); }

template <typename T> T Vector::Dot(const Vector2<T>& v0, const Vector2<T>& v1) { return _DOT2(v0.x, v0.y, v1.x, v1.y); }
template <typename T> T Vector::Dot(const Vector3<T>& v0, const Vector3<T>& v1) { return _DOT3(v0.x, v0.y, v0.z, v1.x, v1.y, v1.z); }
template <typename T> T Vector::Dot(const Vector4<T>& v0, const Vector4<T>& v1) { return _DOT4(v0.x, v0.y, v0.z, v0.w, v1.x, v1.y, v1.z, v1.w); }

template <typename T> Vector2<T> Vector::Normalize(const Vector2<T>& v) { return v * (static_cast<T>(1) / Vector::Length(v)); }
template <typename T> Vector3<T> Vector::Normalize(const Vector3<T>& v) { return v * (static_cast<T>(1) / Vector::Length(v)); }
template <typename T> Vector4<T> Vector::Normalize(const Vector4<T>& v) { return v * (static_cast<T>(1) / Vector::Length(v)); }

template <typename T> Vector3<T> Vector::Cross(const Vector3<T>& v0, const Vector3<T>& v1) {
    T x = v0.y * v1.z - v0.z * v1.y;
    T y = v0.x * v1.z - v0.z * v1.x;
    T z = v0.x * v1.y - v0.y * v1.x;
    return Vector3<T>(x, -y, z);
}

#define DIST(x,y) (((x) - (y)) * ((x) - (y)))
template <typename T> T Vector::Distance(const Vector2<T>& v0, const Vector2<T>& v1) { return sqrt(DIST(v0.x, v1.x) + DIST(v0.y, v1.y)); }
template <typename T> T Vector::Distance(const Vector3<T>& v0, const Vector3<T>& v1) { return sqrt(DIST(v0.x, v1.x) + DIST(v0.y, v1.y) + DIST(v0.z, v1.z)); }
template <typename T> T Vector::Distance(const Vector4<T>& v0, const Vector4<T>& v1) { return sqrt(DIST(v0.x, v1.x) + DIST(v0.y, v1.y) + DIST(v0.z, v1.z) + DIST(v0.w, v1.w)); }

// -----------------------------------------------------------------------------------------------------------------------------------
// ------------------------------ Vector Instantiations ------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------

#define INSTANTIATE_VECTOR_TEMPLATES(x)                                                 \
    template struct Vector2<x>;                                                         \
    template struct Vector3<x>;                                                         \
    template struct Vector4<x>;                                                         \

#define INSTANTIATE_VECTOR_FUNCTION_TEMPLATES(x)                                        \
    template x Vector::Length(const Vector2<x>& v);                                     \
    template x Vector::Length(const Vector3<x>& v);                                     \
    template x Vector::Length(const Vector4<x>& v);                                     \
    template x Vector::Dot(const Vector2<x>& v0, const Vector2<x>& v1);                 \
    template x Vector::Dot(const Vector3<x>& v0, const Vector3<x>& v1);                 \
    template x Vector::Dot(const Vector4<x>& v0, const Vector4<x>& v1);                 \
    template x Vector::Distance(const Vector2<x>& v0, const Vector2<x>& v1);            \
    template x Vector::Distance(const Vector3<x>& v0, const Vector3<x>& v1);            \
    template x Vector::Distance(const Vector4<x>& v0, const Vector4<x>& v1);            \
    template Vector3<x> Vector::Cross(const Vector3<x>& v0, const Vector3<x>& v1);      \
    template Vector2<x> Vector::Normalize(const Vector2<x>& v0);                        \
    template Vector3<x> Vector::Normalize(const Vector3<x>& v0);                        \
    template Vector4<x> Vector::Normalize(const Vector4<x>& v0);                        \

INSTANTIATE_VECTOR_TEMPLATES(char);
INSTANTIATE_VECTOR_TEMPLATES(short);
INSTANTIATE_VECTOR_TEMPLATES(int);
INSTANTIATE_VECTOR_TEMPLATES(long);
INSTANTIATE_VECTOR_TEMPLATES(unsigned char);
INSTANTIATE_VECTOR_TEMPLATES(unsigned short);
INSTANTIATE_VECTOR_TEMPLATES(unsigned int);
INSTANTIATE_VECTOR_TEMPLATES(unsigned long);
INSTANTIATE_VECTOR_TEMPLATES(float);
INSTANTIATE_VECTOR_TEMPLATES(double);

INSTANTIATE_VECTOR_FUNCTION_TEMPLATES(float);
INSTANTIATE_VECTOR_FUNCTION_TEMPLATES(double);

// -----------------------------------------------------------------------------------------------------------------------------------
// ------------------------------ Matrix 2 -------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------

Matrix2F::Matrix2F() : Matrix2F(1.0f) {}

Matrix2F::Matrix2F(float v) {
	values[0] = Vector2F(v, 0);
	values[1] = Vector2F(0, v);
}

Matrix2F::Matrix2F(const Vector2F& v0, const Vector2F& v1) {
	values[0] = v0;
	values[1] = v1;
}

Matrix2F::Matrix2F(const Matrix2F& other) : Matrix2F(&other.values[0][0]) {}
Matrix2F::Matrix2F(const Vector2F* v) : Matrix2F(&v[0][0]) {}
Matrix2F::Matrix2F(const float* v) { memcpy(values, v, sizeof(values)); }

Vector2F&       Matrix2F::operator [] (unsigned int i)       { return values[i]; }
const Vector2F& Matrix2F::operator [] (unsigned int i) const { return values[i]; }

Matrix2F& Matrix2F::operator = (const Matrix2F& other) { memcpy(this->values, other.values, sizeof(values)); return *this; }

Matrix2F Matrix2F::operator + (const Matrix2F& other) const {
    Matrix2F result;
    result[0] = values[0] + other.values[0];
    result[1] = values[1] + other.values[1];
    return result;
}

Matrix2F& Matrix2F::operator += (const Matrix2F& other) {
	values[0] += other.values[0];
	values[1] += other.values[1];
    return *this;
}

Matrix2F Matrix2F::operator - (const Matrix2F& other) const {
    Matrix2F result;
    result[0] = values[0] - other.values[0];
    result[1] = values[1] - other.values[1];
    return result;
}

Matrix2F& Matrix2F::operator -= (const Matrix2F& other) {
	values[0] -= other.values[0];
	values[1] -= other.values[1];
    return *this;
}
#define _DOT(m, m0, m1, r, c) (m)[c][r] = _DOT2((m0)[0][r], (m0)[1][r], (m1)[c][0], (m1)[c][1])
Matrix2F Matrix2F::operator * (const Matrix2F& other) const {
    Matrix2F result;
    _DOT(result.values, this->values, other.values, 0, 0);
    _DOT(result.values, this->values, other.values, 0, 1);
    _DOT(result.values, this->values, other.values, 1, 0);
    _DOT(result.values, this->values, other.values, 1, 1);
    return result;
}

Matrix2F& Matrix2F::operator *= (const Matrix2F& other) {
    Matrix2F copy = *this;
    _DOT(this->values, copy.values, other.values, 0, 0);
    _DOT(this->values, copy.values, other.values, 0, 1);
    _DOT(this->values, copy.values, other.values, 1, 0);
    _DOT(this->values, copy.values, other.values, 1, 1);
    return *this;
}
#undef _DOT

Matrix2F Matrix2F::operator * (float v) const {
    Matrix2F copy = *this;
    copy *= v;
    return copy;
}

Matrix2F& Matrix2F::operator *= (float v) {
	values[0] *= v;
	values[1] *= v;
    return *this;
}

Vector2F Matrix2F::operator * (const Vector2F& v) const {
    return Vector2F(
        _DOT2(v.x, v.y, values[0][0], values[0][1]),
        _DOT2(v.x, v.y, values[1][0], values[1][1])
    );
}

void Matrix2F::print() const {
    printf("[%f, %f]\n[%f, %f]\n",
        values[0][0], values[1][0],
		values[0][1], values[1][1]
    );
}

// -----------------------------------------------------------------------------------------------------------------------------------
// ------------------------------ Matrix 3 -------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------

const Vector3F Matrix3F::X_AXIS = Vector3F(1, 0, 0);
const Vector3F Matrix3F::Y_AXIS = Vector3F(0, 1, 0);
const Vector3F Matrix3F::Z_AXIS = Vector3F(0, 0, 1);

Matrix3F::Matrix3F() : Matrix3F(1.0f) {}

Matrix3F::Matrix3F(float v) {
	values[0] = Vector3F(v, 0, 0);
	values[1] = Vector3F(0, v, 0);
	values[2] = Vector3F(0, 0, v);
}

Matrix3F::Matrix3F(const Vector3F& v0, const Vector3F& v1, const Vector3F& v2) {
	values[0] = v0;
	values[1] = v1;
	values[2] = v2;
}

Matrix3F::Matrix3F(const Matrix3F& other) : Matrix3F(&other.values[0][0]) {}
Matrix3F::Matrix3F(const Vector3F* v) : Matrix3F(&v[0][0]) {}
Matrix3F::Matrix3F(const float* v) { memcpy(values, v, sizeof(values)); }

Vector3F&       Matrix3F::operator [] (unsigned int i)       { return values[i]; }
const Vector3F& Matrix3F::operator [] (unsigned int i) const { return values[i]; }

Matrix3F& Matrix3F::operator = (const Matrix3F& other) { memcpy(this->values, other.values, sizeof(values)); return *this; }

Matrix3F Matrix3F::operator + (const Matrix3F& other) const {
    Matrix3F result;
    result[0] = values[0] + other.values[0];
    result[1] = values[1] + other.values[1];
    result[2] = values[2] + other.values[2];
    return result;
}

Matrix3F& Matrix3F::operator += (const Matrix3F& other) {
	values[0] += other.values[0];
	values[1] += other.values[1];
	values[2] += other.values[2];
    return *this;
}

Matrix3F Matrix3F::operator - (const Matrix3F& other) const {
    Matrix3F result;
    result[0] = values[0] - other.values[0];
    result[1] = values[1] - other.values[1];
    result[2] = values[2] - other.values[2];
    return result;
}

Matrix3F& Matrix3F::operator -= (const Matrix3F& other) {
    values[0] -= other.values[0];
    values[1] -= other.values[1];
    values[2] -= other.values[2];
    return *this;
}
#define _DOT(m, m0, m1, r, c) (m)[c][r] = _DOT3((m0)[0][r], (m0)[1][r], (m0)[2][r], (m1)[c][0], (m1)[c][1], (m1)[c][2])
Matrix3F Matrix3F::operator * (const Matrix3F& other) const {
    Matrix3F result;
    _DOT(result.values, this->values, other.values, 0, 0);
    _DOT(result.values, this->values, other.values, 0, 1);
    _DOT(result.values, this->values, other.values, 0, 2);
    _DOT(result.values, this->values, other.values, 1, 0);
    _DOT(result.values, this->values, other.values, 1, 1);
    _DOT(result.values, this->values, other.values, 1, 2);
    _DOT(result.values, this->values, other.values, 2, 0);
    _DOT(result.values, this->values, other.values, 2, 1);
    _DOT(result.values, this->values, other.values, 2, 2);
    return result;
}

Matrix3F& Matrix3F::operator *= (const Matrix3F& other) {
    Matrix3F copy = *this;
    _DOT(this->values, copy.values, other.values, 0, 0);
    _DOT(this->values, copy.values, other.values, 0, 1);
    _DOT(this->values, copy.values, other.values, 0, 2);
    _DOT(this->values, copy.values, other.values, 1, 0);
    _DOT(this->values, copy.values, other.values, 1, 1);
    _DOT(this->values, copy.values, other.values, 1, 2);
    _DOT(this->values, copy.values, other.values, 2, 0);
    _DOT(this->values, copy.values, other.values, 2, 1);
    _DOT(this->values, copy.values, other.values, 2, 2);
    return *this;
}
#undef _DOT

Matrix3F Matrix3F::operator * (float v) const {
    Matrix3F copy = *this;
    copy *= v;
    return copy;
}

Matrix3F& Matrix3F::operator *= (float v) {
    values[0] *= v;
    values[1] *= v;
    values[2] *= v;
    return *this;
}

Vector3F Matrix3F::operator * (const Vector3F& v) const {
    return Vector3F {
        _DOT3(v.x, v.y, v.z, values[0][0], values[1][0], values[2][0]),
        _DOT3(v.x, v.y, v.z, values[0][1], values[1][1], values[2][1]),
        _DOT3(v.x, v.y, v.z, values[0][2], values[1][2], values[2][2])
    };
}

Matrix3F Matrix3F::Scale(const Vector2F& v) { return Matrix3F(X_AXIS * v.x, Y_AXIS * v.y, Z_AXIS); }
Matrix3F Matrix3F::Translate(const Vector2F& v) { return Matrix3F(X_AXIS, Y_AXIS, Vector3F(v, 1)); }
Matrix3F Matrix3F::Rotate(float v)
{
    float c = cos(v);
    float s = sin(v);
    return Matrix3F {
        { c, -s,  0 },
        { s,  c,  0 },
        { 0,  0,  1 }
    };
}

void Matrix3F::print() const {
    printf("[%f, %f, %f]\n[%f, %f, %f]\n[%f, %f, %f]\n",
        values[0][0], values[0][1], values[0][2],
        values[1][0], values[1][1], values[1][2],
        values[2][0], values[2][1], values[2][2]
    );
}

// -----------------------------------------------------------------------------------------------------------------------------------
// ------------------------------ Matrix 4 -------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------

const Vector4F Matrix4F::X_AXIS = Vector4F(1, 0, 0, 0);
const Vector4F Matrix4F::Y_AXIS = Vector4F(0, 1, 0, 0);
const Vector4F Matrix4F::Z_AXIS = Vector4F(0, 0, 1, 0);
const Vector4F Matrix4F::W_AXIS = Vector4F(0, 0, 0, 1);

Matrix4F::Matrix4F() : Matrix4F(1.0f) {}
    
Matrix4F::Matrix4F(float v) {
    values[0] = Vector4F(v, 0, 0, 0);
    values[1] = Vector4F(0, v, 0, 0);
    values[2] = Vector4F(0, 0, v, 0);
    values[3] = Vector4F(0, 0, 0, v);
}

Matrix4F::Matrix4F(const Vector4F& v0, const Vector4F& v1, const Vector4F& v2, const Vector4F& v3) {
    values[0] = v0;
    values[1] = v1;
    values[2] = v2;
    values[3] = v3;
}

Matrix4F::Matrix4F(const Matrix4F& other) : Matrix4F(&other.values[0][0]) {}
Matrix4F::Matrix4F(const Vector4F* v) : Matrix4F(&v[0][0]) {}
Matrix4F::Matrix4F(const float* v) { memcpy(values, v, sizeof(values)); }

Vector4F&       Matrix4F::operator [] (unsigned int i)       { return values[i]; }
const Vector4F& Matrix4F::operator [] (unsigned int i) const { return values[i]; }

Matrix4F& Matrix4F::operator = (const Matrix4F& other) { memcpy(this->values, other.values, sizeof(values)); return *this; }

Matrix4F Matrix4F::operator + (const Matrix4F& other) const {
    Matrix4F result;
    result[0] = values[0] + other.values[0];
    result[1] = values[1] + other.values[1];
    result[2] = values[2] + other.values[2];
    result[3] = values[3] + other.values[3];
    return result;
}

Matrix4F& Matrix4F::operator += (const Matrix4F& other) {
    values[0] += other.values[0];
    values[1] += other.values[1];
    values[2] += other.values[2];
    values[3] += other.values[3];
    return *this;
}

Matrix4F Matrix4F::operator - (const Matrix4F& other) const {
    Matrix4F result;
    result[0] = values[0] - other.values[0];
    result[1] = values[1] - other.values[1];
    result[2] = values[2] - other.values[2];
    result[3] = values[3] - other.values[3];
    return result;
}

Matrix4F& Matrix4F::operator -= (const Matrix4F& other) {
    values[0] -= other.values[0];
    values[1] -= other.values[1];
    values[2] -= other.values[2];
    values[3] -= other.values[3];
    return *this;
}

#define _DOT(m, m0, m1, r, c) (m)[c][r] = _DOT4((m0)[0][r], (m0)[1][r], (m0)[2][r], (m0)[3][r], (m1)[c][0], (m1)[c][1], (m1)[c][2], (m1)[c][3])
Matrix4F Matrix4F::operator * (const Matrix4F& other) const {
    Matrix4F result;
    _DOT(result.values, this->values, other.values, 0, 0);
    _DOT(result.values, this->values, other.values, 0, 1);
    _DOT(result.values, this->values, other.values, 0, 2);
    _DOT(result.values, this->values, other.values, 0, 3);
    _DOT(result.values, this->values, other.values, 1, 0);
    _DOT(result.values, this->values, other.values, 1, 1);
    _DOT(result.values, this->values, other.values, 1, 2);
    _DOT(result.values, this->values, other.values, 1, 3);
    _DOT(result.values, this->values, other.values, 2, 0);
    _DOT(result.values, this->values, other.values, 2, 1);
    _DOT(result.values, this->values, other.values, 2, 2);
    _DOT(result.values, this->values, other.values, 2, 3);
    _DOT(result.values, this->values, other.values, 3, 0);
    _DOT(result.values, this->values, other.values, 3, 1);
    _DOT(result.values, this->values, other.values, 3, 2);
    _DOT(result.values, this->values, other.values, 3, 3);
    return result;
}

Matrix4F& Matrix4F::operator *= (const Matrix4F& other) {
    Matrix4F copy = *this;
    _DOT(this->values, copy.values, other.values, 0, 0);
    _DOT(this->values, copy.values, other.values, 0, 1);
    _DOT(this->values, copy.values, other.values, 0, 2);
    _DOT(this->values, copy.values, other.values, 0, 3);
    _DOT(this->values, copy.values, other.values, 1, 0);
    _DOT(this->values, copy.values, other.values, 1, 1);
    _DOT(this->values, copy.values, other.values, 1, 2);
    _DOT(this->values, copy.values, other.values, 1, 3);
    _DOT(this->values, copy.values, other.values, 2, 0);
    _DOT(this->values, copy.values, other.values, 2, 1);
    _DOT(this->values, copy.values, other.values, 2, 2);
    _DOT(this->values, copy.values, other.values, 2, 3);
    _DOT(this->values, copy.values, other.values, 3, 0);
    _DOT(this->values, copy.values, other.values, 3, 1);
    _DOT(this->values, copy.values, other.values, 3, 2);
    _DOT(this->values, copy.values, other.values, 3, 3);
    return *this;
}
#undef _DOT

Matrix4F Matrix4F::operator * (float v) const {
    Matrix4F copy = *this;
    copy *= v;
    return copy;
}

Matrix4F& Matrix4F::operator *= (float v) {
    values[0] *= v;
    values[1] *= v;
    values[2] *= v;
    values[3] *= v;
    return *this;
}

Vector4F Matrix4F::operator * (const Vector4F& v) const {
    return Vector4F {
        _DOT4(v.x, v.y, v.z, v.w, values[0][0], values[1][0], values[2][0], values[3][0]),
        _DOT4(v.x, v.y, v.z, v.w, values[0][1], values[1][1], values[2][1], values[3][1]),
        _DOT4(v.x, v.y, v.z, v.w, values[0][2], values[1][2], values[2][2], values[3][2]),
        _DOT4(v.x, v.y, v.z, v.w, values[0][3], values[1][3], values[2][3], values[3][3])
    };
}

Matrix4F Matrix4F::Scale(const Vector3F& v) { return Matrix4F(X_AXIS * v.x, Y_AXIS * v.y, Z_AXIS * v.z, W_AXIS); }
Matrix4F Matrix4F::Translate(const Vector3F& v) { return Matrix4F(X_AXIS, Y_AXIS, Z_AXIS, Vector4F(v, 1)); }

// https://github.com/g-truc/glm/blob/0.9.5/glm/gtc/matrix_transform.inl#L218
Matrix4F Matrix4F::Project(float fov, float width, float height, float near, float far)
{
    float aspect = width / height;
    float half_angle = tanf(fov / 2.0f);

    Matrix4F matrix(0.0f);
    matrix[0][0] = 1.0f / (aspect * half_angle);
    matrix[1][1] = 1.0f / half_angle;
    matrix[2][2] = -(far + near) / (far - near);
    matrix[2][3] = -1.0f;
    matrix[3][2] = -(2.0f * far * near) / (far - near);
    return matrix;
}

// https://www.3dgep.com/understanding-the-view-matrix/
Matrix4F Matrix4F::View(const Vector3F& position, const Vector3F& target, const Vector3F& up)
{
    Vector3F f = Vector::Normalize(target - position);
    Vector3F s = Vector::Normalize(Vector::Cross(f, up));
    Vector3F u = Vector::Cross(s, f);

    return Matrix4F {
        { s.x, u.x, -f.x, 0 },
        { s.y, u.y, -f.y, 0 },
        { s.z, u.z, -f.z, 0 },
        {
            -_DOT3(s.x, s.y, s.z, position.x, position.y, position.z),
            -_DOT3(u.x, u.y, u.z, position.x, position.y, position.z),
             _DOT3(f.x, f.y, f.z, position.x, position.y, position.z),
            1
        }
    };
}

void Matrix4F::print() const {
    printf("[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n",
        values[0][0], values[0][1], values[0][2], values[0][3],
        values[1][0], values[1][1], values[1][2], values[1][3],
        values[2][0], values[2][1], values[2][2], values[2][3],
        values[3][0], values[3][1], values[3][2], values[3][3]
    );
}

// -----------------------------------------------------------------------------------------------------------------------------------
// ------------------------------ Matrix Functions -----------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------

#define _DET2(a, b, c, d) ((a) * (d) - (b) * (c))
#define _DET3(a, b, c, d, e, f, g, h, i) (((a) * _DET2(e, f, h, i)) - ((b) * _DET2(d, f, g, i)) + ((c) * _DET2(d, e, g, h)))

Matrix2F Matrix::Inverse(const Matrix2F& m)
{
   // float d = 1.0f / (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
    float d = 1.0f / _DET2(m[0][0], m[0][1], m[1][0], m[1][1]);
    Matrix2F matrix;
    matrix[0][0] =  m[1][1];
    matrix[1][1] =  m[0][0];
    matrix[0][1] = -m[0][1];
    matrix[1][0] = -m[1][0];
    matrix *= d;
    return matrix;
}

Matrix3F Matrix::Inverse(const Matrix3F& m)
{
    float d = 1.0f / _DET3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]);
    Matrix3F matrix;
    matrix[0][0] =  _DET2(m[1][1], m[1][2], m[2][1], m[2][2]);
    matrix[1][0] = -_DET2(m[1][0], m[1][2], m[2][0], m[2][2]);
    matrix[2][0] =  _DET2(m[1][0], m[1][1], m[2][0], m[2][1]);
    matrix[0][1] = -_DET2(m[0][1], m[0][2], m[2][1], m[2][2]);
    matrix[1][1] =  _DET2(m[0][0], m[0][2], m[2][0], m[2][2]);
    matrix[2][1] = -_DET2(m[0][0], m[0][1], m[2][0], m[2][1]);
    matrix[0][2] =  _DET2(m[0][1], m[0][2], m[1][1], m[1][2]);
    matrix[1][2] = -_DET2(m[0][0], m[0][2], m[1][0], m[1][2]);
    matrix[2][2] =  _DET2(m[0][0], m[0][1], m[1][0], m[1][1]);
    matrix *= d;
    return matrix;
}

Matrix4F Matrix::Inverse(const Matrix4F& m)
{
    float m0 = m[0][0] * _DET3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
    float m1 = m[0][1] * _DET3(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
    float m2 = m[0][2] * _DET3(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
    float m3 = m[0][3] * _DET3(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);
    float d = 1.0f / (m0 - m1 + m2 - m3);

    Matrix4F matrix;
    matrix[0][0] =  _DET3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
    matrix[1][0] = -_DET3(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
    matrix[2][0] =  _DET3(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
    matrix[3][0] = -_DET3(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);

    matrix[0][1] = -_DET3(m[0][1], m[0][2], m[0][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
    matrix[1][1] =  _DET3(m[0][0], m[0][2], m[0][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
    matrix[2][1] = -_DET3(m[0][0], m[0][1], m[0][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
    matrix[3][1] =  _DET3(m[0][0], m[0][1], m[0][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);

    matrix[0][2] =  _DET3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[3][1], m[3][2], m[3][3]);
    matrix[1][2] = -_DET3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[3][0], m[3][2], m[3][3]);
    matrix[2][2] =  _DET3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[3][0], m[3][1], m[3][3]);
    matrix[3][2] = -_DET3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[3][0], m[3][1], m[3][2]);

    matrix[0][3] = -_DET3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3]);
    matrix[1][3] =  _DET3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3]);
    matrix[2][3] = -_DET3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3]);
    matrix[3][3] =  _DET3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]);

    matrix *= d;

    return matrix;
}

Matrix2F Matrix::Transpose(const Matrix2F& m)
{
    return Matrix2F {
        { m[0][0], m[1][0] },
        { m[0][1], m[1][1] }
    };
}
Matrix3F Matrix::Transpose(const Matrix3F& m)
{
    return Matrix3F {
        { m[0][0], m[1][0], m[2][0] },
        { m[0][1], m[1][1], m[2][1] },
        { m[0][2], m[1][2], m[2][2] }
    };
}

Matrix4F Matrix::Transpose(const Matrix4F& m)
{
    return Matrix4F {
        { m[0][0], m[1][0], m[2][0], m[3][0] },
        { m[0][1], m[1][1], m[2][1], m[3][1] },
        { m[0][2], m[1][2], m[2][2], m[3][2] },
        { m[0][3], m[1][3], m[2][3], m[3][3] }
    };
}

// -----------------------------------------------------------------------------------------------------------------------------------
// ------------------------------ Quaternion -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------

// Quaternion implemention based on glm (see https://glm.g-truc.net/)

Quaternion::Quaternion() : Vector4F(0, 0, 0, 1) {}
Quaternion::Quaternion(const Vector4F& v) : Quaternion(v.x, v.y, v.z, v.w) {}
Quaternion::Quaternion(float _x, float _y, float _z) : Quaternion(Vector3F(_x, _y, _z)) {}

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

Quaternion::Quaternion(const Vector3F& v)
{
    const Vector3F h = v * 0.5f; // half vector
    const Vector3F c(cosf(h.x), cosf(h.y), cosf(h.z));
    const Vector3F s(sinf(h.x), sinf(h.y), sinf(h.z));

    w = c.x * c.y * c.z + s.x * s.y * s.z;
    x = s.x * c.y * c.z - c.x * s.y * s.z;
    y = c.x * s.y * c.z + s.x * c.y * s.z;
    z = c.x * c.y * s.z - s.x * s.y * c.z;
}

// https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
Quaternion::Quaternion(const Matrix3F& matrix)
{
    if (matrix[0][0] + matrix[1][1] + matrix[2][2] > 0.0f)
    {
        float f = sqrt(1.0f + matrix[0][0] + matrix[1][1] + matrix[2][2]) * 2.0f;
        this->w = f * 0.25f;
        this->x = (matrix[1][2] - matrix[2][1]) / f;
        this->y = (matrix[2][0] - matrix[0][2]) / f;
        this->z = (matrix[0][1] - matrix[1][0]) / f;
    }
    else if ((matrix[0][0] > matrix[1][1]) && (matrix[0][0] > matrix[2][2]))
    {
        float f = sqrt(1.0f + matrix[0][0] - matrix[1][1] - matrix[2][2]) * 2.0f;
        this->w = (matrix[1][2] - matrix[2][1]) / f;
        this->x = f * 0.25f;
        this->y = (matrix[1][0] + matrix[0][1]) / f;
        this->z = (matrix[2][0] + matrix[0][2]) / f;
    }
    else if (matrix[1][1] > matrix[2][2])
    {
        float f = sqrt(1.0f - matrix[0][0] + matrix[1][1] - matrix[2][2]) * 2.0f;
        this->w = (matrix[2][0] - matrix[0][2]) / f;
        this->x = (matrix[1][0] + matrix[0][1]) / f;
        this->y = f * 0.25f;
        this->z = (matrix[2][1] + matrix[1][2]) / f;
    }
    else
    {
        float f = sqrt(1.0f - matrix[0][0] - matrix[1][1] + matrix[2][2]) * 2.0f;
        this->w = (matrix[0][1] - matrix[1][0]) / f;
        this->x = (matrix[2][0] + matrix[0][2]) / f;
        this->y = (matrix[2][1] + matrix[1][2]) / f;
        this->z = f * 0.25f;
    }
}

Quaternion Quaternion::operator * (const Quaternion& q) const
{
    Quaternion r;
    r.w = this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z;
    r.x = this->w * q.x + this->x * q.w + this->y * q.z - this->z * q.y;
    r.y = this->w * q.y + this->y * q.w + this->z * q.x - this->x * q.z;
    r.z = this->w * q.z + this->z * q.w + this->x * q.y - this->y * q.x;
    return r;
}

Quaternion Quaternion::operator *= (const Quaternion& q)
{
    const Quaternion p = *this;   
    this->w = p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z;
    this->x = p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y;
    this->y = p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z;
    this->z = p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x;
    return *this;
}

Quaternion& Quaternion::operator = (const Vector4F& other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->w = other.w;
    return *this;
}

Quaternion& Quaternion::operator = (const Quaternion& other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->w = other.w;
    return *this;
}

void Quaternion::normalize()
{
    float len = Vector::Length(*this);
    if(len != 0.0f)
    {
        float f = 1.0f / len;

        if(f != 1.0f)
        {
            x *= f;
            y *= f;
            z *= f;
            w *= f;
        }
    }
}

Matrix4F Quaternion::matrix() const
{
    Matrix4F result(1.0f);
    float qxx = x * x;
    float qyy = y * y;
    float qzz = z * z;
    float qxz = x * z;
    float qxy = x * y;
    float qyz = y * z;
    float qwx = w * x;
    float qwy = w * y;
    float qwz = w * z;

    result[0][0] = 1.0f - 2.0f * (qyy +  qzz);
    result[0][1] = 2.0f * (qxy + qwz);
    result[0][2] = 2.0f * (qxz - qwy);

    result[1][0] = 2.0f * (qxy - qwz);
    result[1][1] = 1.0f - 2.0f * (qxx +  qzz);
    result[1][2] = 2.0f * (qyz + qwx);

    result[2][0] = 2.0f * (qxz + qwy);
    result[2][1] = 2.0f * (qyz - qwx);
    result[2][2] = 1.0f - 2.0f * (qxx +  qyy);

    return result;
}

// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
Vector3F Quaternion::euler_angles() const
{
    Vector3F euler;

    euler.x = atan2f(2.0f * (x * w + y * z), 1.0f - 2.0f * (x * x + y * y));

    float p = 2.0f * (y * w - x * z);
    if (fabs(p) >= 1.0f) {
        euler.y = copysign(M_PI / 2.0f, p);
    }
    else {
        euler.y = asinf(p);
    }

    euler.z = atan2f(2.0f * (x * y + z * w), 1.0f - 2.0f * (y * y + z * z));

    return euler;
}

const Vector4F ORIGIN     = Vector4F(0, 0, 0, 1);
const Vector4F X_POSITIVE = Vector4F(1, 0, 0, 1);
const Vector4F Y_POSITIVE = Vector4F(0, 1, 0, 1);
const Vector4F Z_POSITIVE = Vector4F(0, 0, 1, 1);
