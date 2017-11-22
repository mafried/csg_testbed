#include "csg_operator.h"
#include "csg_node_factory.h"

#include <assert.h>
#include <algorithm>

#include "vtkImplicitBoolean.h"

CSGOperator::CSGOperator(const json& json, const CSGNodeFactory& factory)
{
	auto opType = json.at("operatorType").get<std::string>();
	std::transform(opType.begin(), opType.end(), opType.begin(), ::tolower);

	if (opType == "intersection")
		m_type = CSGOperatorType::INTERSECTION;
	else if (opType == "union")
		m_type = CSGOperatorType::UNION;
	else if (opType == "difference")
		m_type = CSGOperatorType::DIFFERENCE;
	else
		throw std::runtime_error("Operator type does not exist.");

	auto childs = json.at("childs");
	for (json::iterator it = childs.begin(); it != childs.end(); ++it) {
		auto childType = it->at("type").get<std::string>();
		std::transform(childType.begin(), childType.end(), childType.begin(), ::tolower);

		m_childs.push_back(factory.create(childType, *it));
	}
}

CSGOperator::CSGOperator(CSGOperatorType type) :
	m_type(type)
{
}

vtkSmartPointer<vtkImplicitFunction> CSGOperator::createImplicitFunction() const 
{
	vtkSmartPointer<vtkImplicitBoolean> implicitBoolean = vtkSmartPointer<vtkImplicitBoolean>::New();

	switch (m_type)
	{
	case CSGOperatorType::INTERSECTION: 
		implicitBoolean->SetOperationTypeToIntersection();
		break; 
	case CSGOperatorType::UNION:
		implicitBoolean->SetOperationTypeToUnion();
		break;
	case CSGOperatorType::DIFFERENCE:
		implicitBoolean->SetOperationTypeToDifference();
		break;
	default: 
		assert(false);
	}

	for (const auto& child : m_childs)
	{
		implicitBoolean->AddFunction(child->createImplicitFunction());
	}

	return implicitBoolean;
}
