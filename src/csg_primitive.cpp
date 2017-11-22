#include <assert.h>

#include "vtkSphere.h"
#include "vtkBox.h"
#include "vtkCylinder.h"
#include "vtkPlane.h"
#include "vtkCone.h"
#include "vtkImplicitBoolean.h"
#include "vtkTransform.h"

#include "csg_primitive.h"

Matrix4x4<double> toTransform(const json& json)
{
	auto trans = vtkSmartPointer<vtkTransform>::New();
	auto pos = json.at("pos");
	
	auto rotIt = json.find("rot"); //rotation is optional.
	if (rotIt != json.end())
	{
		trans->RotateX(rotIt->at(0).get<double>());
		trans->RotateY(rotIt->at(1).get<double>());
		trans->RotateZ(rotIt->at(2).get<double>());
	}

	trans->Translate(
		pos.at(0).get<double>(),
		pos.at(1).get<double>(),
		pos.at(2).get<double>()
	);
	
	return *trans->GetMatrix();
}

Vector3<double> toSize(const json& json)
{	
	auto pos = json.at("bounds");

	return Vector3<double>(pos.at(0).get<double>(), pos.at(1).get<double>(), pos.at(2).get<double>());
}

CSGPrimitive::CSGPrimitive(const Matrix4x4<double>& transform) :
	m_transform(transform)
{
}

Sphere::Sphere(const json& json, const CSGNodeFactory & factory) : 
	CSGPrimitive(toTransform(json)),
	m_radius(json.at("radius").get<double>())
{
}

Sphere::Sphere(const Matrix4x4<double>& transform, double radius) :
	CSGPrimitive(transform),
	m_radius(radius)
{
}

vtkSmartPointer<vtkImplicitFunction> Sphere::createImplicitFunction() const
{
	auto sphere = vtkSmartPointer<vtkSphere>::New();	
	sphere->SetTransform(m_transform.data);
	sphere->SetRadius(m_radius);

	return sphere;
}

Box::Box(const json & json, const CSGNodeFactory & factory) : 
	CSGPrimitive(toTransform(json)),
	m_size(toSize(json))
{
}

Box::Box(const Matrix4x4<double>& transform, const Vector3<double> size) : 
	CSGPrimitive(transform), 
	m_size(size)
{
}

vtkSmartPointer<vtkImplicitFunction> Box::createImplicitFunction() const
{
	auto box = vtkSmartPointer<vtkBox>::New();
	box->SetTransform(m_transform.data);
	box->SetBounds(
		-m_size.data[0] * 0.5, m_size.data[0] * 0.5,
		-m_size.data[1] * 0.5, m_size.data[1] * 0.5,
		-m_size.data[2] * 0.5, m_size.data[2] * 0.5);

	return box;
}

Cylinder::Cylinder(const json & json, const CSGNodeFactory & factory) : 
	CSGPrimitive(toTransform(json)),
	m_radius(json.at("radius").get<double>()),
	m_height(json.at("height").get<double>())
{
}

Cylinder::Cylinder(const Matrix4x4<double>& transform, double radius, double height) : 
	CSGPrimitive(transform),
	m_radius(radius),
	m_height(height)
{
}

vtkSmartPointer<vtkImplicitFunction> Cylinder::createImplicitFunction() const
{
	//vtkCylinder is of endless height. This is why we introduce two clipping planes.

	auto cylinder = vtkSmartPointer<vtkCylinder>::New();
	cylinder->SetRadius(m_radius);

	auto clippingPlane1 = vtkSmartPointer<vtkPlane>::New();
	clippingPlane1->SetNormal(0, -1, 0);
	clippingPlane1->SetOrigin(0, -m_height * 0.5, 0);

	auto clippingPlane2 = vtkSmartPointer<vtkPlane>::New();
	clippingPlane2->SetNormal(0, 1, 0);
	clippingPlane2->SetOrigin(0, m_height * 0.5, 0);

	vtkSmartPointer<vtkImplicitBoolean> implicitBoolean = vtkSmartPointer<vtkImplicitBoolean>::New();
	implicitBoolean->AddFunction(cylinder);
	implicitBoolean->AddFunction(clippingPlane1);
	implicitBoolean->AddFunction(clippingPlane2);
	implicitBoolean->SetOperationTypeToIntersection();

	implicitBoolean->SetTransform(m_transform.data);
	
	return implicitBoolean;
}

Cone::Cone(const json & json, const CSGNodeFactory & factory) :
	CSGPrimitive(toTransform(json)),
	m_angle(json.at("angle").get<double>()),
	m_height(json.at("height").get<double>())
{
}

Cone::Cone(const Matrix4x4<double>& transform, double angle, double height) :
	CSGPrimitive(transform),
	m_angle(angle),
	m_height(height)
{
}

vtkSmartPointer<vtkImplicitFunction> Cone::createImplicitFunction() const
{
	//vtkCone is of endless height. This is why we introduce clipping planes.

	auto cone = vtkSmartPointer<vtkCone>::New();
	cone->SetAngle(m_angle);

	auto clippingPlane1 = vtkSmartPointer<vtkPlane>::New();
	clippingPlane1->SetNormal(-1, 0, 0);
	clippingPlane1->SetOrigin(0, 0, 0);
	
	auto clippingPlane2 = vtkSmartPointer<vtkPlane>::New();
	clippingPlane2->SetNormal(1, 0, 0);
	clippingPlane2->SetOrigin(m_height, 0, 0);

	vtkSmartPointer<vtkImplicitBoolean> implicitBoolean = vtkSmartPointer<vtkImplicitBoolean>::New();
	implicitBoolean->AddFunction(cone);	
	implicitBoolean->AddFunction(clippingPlane2);
	implicitBoolean->AddFunction(clippingPlane1);
	implicitBoolean->SetOperationTypeToIntersection();

	implicitBoolean->SetTransform(m_transform.data);

	return implicitBoolean;
}
