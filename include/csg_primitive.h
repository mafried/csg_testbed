#ifndef _H_CSG_PRIMITIVE_
#define _H_CSG_PRIMITIVE_

#include "csg_node.h"
#include "math_helper.h"

class CSGPrimitive : public CSGNode 
{
public:
	CSGPrimitive(const Matrix4x4<double>& transform);
protected:
	Matrix4x4<double> m_transform;
};

class Sphere : public CSGPrimitive
{
public:
	Sphere(const json& json, const CSGNodeFactory& factory);
	Sphere(const Matrix4x4<double>& transform, double radius);
	virtual vtkSmartPointer<vtkImplicitFunction> createImplicitFunction() const override;
private: 
	double m_radius;
};

class Box : public CSGPrimitive
{
public:
	Box(const json& json, const CSGNodeFactory& factory);
	Box(const Matrix4x4<double>& transform, const Vector3<double> size);
	virtual vtkSmartPointer<vtkImplicitFunction> createImplicitFunction() const override;
private:
	Vector3<double> m_size;
};

class Cylinder : public CSGPrimitive
{
public:
	Cylinder(const json& json, const CSGNodeFactory& factory);
	Cylinder(const Matrix4x4<double>& transform, double radius, double height);
	virtual vtkSmartPointer<vtkImplicitFunction> createImplicitFunction() const override;
private:
	double m_radius;
	double m_height;
};

class Cone : public CSGPrimitive
{
public:
	Cone(const json& json, const CSGNodeFactory& factory);
	Cone(const Matrix4x4<double>& transform, double angle, double height);
	virtual vtkSmartPointer<vtkImplicitFunction> createImplicitFunction() const override;
private:
	double m_angle;
	double m_height;
};

#endif 

