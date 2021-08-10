#ifndef GK_MATRIX_H
#define GK_MATRIX_H

#ifndef _USE_MATH_DEFINES
    #define _USE_MATH_DEFINES // for M_PI
#endif
#include <cmath>

/*
Notes:
- All rotations are in radians
- All matrices are column major (just like OpenGL)
*/

#define INF INFINITY

template <typename T>
T step(T v0, T v1, T step);

template <typename T>
T clamp(T val, T min, T max);

template <typename T>
struct Vector2
{
    union {
        struct { T x, y; };
        T values[2];
    };

    Vector2();
    Vector2(T v);
    Vector2(const Vector2<T>& v);
    Vector2(T _x, T _y);

    T&       operator [] (unsigned int i);
    const T& operator [] (unsigned int i) const;

    Vector2<T> operator - () const;
    Vector2<T> operator + () const;

    Vector2<T>  operator +  (const Vector2<T>& v) const;
    Vector2<T>  operator -  (const Vector2<T>& v) const;
    Vector2<T>& operator += (const Vector2<T>& v);
    Vector2<T>& operator -= (const Vector2<T>& v);

    Vector2<T>  operator *  (T v) const;
    Vector2<T>& operator *= (T v);

    bool        operator < (const Vector2<T>& v) const;

    bool        operator == (const Vector2<T>& v) const;
    bool        operator != (const Vector2<T>& v) const;
};

template <typename T>
struct Vector3
{
    union {
        T values[3];
        struct { T x, y, z; };
    };

    Vector3();
    Vector3(T v);
    Vector3(const Vector2<T>& v, T z);
    Vector3(const Vector3<T>& v);
    Vector3(T _x, T _y, T _z);

    Vector2<T> xy() const;

    T&        operator [] (unsigned int i);
    const T&  operator [] (unsigned int i) const;

    Vector3<T> operator - () const;
    Vector3<T> operator + () const;

    Vector3<T>  operator +  (const Vector3<T>& v) const;
    Vector3<T>  operator -  (const Vector3<T>& v) const;
    Vector3<T>& operator += (const Vector3<T>& v);
    Vector3<T>& operator -= (const Vector3<T>& v);

    Vector3<T>  operator *  (T v) const;
    Vector3<T>& operator *= (T v);

    bool        operator < (const Vector3<T>& v) const;

    bool        operator == (const Vector3<T>& v) const;
    bool        operator != (const Vector3<T>& v) const;
};

template <typename T>
struct Vector4
{
    union {
        T values[4];
        struct { T x, y, z, w; };
    };

    Vector4();
    Vector4(T v);
    Vector4(const Vector3<T>& v, T w);
    Vector4(const Vector4<T>& v);
    Vector4(T _x, T _y, T _z, T _w);

    Vector3<T> xyz() const;

    T&        operator [] (unsigned int i);
    const T&  operator [] (unsigned int i) const;

    Vector4<T> operator - () const;
    Vector4<T> operator + () const;

    Vector4<T>  operator +  (const Vector4<T>& v) const;
    Vector4<T>  operator -  (const Vector4<T>& v) const;
    Vector4<T>& operator += (const Vector4<T>& v);
    Vector4<T>& operator -= (const Vector4<T>& v);

    Vector4<T>  operator *  (T v) const;
    Vector4<T>& operator *= (T v);

    bool        operator < (const Vector4<T>& v) const;

    bool        operator == (const Vector4<T>& v) const;
    bool        operator != (const Vector4<T>& v) const;
};

namespace Vector
{
    template <typename T> T Length(const Vector2<T>& v);
    template <typename T> T Length(const Vector3<T>& v);
    template <typename T> T Length(const Vector4<T>& v);

    template <typename T> T Dot(const Vector2<T>& v0, const Vector2<T>& v1);
    template <typename T> T Dot(const Vector3<T>& v0, const Vector3<T>& v1);
    template <typename T> T Dot(const Vector4<T>& v0, const Vector4<T>& v1);

    template <typename T> Vector2<T> Normalize(const Vector2<T>& v);
    template <typename T> Vector3<T> Normalize(const Vector3<T>& v);
    template <typename T> Vector4<T> Normalize(const Vector4<T>& v);

    template <typename T> Vector3<T> Cross(const Vector3<T>& v0, const Vector3<T>& v1);

    template <typename T> T Distance(const Vector2<T>& v0, const Vector2<T>& v1);
    template <typename T> T Distance(const Vector3<T>& v0, const Vector3<T>& v1);
    template <typename T> T Distance(const Vector4<T>& v0, const Vector4<T>& v1);
}

typedef Vector2<float> Vector2F;
typedef Vector3<float> Vector3F;
typedef Vector4<float> Vector4F;
typedef Vector2<int> Vector2I;
typedef Vector3<int> Vector3I;
typedef Vector4<int> Vector4I;
typedef Vector2<unsigned int> Vector2U;
typedef Vector3<unsigned int> Vector3U;
typedef Vector4<unsigned int> Vector4U;

struct Matrix2F
{
    Vector2F values[2];

    Matrix2F();
    Matrix2F(float v);
    Matrix2F(const Vector2F& v0, const Vector2F& v1);
    Matrix2F(const Matrix2F& other);
    Matrix2F(const Vector2F* v);
    Matrix2F(const float* v);

    Vector2F&       operator [] (unsigned int i);
    const Vector2F& operator [] (unsigned int i) const;

    Matrix2F& operator  = (const Matrix2F& other);

    Matrix2F  operator +  (const Matrix2F& other) const;
    Matrix2F& operator += (const Matrix2F& other);

    Matrix2F  operator -  (const Matrix2F& other) const;
    Matrix2F& operator -= (const Matrix2F& other);

    Matrix2F  operator *  (float v) const;
    Matrix2F& operator *= (float v);

    Matrix2F  operator *  (const Matrix2F& other) const;
    Matrix2F& operator *= (const Matrix2F& other);

    Vector2F operator * (const Vector2F& v) const;

    void print() const;
};

struct Matrix3F
{
    static const Vector3F X_AXIS;
    static const Vector3F Y_AXIS;
    static const Vector3F Z_AXIS;

    Vector3F values[3];

    Matrix3F();
    Matrix3F(float v);
    Matrix3F(const Vector3F& v0, const Vector3F& v1, const Vector3F& v2);
    Matrix3F(const Matrix3F& other);
    Matrix3F(const Vector3F* v);
    Matrix3F(const float* v);

    Vector3F&       operator [] (unsigned int i);
    const Vector3F& operator [] (unsigned int i) const;

    Matrix3F& operator  = (const Matrix3F& other);

    Matrix3F  operator +  (const Matrix3F& other) const;
    Matrix3F& operator += (const Matrix3F& other);

    Matrix3F  operator -  (const Matrix3F& other) const;
    Matrix3F& operator -= (const Matrix3F& other);

    Matrix3F  operator *  (float v) const;
    Matrix3F& operator *= (float v);

    Matrix3F  operator *  (const Matrix3F& other) const;
    Matrix3F& operator *= (const Matrix3F& other);

    Vector3F operator * (const Vector3F& v) const;

    static Matrix3F Scale(const Vector2F& v);
    static Matrix3F Rotate(float v);
    static Matrix3F Translate(const Vector2F& v);

    void print() const;
};

struct Matrix4F
{
    static const Vector4F X_AXIS;
    static const Vector4F Y_AXIS;
    static const Vector4F Z_AXIS;
    static const Vector4F W_AXIS;

    Vector4F values[4];

    Matrix4F();
    Matrix4F(float v);
    Matrix4F(const Vector4F& v0, const Vector4F& v1, const Vector4F& v2, const Vector4F& v3);
    Matrix4F(const Matrix4F& other);
    Matrix4F(const Vector4F* v);
    Matrix4F(const float* v);

    Vector4F&       operator [] (unsigned int i);
    const Vector4F& operator [] (unsigned int i) const;

    Matrix4F& operator  = (const Matrix4F& other);

    Matrix4F  operator +  (const Matrix4F& other) const;
    Matrix4F& operator += (const Matrix4F& other);

    Matrix4F  operator -  (const Matrix4F& other) const;
    Matrix4F& operator -= (const Matrix4F& other);

    Matrix4F  operator *  (float v) const;
    Matrix4F& operator *= (float v);

    Matrix4F  operator *  (const Matrix4F& other) const;
    Matrix4F& operator *= (const Matrix4F& other);

    Vector4F operator * (const Vector4F& v) const;

    static Matrix4F Scale(const Vector3F& v);
    static Matrix4F Translate(const Vector3F& v);
    static Matrix4F Project(float fov, float width, float height, float near, float far);
    static Matrix4F View(const Vector3F& position, const Vector3F& target, const Vector3F& up);

    void print() const;
};

namespace Matrix
{
    Matrix2F Inverse(const Matrix2F& m);
    Matrix3F Inverse(const Matrix3F& m);
    Matrix4F Inverse(const Matrix4F& m);

    Matrix2F Transpose(const Matrix2F& m);
    Matrix3F Transpose(const Matrix3F& m);
    Matrix4F Transpose(const Matrix4F& m);
}

struct Quaternion : public Vector4F
{
    Quaternion();
    Quaternion(const Vector3F& v);
    Quaternion(const Vector4F& v);
    Quaternion(float _x, float _y, float _z, float _w);
    Quaternion(float _x, float _y, float _z);
    Quaternion(const Matrix3F& matrix);

    Quaternion operator * (const Quaternion& q) const;
    Quaternion operator *= (const Quaternion& q);

    Quaternion& operator = (const Vector4F& q);
    Quaternion& operator = (const Quaternion& q);
    
    Matrix4F matrix() const;
    
    void normalize();
};

extern const Vector4F ORIGIN;
extern const Vector4F X_POSITIVE;
extern const Vector4F Y_POSITIVE;
extern const Vector4F Z_POSITIVE;

#endif // GK_MATRIX_H
