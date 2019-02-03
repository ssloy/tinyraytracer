#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__
#include <cmath>
#include <vector>
#include <cassert>

template <typename T> struct vec3 {
          T& operator[](const size_t i)       { assert(i<3); return i<=0 ? x : (1==i ? y : z); }
    const T& operator[](const size_t i) const { assert(i<3); return i<=0 ? x : (1==i ? y : z); }
    vec3<T> & normalize(T l=1) { *this = (*this)*(l/std::sqrt((*this)*(*this))); return *this; }
    T x,y,z;
};

template<typename T> T operator*(const vec3<T>& lhs, const vec3<T>& rhs) {
    T ret = T();
    for (size_t i=3; i--; ret+=lhs[i]*rhs[i]);
    return ret;
}

template<typename T> vec3<T> operator+(vec3<T> lhs, const vec3<T>& rhs) {
    for (size_t i=3; i--; lhs[i]+=rhs[i]);
    return lhs;
}

template<typename T> vec3<T> operator-(vec3<T> lhs, const vec3<T>& rhs) {
    for (size_t i=3; i--; lhs[i]-=rhs[i]);
    return lhs;
}

template<typename T,typename U> vec3<T> operator*(const vec3<T> &lhs, const U& rhs) {
    vec3<T> ret;
    for (size_t i=3; i--; ret[i]=lhs[i]*rhs);
    return ret;
}

template<typename T> vec3<T> operator-(const vec3<T> &lhs) {
    return lhs*T(-1);
}

typedef vec3<float> Vec3f;

#endif //__GEOMETRY_H__

