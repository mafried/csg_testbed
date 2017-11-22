#ifndef _H_MATH_HELPER
#define _H_MATH_HELPER

#include <cstring>

#include "vtkMatrix4x4.h"

template<typename T>
struct Matrix4x4
{
	T data[16];
};
template <>
struct Matrix4x4 <double>
{
	Matrix4x4(vtkMatrix4x4& m)
	{
		std::memcpy(data, m.GetData(), sizeof(data));
	}

	double data[16];
};


template<typename T>
struct Vector4
{
	T data[4];
};

template<typename T>
struct Vector3
{
	Vector3(T x, T y, T z) :
		data {x,y,z} 
	{
	}

	T data[3];
};

#endif 

