#include <vtkSmartPointer.h>
#include <vtkSampleFunction.h>
#include <vtkContourFilter.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageData.h>
#include <vtkImplicitBoolean.h>
#include <vtkBoundedPointSource.h>
#include <vtkSphere.h>
#include <vtkExtractPoints.h>
#include <vtkSphereSource.h>
#include <vtkGlyph3D.h>
#include <vtkFitImplicitFunction.h>


#include "csg_operator.h"
#include "csg_primitive.h"
#include "csg_node_factory.h"
#include "json.hpp"

using json = nlohmann::json;

int main(int, char *[])
{
	/*json jsonObj = {
		{ "operatorType", "union"},
		{ "childs", 
			{
				{ { "type", "sphere" }, { "pos",{ 0.3,0.0,0.0 } }, { "radius",0.5 } },
				{ { "type", "box" }, { "pos",{ -0.3,0.0,0.0 } }, { "bounds",{0.2,0.2,0.2} } },
				{
					{ "type", "operator" },
					{ "operatorType", "union" },
					{ "childs",
					{
						{ { "type", "sphere" },{ "pos",{ 1,0.0,0.0 } },{ "radius",0.2 } },
						{ { "type", "cylinder" },{ "pos",{ 1,1,0.0 } },{ "radius",0.2 },{ "height",0.2 } }
					}
					}
				}
			} 
		},		
	};*/

	json jsonObj = {
	{ "operatorType", "union"},
	{ "childs",
		{
			{ { "type", "cone" }, { "pos",{ 0.3,0.0,0.0 } }, { "rot",{ 0.0,0.0,90.0 } }, { "angle",30 }, { "height",1.0 } }
		}
	}
	};


	CSGNodeFactory factory;
	factory.registerType<Sphere>("sphere");
	factory.registerType<Box>("box");
	factory.registerType<Cylinder>("cylinder");
	factory.registerType<Cone>("cone");
	factory.registerType<CSGOperator>("operator");

	CSGOperator op(jsonObj, factory);

	/*vtkSmartPointer<vtkSphere> sphere1 =
		vtkSmartPointer<vtkSphere>::New();
	sphere1->SetCenter(.3, 0, 0);
	
	vtkSmartPointer<vtkSphere> sphere2 =
		vtkSmartPointer<vtkSphere>::New();
	sphere2->SetCenter(-.3, 0, 0);

	vtkSmartPointer<vtkImplicitBoolean> implicitBoolean =
		vtkSmartPointer<vtkImplicitBoolean>::New();
	implicitBoolean->AddFunction(sphere1);
	implicitBoolean->AddFunction(sphere2);
	implicitBoolean->SetOperationTypeToUnion();*/
	
	// Sample the function
	vtkSmartPointer<vtkSampleFunction> sample =
		vtkSmartPointer<vtkSampleFunction>::New();
	sample->SetImplicitFunction(op.createImplicitFunction()/*implicitBoolean*/);
	sample->SetModelBounds(-2, 2, -2, 2, -2, 2);
	sample->SetSampleDimensions(100, 100, 100);

	// Create an isosurface
	vtkSmartPointer<vtkContourFilter> contourFilter =
		vtkSmartPointer<vtkContourFilter>::New();
	contourFilter->SetInputConnection(sample->GetOutputPort());
	contourFilter->GenerateValues(1, 0, 0); // (numContours, rangeStart, rangeEnd)


	double radius = .01;
	vtkSmartPointer<vtkSphereSource> sphereSource =
		vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetRadius(radius);


	//vtkSmartPointer<vtkBoundedPointSource> pointSource =
	//	vtkSmartPointer<vtkBoundedPointSource>::New();
	//pointSource->SetNumberOfPoints(1000000);

	//double bounds[] = { xmin, xmax, ymin, ymax, zmin, zmax };

	//pointSource->SetBounds(bounds);


	/*vtkSmartPointer<vtkFitImplicitFunction> fit =
		vtkSmartPointer<vtkFitImplicitFunction>::New();
	fit->SetInputConnection(pointSource->GetOutputPort());
	fit->SetImplicitFunction(implicitBoolean);
	fit->SetThreshold(.01);
	fit->Update();*/
	
	vtkSmartPointer<vtkGlyph3D> glyph =
		vtkSmartPointer<vtkGlyph3D>::New();
	glyph->SetInputConnection(contourFilter->GetOutputPort()/*fit->GetOutputPort()*/);
	glyph->SetSourceConnection(sphereSource->GetOutputPort());
	glyph->ScalingOff();

	vtkSmartPointer<vtkPolyDataMapper> glyphMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	glyphMapper->SetInputConnection(glyph->GetOutputPort());
	glyphMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> glyphActor =
		vtkSmartPointer<vtkActor>::New();
	glyphActor->SetMapper(glyphMapper);


	// Visualize
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> interactor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);


	renderer->AddActor(glyphActor);
	renderer->SetBackground(.2, .3, .4);

	renderWindow->Render();
	interactor->Start();

	return EXIT_SUCCESS;
}
